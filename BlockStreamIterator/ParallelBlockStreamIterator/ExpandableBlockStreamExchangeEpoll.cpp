/*
 * ExpandableBlockStreamExchangeEpoll.cpp
 *
 *  Created on: Aug 29, 2013
 *      Author: wangli
 */


#include <libconfig.h++>
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/select.h>
#include <assert.h>
#include <sys/epoll.h>
#include "ExpandableBlockStreamExchangeEpoll.h"
#include "ExpandableBlockStreamExchangeLowerEfficient.h"
#include "../../common/Logging.h"
#include "../../Environment.h"
#include "../../Executor/ExchangeTracker.h"
#include "../../configure.h"
#include "../../common/rename.h"
#include "../../utility/rdtsc.h"
#define BUFFER_SIZE_IN_EXCHANGE 100

ExpandableBlockStreamExchangeEpoll::ExpandableBlockStreamExchangeEpoll(State state)
:state(state){
	sem_open_.set_value(1);
	open_finished_=false;
	logging_=new ExchangeIteratorEagerLogging();
	assert(state.partition_key_index<100);
	winner_thread=0;
}
ExpandableBlockStreamExchangeEpoll::ExpandableBlockStreamExchangeEpoll(){
	sem_open_.set_value(1);
	open_finished_=false;
	logging_=new ExchangeIteratorEagerLogging();
	winner_thread=0;
}
ExpandableBlockStreamExchangeEpoll::~ExpandableBlockStreamExchangeEpoll() {
	// TODO Auto-generated destructor stub
	delete logging_;
}

bool ExpandableBlockStreamExchangeEpoll::open(const PartitionOffset& partition_offset){
	unsigned long long int start=curtick();
	if(sem_open_.try_wait()){
		winner_thread++;
		nexhausted_lowers=0;
		this->partition_offset=partition_offset;
		nlowers=state.lower_ip_list.size();

		for(unsigned i=0;i<nlowers;i++){
			received_block[i]=0;
		}

		socket_fd_lower_list=new int[nlowers];

		buffer=new BlockStreamBuffer(state.block_size,BUFFER_SIZE_IN_EXCHANGE,state.schema);
		ExpanderTracker::getInstance()->addNewStageEndpoint(pthread_self(),LocalStageEndPoint(stage_src,"Exchange",buffer));
		received_block_stream_=BlockStreamBase::createBlock(state.schema,state.block_size);
		block_for_socket_=new BlockContainer*[nlowers];
		for(unsigned i=0;i<nlowers;i++){
			block_for_socket_[i]=new BlockContainer(received_block_stream_->getSerializedBlockSize());
		}
		if(PrepareTheSocket()==false)
			return false;

		if(SetSocketNonBlocking(sock_fd)==false){
			return false;
		}

		logging_->log("[%ld] Open: nexhausted lowers=%d, nlower=%d",state.exchange_id,nexhausted_lowers,nlowers);

		if(RegisterExchange()==false){
			logging_->elog("Register Exchange with ID=%d fails!",state.exchange_id);
		}

		if(partition_offset==0){
			/*TODO: According to a bug reported by dsc, the master exchangeupper should check whether other
			 *  uppers have registered to exchangeTracker. Otherwise, the lower may fail to connect to the
			 *  exchangeTracker of some uppers when the lower nodes receive the exchagnelower, as some uppers
			 *  have not register the exchange_id to the exchangeTracker.
			*/
			logging_->log("[%ld] Synchronizing....",state.exchange_id);
			checkOtherUpperRegistered();
			logging_->log("[%ld] Synchronized!",state.exchange_id);
			logging_->log("[%ld] This exchange is the master one, serialize the iterator subtree to the children...",state.exchange_id);

			if(SerializeAndSendToMulti()==false)
				return false;

		}


		if(CreateReceiverThread()==false){
			return false;
		}


		perf_info_=ExpanderTracker::getInstance()->getPerformanceInfo(pthread_self());
		perf_info_->initialize();

		open_finished_=true;
		return true;
	}
	else{

		while(!open_finished_){
			usleep(1);
		}

		return true;
	}
}

