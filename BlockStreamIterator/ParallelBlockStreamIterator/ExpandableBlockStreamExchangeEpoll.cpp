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
#include "ExpandableBlockStreamExchangeLower.h"
#include "ExpandableBlockStreamExchangeLowerEfficient.h"
#include "../../Block/BlockReadableFix.h"
#include "../../Logging.h"
#include "../../PortManager.h"
#include "../../Environment.h"
#include "../../Executor/ExchangeTracker.h"
#include "../../configure.h"
#include "../../rename.h"
ExpandableBlockStreamExchangeEpoll::ExpandableBlockStreamExchangeEpoll(State state)
:state(state){
	sem_open_.set_value(1);
	open_finished_=false;
	logging_=new ExchangeIteratorEagerLogging();
}
ExpandableBlockStreamExchangeEpoll::ExpandableBlockStreamExchangeEpoll(){
	sem_open_.set_value(1);
	open_finished_=false;
	logging_=new ExchangeIteratorEagerLogging();
}
ExpandableBlockStreamExchangeEpoll::~ExpandableBlockStreamExchangeEpoll() {
	// TODO Auto-generated destructor stub
}

bool ExpandableBlockStreamExchangeEpoll::open(const PartitionOffset& partition_offset){

	if(sem_open_.try_wait()){

		nexhausted_lowers=0;
		nlowers=state.lower_ip_list.size();

		for(unsigned i=0;i<nlowers;i++){
			received_block[i]=0;
		}

		socket_fd_lower_list=new int[nlowers];
//		lower_ip_array=new std::string[nlowers];

		buffer=new BlockStreamBuffer(state.block_size,nlowers*2000,state.schema);
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

		if(RegisterExchange()==false){
			logging_->elog("Register Exchange with ID=%l fails!",state.exchange_id);
		}

		if(isMaster()){
			/*TODO: According to a bug reported by dsc, the master exchangeupper should check whether other
			 *  uppers have registered to exchangeTracker. Otherwise, the lower may fails to connection to the
			 *  exchangeTracker of some uppers when the lower nodes receive the exchagnelower, as some uppers
			 *  have not register the exchange_id to the exchangeTracker.
			*/
			logging_->log("Synchronizing....");
			checkOtherUpperRegistered();
			logging_->log("Synchronized!");
			logging_->log("This exchange is the master one, serialize the iterator subtree to the children...");
			if(SerializeAndSendToMulti()==false)
				return false;
		}


//		if(WaitForConnectionFromLowerExchanges()==false){
//			return false;
//		}

		if(CreateReceiverThread()==false){
			return false;
		}




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
	while(nexhausted_lowers<nlowers){
		if(buffer->getBlock(*block)){
			return true;
		}
	}
	/* all the lowers exchange are exhausted.*/
	return buffer->getBlock(*block);

}

bool ExpandableBlockStreamExchangeEpoll::close(){

	sem_open_.set_value(1);


	open_finished_=false;

	CancelReceiverThread();

	for(unsigned i=0;i<nlowers;i++){
//		FileClose(this->socket_fd_lower_list[i]);
		block_for_socket_[i]->~BlockContainer();
	}


	received_block_stream_->~BlockStreamBase();
	buffer->~BlockStreamBuffer();

	delete[] socket_fd_lower_list;
	delete[] block_for_socket_;
	CloseTheSocket();

	lower_sock_fd_to_index.clear();
	lower_ip_array.clear();

	Environment::getInstance()->getExchangeTracker()->LogoutExchange(state.exchange_id);

	return true;
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
		logging_->elog("Fails to apply a port for the socket. Reason: the PortManager is exhausted!");
	}
	logging_->log("The applied port for socket is %d",socket_port);

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

	logging_->log("socket created as: %s:%d",inet_ntoa(my_addr.sin_addr),socket_port);

	return true;
}

