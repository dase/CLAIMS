/*
 * SocketSender.cpp
 *
 *  Created on: Aug 8, 2013
 *      Author: wangli
 */
#include <stdlib.h>
#include <error.h>
#include <string.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
int main_sockersender(int argc,char** argv){
//	int main(int argc,char** argv){

	printf("%d ,%d ,%d\n",EAGAIN,EWOULDBLOCK,EINTR);

	int sock_fd, recvbytes;
	int value=1;
	int block_size=atoi(argv[1]);

	char buffer[50000];
	printf("value=%d",value);
	struct hostent* host;
	struct sockaddr_in serv_addr;
	const char* name=argv[2];
	if((host=gethostbyname(name))==0)
	{
		perror("gethostbyname errors!\n");
		return 1;
	}
	if((sock_fd = socket(AF_INET, SOCK_STREAM,0))==-1)
	{
		perror("socket create errors!\n");
		return 1;
	}
	int size=1024*atoi(argv[3]);
//	setsockopt(sock_fd,SOL_SOCKET,SO_SNDBUF,(const char*)&size,sizeof(int));


	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(1234);
	serv_addr.sin_addr=*((struct in_addr*)host->h_addr);
	bzero(&(serv_addr.sin_zero),8);
	if(connect(sock_fd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr))==-1)
	{
		perror("connection errors!\n");
		return 1;
	}

	  int actual_size;
	  unsigned  size_of=sizeof(actual_size);
	  getsockopt(sock_fd,SOL_SOCKET,SO_SNDBUF,&actual_size,&size_of);
	  printf("The actual send buffer is %d K bytes.\n",actual_size/1024);


	int send_count=0;
	int send_total_bytes=0;
	int while_count=0;
	while(1)
	{
//		getchar();
		*(int*)buffer=value;
		int sended=0;
		while(sended!=block_size){
			int bytes;
			printf("before send || size=%d\n",block_size-sended);
			if((bytes=send(sock_fd,&buffer,block_size-sended,MSG_DONTWAIT))==-1)
			{
				 if (errno == EAGAIN){
					 break;
				 }
				 printf("Error=%d\n",errno);
				perror("Send error!\n");
				break;
			}
			sended+=bytes;
			send_total_bytes+=bytes;
			if((while_count++)%5000==0){
				printf("send bytes: %d",bytes);
			}

		}
//		recv(sock_fd,&buffer,1,0);
//		printf("I have sent the message [%d]\n",value);

		value++;
		printf("send block=%d, send total bytes=%d k bytes\n",send_count++,send_total_bytes/1024);

//		if(value==atoi(argv[3])){
//			break;
//		}
	}

	close(sock_fd);
}