bool ExpandableBlockStreamExchangeEpoll::next(BlockStreamBase* block){
//	if(ExpanderTracker::getInstance()->isExpandedThreadCallBack(pthread_self())){
//		logging_->log("<<<<<<<<<<<<<<<<<Exchange detected call back signal!>>>>>>%lx>>>>>>>>>>>\n",pthread_self());
//		printf("exchange received terminate request! %lx\n",pthread_self());
////		assert(false);
//		return false;
//	}
	if(this->checkTerminateRequest()){
		logging_->log("<<<<<<<<<<<<<<<<<Exchange detected call back signal!>>>>>>%lx>>>>>>>>>>>\n",pthread_self());
		return false;
	}

/**
 * In the initial implementation, busy waiting is used in while(), and consequently consumes
 * large CPU usage. I add usleep(1) in the while to release this problem. Perhaps, a better way
 * is to use conditioned wait.
 * TODO: better implementation based on conditioned wait.
 * --Li.
 * Mar. 30th, 2014.
 */

	while(nexhausted_lowers<nlowers){
		if(buffer->getBlock(*block)){
			perf_info_->processed_one_block();
			return true;
		}
		if(this->checkTerminateRequest()){
			logging_->log("<<<<<<<<<<<<<<<<<Exchange detected call back signal!>>>>>>%lx>>>>>>>>>>>\n",pthread_self());
			return false;
		}
		usleep(1);
	}
	/* all the lowers exchange are exhausted.*/
	if(buffer->getBlock(*block)){
		perf_info_->processed_one_block();
		return true;
	}
	else{
		return false;
	}

}

bool ExpandableBlockStreamExchangeEpoll::close(){
//	socket_fd_lower_list
//	buffer
//	received_block_stream_
//	block_for_socket_
	logging_->log("[%ld] Close: nexhausted lowers=%d, nlower=%d",state.exchange_id,nexhausted_lowers,nlowers);
	sem_open_.set_value(1);


	open_finished_=false;

	CancelReceiverThread();

	FileClose(epoll_fd_);

	for(unsigned i=0;i<nlowers;i++){
//		FileClose(this->socket_fd_lower_list[i]);
		delete block_for_socket_[i];;
	}

//	sleep(1);
	for(std::map<int,int>::iterator it=lower_sock_fd_to_index.begin();it!=lower_sock_fd_to_index.end();it++){
//		printf("upper %d is closed!\n",it->first);
		FileClose(it->first);
	}

	delete received_block_stream_;
	delete buffer;
//	printf("Buffer is freed in Exchange!\n");
	delete[] socket_fd_lower_list;
	delete[] block_for_socket_;
	CloseTheSocket();

	lower_sock_fd_to_index.clear();
	lower_ip_array.clear();



	Environment::getInstance()->getExchangeTracker()->LogoutExchange(ExchangeID(state.exchange_id,partition_offset));
	logging_->log("[%ld] ExchangeUpper is closed!",state.exchange_id);
//	for(unsigned i=0;i<nlowers;i++){
//		printf("Exchange: [%ld] consumes %d blocks from Lower[%d]\n",state.exchange_id,received_block[i],i);
//	}
	return true;
}