void ExpandableBlockStreamExchangeEpoll::CloseTheSocket(){
	/* colse the sockets of the lowers*/
//	for(unsigned i=0;i<nlowers;i++){
//		FileClose(socket_fd_lower_list[i]);
//	}

	/* close the socket of this exchange*/
	FileClose(sock_fd);

	/* return the applied port to the port manager*/
	PortManager::getInstance()->returnPort(socket_port);
}

bool ExpandableBlockStreamExchangeEpoll::RegisterExchange(){
	ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
	std::ostringstream port_str;
	port_str<<socket_port;
	return et->RegisterExchange(state.exchange_id,port_str.str());
}
bool ExpandableBlockStreamExchangeEpoll::checkOtherUpperRegistered(){
	ExchangeTracker* et=Environment::getInstance()->getExchangeTracker();
	for(unsigned i=0;i<state.upper_ip_list.size();i++){
		std::string ip=state.upper_ip_list[i];
		/* Repeatedly ask node with ip for port information untile the received port is other than 0, which means
		 * that the exchangeId on noede ip is registered to the exchangeTracker*/
		while(et->AskForSocketConnectionInfo(state.exchange_id,ip)==0){
			usleep(1);
		}
		logging_->log("ExchangeID[%lld] is synchronized in %s",state.exchange_id,ip.c_str());
	}
}
bool ExpandableBlockStreamExchangeEpoll::isMaster(){
	logging_->log("master ip=%s, self ip=%s",state.upper_ip_list[0].c_str(),Environment::getInstance()->getIp().c_str());
	return Environment::getInstance()->getIp()==state.upper_ip_list[0];
}
bool ExpandableBlockStreamExchangeEpoll::SerializeAndSendToMulti(){
	IteratorExecutorMaster* IEM=IteratorExecutorMaster::getInstance();
	ExpandableBlockStreamExchangeLowerEfficient::State EIELstate(state.schema,state.child,state.upper_ip_list,state.block_size,state.exchange_id,state.partition_key_index);
	for(unsigned i=0;i<state.lower_ip_list.size();i++){
		/* set the partition offset*/
		EIELstate.partition_offset=i;
		BlockStreamIteratorBase *EIEL=new ExpandableBlockStreamExchangeLowerEfficient(EIELstate);

		if(IEM->ExecuteBlockStreamIteratorsOnSite(EIEL,state.lower_ip_list[i])==false){
			logging_->elog("Cannot send the serialized iterator tree to the remote node!\n");
			return false;
		}
		EIEL->~BlockStreamIteratorBase();
	}

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
		logging_->log("Waiting for the socket connection from the lower exchange..");
		if((socket_fd_lower_list[count]=accept(sock_fd,(struct sockaddr*)&remote_addr,&sin_size))!=-1)
		{
			lower_ip_array.push_back(inet_ntoa(remote_addr.sin_addr));
			logging_->log("The lower exchange <%s> is connected to the socket.",lower_ip_array[count].c_str());
			count++;
		}
	}

	return true;
}

bool ExpandableBlockStreamExchangeEpoll::CreateReceiverThread(){
	int error;
	error=pthread_create(&receiver_tid,NULL,receiver,this);
	if(error!=0){
		logging_->elog("Failed to create receiver thread.");
		return false;
	}

	pthread_create(&debug_tid,NULL,debug,this);
	return true;
}
void ExpandableBlockStreamExchangeEpoll::CancelReceiverThread(){
	pthread_cancel(receiver_tid);
	pthread_cancel(debug_tid);
}

