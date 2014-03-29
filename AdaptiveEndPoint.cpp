/*
 * AdaptiveActor.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: wangli
 */
#include <iostream>
#include <libconfig.h++>
#include <sys/socket.h>
#include <netdb.h>
#include <sstream>
#include <arpa/inet.h>
#include "AdaptiveEndPoint.h"
#include "Debug.h"
#include "Logging.h"
#include "utility/ThreadSafe.h"
AdaptiveEndPoint::AdaptiveEndPoint(const char* name,  std::string ip, std::string port)
:Theron::EndPoint(name, ("tcp://"+ip+":"+port).c_str()){
	logging_=new AdaptiveEndPointLogging();

	logging_->log("The AdaptiveEndPoint is created as %s:%s",ip.c_str(),port.c_str());
	framework=new Theron::Framework(*(EndPoint*)this);
	connectionActor=new AdaptiveEndPoint::ConnectionActor(this,("ConnectionActor://"+ip+":"+port).c_str());

	if(SayHelloToCoordinator(ip,port)==false){
		logging_->elog("Error occurs when saying hello to the coordinator!");
	}

	logging_->log("Get Coordinator EndPoint Port...");

	int coordinator_endpoint_port;
	if((coordinator_endpoint_port=GetCoordinatorEndPointPort())==-1){
		logging_->elog("Error occurs when getting the coordinator EndPoint port");
	}

	logging_->log("Connect to Coordinator EndPoint...");
	if(ConnectToCoordinateEndPoint(coordinator_endpoint_port)==false){
		logging_->elog("Error occurs when connecting to the coordinator EndPoint");
	}

	logging_->log("Waiting for the Ready signal from the Coordinator.");
	if(WaitForReadySignalFromCoordinator()==false){
		logging_->elog("Error occurs when connecting to the coordinator EndPoint");
	}
	close(socket_coor);

}

AdaptiveEndPoint::~AdaptiveEndPoint() {
	// TODO Auto-generated destructor stub
//	return;
	connectionActor->~Actor();
	framework->~Framework();
//	this->~EndPoint();
}

bool AdaptiveEndPoint::SayHelloToCoordinator(std::string ip,std::string port){
	libconfig::Config cfg;
	cfg.readFile(COOR);
	ip_coor=(const char *)cfg.lookup("coordinator.ip");
	std::string coord_port=(const char*)cfg.lookup("coordinator.port");
	int recvbytes;

	struct hostent host;
	struct sockaddr_in serv_addr;

	if((ThreadSafe::gethostbyname_ts(host,ip_coor.c_str()))==0)
	{
		logging_->elog("gethostbyname errors!\n");
		assert(false);
		return false;
	}
	if((socket_coor = socket(AF_INET, SOCK_STREAM,0))==-1)
	{
		logging_->elog("socket create errors!\n");
		assert(false);
		return false;
	}

	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(atoi(coord_port.c_str()));
	serv_addr.sin_addr=*((struct in_addr*)host.h_addr);
	bzero(&(serv_addr.sin_zero),8);
	if(connect(socket_coor,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr))==-1)
	{
		logging_->elog("connection errors when connecting to %s:%s! Reason:%s",inet_ntoa(serv_addr.sin_addr),coord_port.c_str(),strerror(errno));
		assert(false);
		return false;
	}

	int port_send=atoi(port.c_str());
	if(send(socket_coor,&port_send,sizeof(int),0)==-1)
	{
		logging_->elog("Error occurs when sending the hello message to the coordinator!\n");
		assert(false);
		return false;
	}
	return true;
}
int AdaptiveEndPoint::GetCoordinatorEndPointPort(){
	int recvbytes;
	int port;
	if((recvbytes=recv(socket_coor,&port,sizeof(int),0))==-1){
		logging_->elog("recv error!\n");
		return -1;
	}
	logging_->log("Get CoordinatorEndPoint is successfully! The Coordinator Theron EndPoint is %s:%d",ip_coor.c_str(),port);
	return port;

}
bool AdaptiveEndPoint::WaitForReadySignalFromCoordinator(){
	int recvbytes;
	char signal;
	if((recvbytes=recv(socket_coor,&signal,sizeof(char),0))==-1){
		logging_->elog("recv error!\n");
		return false;
	}
	logging_->log("Join to the EndPoint network successfully!");
	return true;
}
bool AdaptiveEndPoint::ConnectToCoordinateEndPoint(int port){
	std::ostringstream os;
	os<<"tcp://"<<ip_coor<<":"<<port;
//	sleep(1);
	if(!ConnectToRemoteEndPoint(os.str().c_str())){
		logging_->elog("Check whether network is enabled! %s",os.str().c_str());
		assert(false);
		return false;
	}

	logging_->log("ConnectToCoordiateEndPoint is successful!");
	return true;
}
bool AdaptiveEndPoint::ConnectToRemoteEndPoint(std::string location){
	return this->Connect(location.c_str());
}
AdaptiveEndPoint::ConnectionActor::ConnectionActor(AdaptiveEndPoint* AEP, const char* Name)
:Actor(*(AEP->framework),Name),AEP(AEP){
	RegisterHandler(this,&AdaptiveEndPoint::ConnectionActor::ReceiveNodeStatus256);

}

void AdaptiveEndPoint::ConnectionActor::ReceiveNodeStatus256(const Message256 &message, const Theron::Address from){
	NodeConnectionMessage NCM=NodeConnectionMessage::deserialize(message);
	std::ostringstream str;
	str<<"tcp://"+NCM.ip<<":"<<NCM.port;
	if(!AEP->ConnectToRemoteEndPoint(str.str().c_str())){
		AEP->logging_->elog("Check whether the Network is enabled!");
	}
	Send(0,from);


	AEP->logging_->log("Successfully connected to the EndPoint of new node through %s",str.str().c_str());
}