void ExpandableBlockStreamExchangeEpoll::print(){
	printf("Exchange upper[%ld]:",state.exchange_id);
	for(unsigned i=0;i<state.upper_ip_list.size();i++){
		printf("%s ",state.upper_ip_list[i].c_str());
	}
	printf("\nlower:");
	for(unsigned i=0;i<state.lower_ip_list.size();i++){
		printf("%s ",state.lower_ip_list[i].c_str());
	}
	printf("Partition key index:%d",state.partition_key_index);
	printf("\n---------\n");
	state.child->print();
}
bool ExpandableBlockStreamExchangeEpoll::PrepareTheSocket()
{
	struct sockaddr_in my_addr;

	//sock_fd is the socket of this node
	if((sock_fd=socket(AF_INET, SOCK_STREAM, 0))==-1)
	{
		perror("socket creation error!\n");
		return false;
	}
	my_addr.sin_family=AF_INET;

	/* apply for the port dynamicaly.*/
	if((socket_port=PortManager::getInstance()->applyPort())==0){
		logging_->elog("[%ld] Fails to apply a port for the socket. Reason: the PortManager is exhausted!",state.exchange_id);
	}
	logging_->log("[%ld] The applied port for socket is %d",state.exchange_id,socket_port);

//	printf("Upper open %d is created!\n",sock_fd);


	my_addr.sin_port=htons(socket_port);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero),8);

	/* Enable address reuse */
	int on=1;
	setsockopt(sock_fd, SOL_SOCKET,SO_REUSEADDR, &on, sizeof(on));
//	int size=1024*1024*64;
//	setsockopt(sock_fd,SOL_SOCKET,SO_RCVBUF,(const char*)&size,sizeof(int));



	if(bind(sock_fd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1)
	{
		perror("bind errors!\n");
		return false;
	}

	if(listen(sock_fd, nlowers)==-1)
	{
		perror("listen errors!\n");
		return false;
	}

	logging_->log("[%ld ]socket created as: %s:%d",state.exchange_id,inet_ntoa(my_addr.sin_addr),socket_port);

	return true;
}

void ExpandableBlockStreamExchangeEpoll::CloseTheSocket(){
	/* colse the sockets of the lowers*/
//	for(unsigned i=0;i<nlowers;i++){
//		FileClose(socket_fd_lower_list[i]);
//	}

	/* close the socket of this exchange*/
	FileClose(sock_fd);

//	printf("Upper %d is closed!\n",sock_fd);

	/* return the applied port to the port manager*/
	PortManager::getInstance()->returnPort(socket_port);
}

bool ExpandableBlockStreamExchangeEpoll::RegisterExchange(){
	ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
	std::ostringstream port_str;
	port_str<<socket_port;
	return et->RegisterExchange(ExchangeID(state.exchange_id,partition_offset),port_str.str());
}
bool ExpandableBlockStreamExchangeEpoll::checkOtherUpperRegistered(){
	ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
	for(unsigned i=0;i<state.upper_ip_list.size();i++){
		std::string ip=state.upper_ip_list[i];
		/* Repeatedly ask node with ip for port information untile the received port is other than 0, which means
		 * that the exchangeId on noede ip is registered to the exchangeTracker*/
		int wait_time_in_millisecond=1;
		while(et->AskForSocketConnectionInfo(ExchangeID(state.exchange_id,i),ip)==0){
			usleep(wait_time_in_millisecond);
			wait_time_in_millisecond=wait_time_in_millisecond<1000?wait_time_in_millisecond+20:1000;
		}
//		printf("ExchangeID[%lld] is synchronized in %s",state.exchange_id,ip.c_str());
	}
}
bool ExpandableBlockStreamExchangeEpoll::isMaster(){
	logging_->log("[%ld] master ip=%s, self ip=%s",state.exchange_id,state.upper_ip_list[0].c_str(),Environment::getInstance()->getIp().c_str());
	return Environment::getInstance()->getIp()==state.upper_ip_list[0];
}
bool ExpandableBlockStreamExchangeEpoll::SerializeAndSendToMulti(){
	IteratorExecutorMaster* IEM=IteratorExecutorMaster::getInstance();
	ExpandableBlockStreamExchangeLowerEfficient::State EIELstate(state.schema,state.child,state.upper_ip_list,state.block_size,state.exchange_id,state.partition_key_index);
//	unsigned times=1;
//	int i=0;
//	unsigned long long int start=curtick();
//	while(i++<times)
	for(unsigned i=0;i<state.lower_ip_list.size();i++){
		/* set the partition offset*/
		EIELstate.partition_offset=i;
		BlockStreamIteratorBase *EIEL=new ExpandableBlockStreamExchangeLowerEfficient(EIELstate);

		if(IEM->ExecuteBlockStreamIteratorsOnSite(EIEL,state.lower_ip_list[i])==false){
			logging_->elog("[%ld] Cannot send the serialized iterator tree to the remote node!\n",state.exchange_id);
			return false;
		}
//		printf("***************************** send iterator to %d,%d*********************************\n",state.exchange_id,i);
//		EIEL->~BlockStreamIteratorBase();
		delete EIEL;
	}
//	printf("AVG::%f\n",getSecond(start)/times);
	return true;
}