void* ExpandableBlockStreamExchangeEpoll::receiver(void* arg){
	ExpandableBlockStreamExchangeEpoll* Pthis=(ExpandableBlockStreamExchangeEpoll*)arg;

	struct epoll_event event;
	struct epoll_event *events;

	int status;

	int efd=epoll_create1(0);
	if(efd==-1){
		Pthis->logging_->elog("epoll create error!\n");
		return 0;
	}

	event.data.fd=Pthis->sock_fd;
	event.events=EPOLLIN | EPOLLET;
	status=epoll_ctl(efd,EPOLL_CTL_ADD,Pthis->sock_fd,&event);
	if(status==-1){
		Pthis->logging_->elog("epoll ctl error!\n");
		return 0;
	}

	events=(epoll_event*)calloc(Pthis->nlowers,sizeof(epoll_event));

	while(true){
		const int event_count=epoll_wait(efd,events,Pthis->nlowers,-1);
		for(int i=0;i<event_count;i++){
			if((events[i].events & EPOLLERR)||(events[i].events & EPOLLHUP)||(!(events[i].events&EPOLLIN))){
				if(errno==EINTR){
					continue;
				}
				Pthis->logging_->elog("epoll error,reason:%s\n",strerror(errno));
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
							perror("accept error!");
							break;
						}
					}
					status=getnameinfo(&in_addr,in_len,hbuf,sizeof(hbuf),sbuf,sizeof(sbuf),NI_NUMERICHOST|NI_NUMERICSERV);
					if(status==0){
						Pthis->logging_->log("Accepted connection on descriptor %d (host=%s, port=%s)\n", infd, hbuf, sbuf);
						Pthis->lower_ip_array.push_back(hbuf);
						Pthis->lower_sock_fd_to_index[infd]=Pthis->lower_sock_fd_to_index.size()-1;


					}
					/*Make the incoming socket non-blocking and add it to the list of fds to monitor.*/
					if(!Pthis->SetSocketNonBlocking(infd)){
						return 0;
					}
					event.data.fd=infd;
					event.events=EPOLLIN|EPOLLET;
					status=epoll_ctl(efd,EPOLL_CTL_ADD, infd,&event);
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
					Pthis->logging_->log("The %d-th block is received from Lower[%s]",Pthis->received_block[socket_fd_index],Pthis->lower_ip_array[socket_fd_index].c_str());
					Pthis->received_block[socket_fd_index]++;
					Pthis->received_block_stream_->deserialize((Block*)Pthis->block_for_socket_[socket_fd_index]);
					Pthis->block_for_socket_[socket_fd_index]->reset();
					const bool isLastBlock=Pthis->received_block_stream_->Empty();
							//((BlockReadable*)Pthis->block_for_socket_[socket_fd_index])->IsLastBlock();
					if(!isLastBlock){
						Pthis->buffer->insertBlock(Pthis->received_block_stream_);
					}
					else{
						Pthis->logging_->log("*****This block is the last one.");
						Pthis->nexhausted_lowers++;
						Pthis->logging_->log("<<<<<<<<<<<<<<<<nexhausted_lowers=%d>>>>>>>>>>>>>>>>",Pthis->nexhausted_lowers);
						Pthis->SendBlockAllConsumedNotification(events[i].data.fd);
						Pthis->logging_->log("This notification (all the blocks in the socket buffer are consumed) is send to the lower[%s].",Pthis->lower_ip_array[socket_fd_index].c_str());

					}
				}
				if(done){
					Pthis->logging_->log ("Closed connection on descriptor %d\n",
	                          events[i].data.fd);

	                  /* Closing the descriptor will make epoll remove it
	                     from the set of descriptors which are monitored. */
	                  FileClose (events[i].data.fd);
				}
			}
		}
	}

}

void ExpandableBlockStreamExchangeEpoll::SendBlockBufferedNotification(int target_socket_fd){
	char content='c';
	if(send(target_socket_fd,&content,sizeof(char),0)==-1){
		logging_->log("Send error!\n");
		return ;
	}

}
void ExpandableBlockStreamExchangeEpoll::SendBlockAllConsumedNotification(int target_socket_fd){
	char content='e';
	if(send(target_socket_fd,&content,sizeof(char),MSG_WAITALL)==-1){
		logging_->log("Send error!\n");
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
//		printf("Upper: %d blocks in buffer.\n",Pthis->buffer->getBlockInBuffer());
		usleep(100000);
	}
}