bool ExpandableBlockStreamExchangeEpoll::WaitForConnectionFromLowerExchanges(){
	//wait for the lower nodes send the connection information
	socklen_t sin_size=sizeof(struct sockaddr_in);
	struct sockaddr_in remote_addr;
	unsigned count=0;
	while(1){
		if(count>=nlowers){
			return true;
		}
		logging_->log("[%ld] Waiting for the socket connection from the lower exchange..",state.exchange_id);
		if((socket_fd_lower_list[count]=accept(sock_fd,(struct sockaddr*)&remote_addr,&sin_size))!=-1)
		{
			lower_ip_array.push_back(inet_ntoa(remote_addr.sin_addr));
			logging_->log("[%ld] The lower exchange <%s> is connected to the socket.",state.exchange_id,lower_ip_array[count].c_str());
			count++;
		}
	}

	return true;
}

bool ExpandableBlockStreamExchangeEpoll::CreateReceiverThread(){
	int error;
	error=pthread_create(&receiver_tid,NULL,receiver,this);
	if(error!=0){
		logging_->elog("[%ld] Failed to create receiver thread.",state.exchange_id);
		return false;
	}

//	pthread_create(&debug_tid,NULL,debug,this);
	return true;
}
void ExpandableBlockStreamExchangeEpoll::CancelReceiverThread(){
	pthread_cancel(receiver_tid);
	void *res=0;
	while(res!=PTHREAD_CANCELED){
		pthread_join(receiver_tid,&res);
	}
//	pthread_cancel(debug_tid);
}

void* ExpandableBlockStreamExchangeEpoll::receiver(void* arg){
	ExpandableBlockStreamExchangeEpoll* Pthis=(ExpandableBlockStreamExchangeEpoll*)arg;

	struct epoll_event event;
	struct epoll_event *events;

	int status;

	Pthis->epoll_fd_=epoll_create1(0);
	if(Pthis->epoll_fd_==-1){
		Pthis->logging_->elog("epoll create error!\n");
		return 0;
	}

	event.data.fd=Pthis->sock_fd;
	event.events=EPOLLIN | EPOLLET;
	status=epoll_ctl(Pthis->epoll_fd_,EPOLL_CTL_ADD,Pthis->sock_fd,&event);
	if(status==-1){
		Pthis->logging_->elog("epoll ctl error!\n");
		return 0;
	}

	events=(epoll_event*)calloc(Pthis->nlowers,sizeof(epoll_event));
	int fd_cur=0;

	while(true){
		usleep(1);
		const int event_count=epoll_wait(Pthis->epoll_fd_,events,Pthis->nlowers,-1);
		for(int i=0;i<event_count;i++){
			if((events[i].events & EPOLLERR)||(events[i].events & EPOLLHUP)||(!(events[i].events&EPOLLIN))){
				if(errno==EINTR){
					continue;
				}
				Pthis->logging_->elog("[%ld] epoll error,reason:%s\n",Pthis->state.exchange_id,strerror(errno));
				FileClose(events[i].data.fd);
				continue;
			}
			else if(Pthis->sock_fd==events[i].data.fd){
				/* We have a notification on the listening socket, which means one or more incoming connections.*/
				while(true){
					sockaddr in_addr;
					socklen_t in_len;
					int infd;
					char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];

					in_len=sizeof in_addr;
					infd=accept(Pthis->sock_fd,&in_addr,&in_len);
					if(infd==-1){
						if((errno==EAGAIN)||(errno==EWOULDBLOCK)){
							/* all the incoming connections are processed.*/
							break;
						}
						else{
							perror("accept error!  ");
							break;
						}
					}
					status=getnameinfo(&in_addr,in_len,hbuf,sizeof(hbuf),sbuf,sizeof(sbuf),NI_NUMERICHOST|NI_NUMERICSERV);
					if(status==0){
						Pthis->logging_->log("[%ld] Accepted connection on descriptor %d (host=%s, port=%s),id=%d\n",Pthis->state.exchange_id, infd, hbuf, sbuf,Pthis->state.exchange_id);
						Pthis->logging_->log("[%ld] Accepted connection on descriptor %d (host=%s, port=%s)\n",Pthis->state.exchange_id, infd, hbuf, sbuf);
						Pthis->lower_ip_array.push_back(hbuf);
						Pthis->lower_sock_fd_to_index[infd]=Pthis->lower_ip_array.size()-1;
						assert(Pthis->lower_ip_array.size()<=Pthis->state.lower_ip_list.size());
					}
					/*Make the incoming socket non-blocking and add it to the list of fds to monitor.*/
					if(!Pthis->SetSocketNonBlocking(infd)){
						return 0;
					}
					event.data.fd=infd;
					event.events=EPOLLIN|EPOLLET;
					status=epoll_ctl(Pthis->epoll_fd_,EPOLL_CTL_ADD, infd,&event);
					if(status==-1){
						perror("epoll_ctl");
						return 0;
					}
				}
				continue;
			}
			else{
				/* We have data on the fd waiting to be read.*/
				int done=0;
				while(true){
					int byte_received;
					int socket_fd_index=Pthis->lower_sock_fd_to_index[events[i].data.fd];

					byte_received=read(events[i].data.fd,(char*)Pthis->block_for_socket_[socket_fd_index]->getBlock()+Pthis->block_for_socket_[socket_fd_index]->GetCurSize(),Pthis->block_for_socket_[socket_fd_index]->GetRestSize());
					if(byte_received==-1){
						if(errno==EAGAIN){
							/*We have read all the data,so go back to the loop.*/
							break;
						}
						perror("read error!\n");
						done=1;
					}
					else if(byte_received==0){
						/* End of file. The remote has closed the connection.*/
						done=1;
						break;
					}


//					assert(byte_received==Pthis->block_for_socket_[socket_fd_index]->);
					/* The data is successfully read.*/

					Pthis->block_for_socket_[socket_fd_index]->IncreaseActualSize(byte_received);
					if(Pthis->block_for_socket_[socket_fd_index]->GetRestSize()>0)
						continue;
//					const int tuples=*(int*)((char*)Pthis->block_for_socket_[socket_fd_index]->getBlock()+Pthis->block_for_socket_[socket_fd_index]->GetMaxSize()-2*sizeof(int));
//					if(tuples>65536){
//						printf("The %d-th block is received from Lower[%s,fd=%d], tuples=%d\n",Pthis->received_block[socket_fd_index],Pthis->lower_ip_array[socket_fd_index].c_str(),events[i].data.fd,tuples);
//						assert(false);
//					}
//					assert(*(int*)((char*)Pthis->block_for_socket_[socket_fd_index]->getBlock()+Pthis->block_for_socket_[socket_fd_index]->GetMaxSize()-2*sizeof(int))<65536);
					Pthis->logging_->log("[%ld] The %d-th block is received from Lower[%s]",Pthis->state.exchange_id,Pthis->received_block[socket_fd_index],Pthis->lower_ip_array[socket_fd_index].c_str());
					Pthis->received_block[socket_fd_index]++;
					Pthis->received_block_stream_->deserialize((Block*)Pthis->block_for_socket_[socket_fd_index]);
					Pthis->block_for_socket_[socket_fd_index]->reset();
					const bool isLastBlock=Pthis->received_block_stream_->Empty();
							//((BlockReadable*)Pthis->block_for_socket_[socket_fd_index])->IsLastBlock();
					if(!isLastBlock){
						Pthis->buffer->insertBlock(Pthis->received_block_stream_);
					}
					else{
						Pthis->logging_->log("[%ld] *****This block is the last one.",Pthis->state.exchange_id);
						Pthis->nexhausted_lowers++;

						if(Pthis->nexhausted_lowers==Pthis->nlowers){
							/*
							 * When all the exchange lowers are exhausted, notify the buffer
							 * that the input data is completely received.
							 */
							Pthis->buffer->setInputComplete();
						}


						Pthis->logging_->log("[%ld] <<<<<<<<<<<<<<<<nexhausted_lowers=%d>>>>>>>>>>>>>>>>exchange=(%d,%d)",Pthis->state.exchange_id,Pthis->nexhausted_lowers,Pthis->state.exchange_id,Pthis->partition_offset);
						Pthis->SendBlockAllConsumedNotification(events[i].data.fd);
						Pthis->logging_->log("[%ld] This notification (all the blocks in the socket buffer are consumed) is send to the lower[%s] exchange=(%d,%d).\n",Pthis->state.exchange_id,Pthis->lower_ip_array[socket_fd_index].c_str(),Pthis->state.exchange_id,Pthis->partition_offset);

					}
				}
				if(done){
					Pthis->logging_->log ("[%ld] Closed connection on descriptor %d[%s]\n",Pthis->state.exchange_id,
	                          events[i].data.fd,Pthis->lower_ip_array[Pthis->lower_sock_fd_to_index[events[i].data.fd]].c_str());
//					Pthis->nexhausted_lowers++;
	                  /* Closing the descriptor will make epoll remove it
	                     from the set of descriptors which are monitored. */
//	                  FileClose (events[i].data.fd);
//	                  printf("Closed connection on descriptor %d[%s]\n",
//	                          events[i].data.fd,Pthis->lower_ip_array[Pthis->lower_sock_fd_to_index[events[i].data.fd]].c_str());
//	                  Pthis->lower_sock_fd_to_index.erase(Pthis->lower_sock_fd_to_index.find(events[i].data.fd));
				}
			}
		}
	}

}

void ExpandableBlockStreamExchangeEpoll::SendBlockBufferedNotification(int target_socket_fd){
	char content='c';
	if(send(target_socket_fd,&content,sizeof(char),0)==-1){
		logging_->log("[%ld] Send error!\n",state.exchange_id);
		return ;
	}

}
void ExpandableBlockStreamExchangeEpoll::SendBlockAllConsumedNotification(int target_socket_fd){
//	FileClose(target_socket_fd);
//	return;
	char content='e';
	if(send(target_socket_fd,&content,sizeof(char),MSG_WAITALL)==-1){
		logging_->log("[%ld] Send error!\n",state.exchange_id);
		return ;
	}
}

bool ExpandableBlockStreamExchangeEpoll::SetSocketNonBlocking(int socket_fd){
	int flags, s;

	flags = fcntl (socket_fd, F_GETFL, 0);
	if (flags == -1)
	{
	  perror ("fcntl");
	  return false;
	}

	flags |= O_NONBLOCK;
	s = fcntl (socket_fd, F_SETFL, flags);
	if (s == -1)
	{
	  perror ("fcntl");
	  return false;
	}

	return true;
}


void* ExpandableBlockStreamExchangeEpoll::debug(void* arg){
	ExpandableBlockStreamExchangeEpoll* Pthis=(ExpandableBlockStreamExchangeEpoll*)arg;
	while(true){
//		if(Pthis->state.exchange_id==2){
		printf("Upper: %d blocks in buffer.\n",Pthis->buffer->getBlockInBuffer());
		usleep(100000);
//		}
	}
}
