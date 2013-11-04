/*
 * Socket.cpp
 *
 *  Created on: Jun 24, 2013
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


#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SERVPORT 3333
int mainSocket()
{
	int master;
	printf("master (0) or slave (1) ?\n");
	scanf("%d",&master);
	if(master==0)
	{
		char buf[1024];
		int sock_fd, client_fd;
		socklen_t sin_size;
		struct sockaddr_in my_addr;
		struct sockaddr_in remote_addr;
		if((sock_fd=socket(AF_INET, SOCK_STREAM, 0))==-1)
		{
			perror("socket creation error!\n");
			return 1;
		}
		my_addr.sin_family=AF_INET;
		my_addr.sin_port=htons(SERVPORT);
		my_addr.sin_addr.s_addr = INADDR_ANY;
		bzero(&(my_addr.sin_zero),8);
		if(bind(sock_fd,(struct sockaddr *)&my_addr, sizeof(struct sockaddr))==-1)
		{
			perror("bind errors!\n");
			return 1;
		}
		if(listen(sock_fd, 10)==-1)
		{
			perror("listen errors!\n");
		}
		while(1)
		{
			sin_size=sizeof(struct sockaddr_in);
			if((client_fd=accept(sock_fd, (struct sockaddr*)&remote_addr,&sin_size))==-1)
			{
				perror("accept errors!\n");
				continue;
			}
			printf("received a connection from %s\n", inet_ntoa(remote_addr.sin_addr));
			while(1)
			{
				if(!fork())
				{
					if(send(client_fd,"Hello, you are connected!\n",26,0)==-1)
					{
						perror("Send error!\n");
					}
					printf("I have sended the message. \n");
					int recvbytes;
					if((recvbytes=recv(client_fd,buf,100,0))==-1)
					{
						perror("recv error!\n");
						return 1;
					}
					buf[recvbytes]='\0';
					printf("Received %d bytes: %s\n",recvbytes,buf);
//					close(client_fd);
					return 0;
				}

				sleep(1);

			}
			close(client_fd);
		}
	}
	else
	{
		int sock_fd, recvbytes;
		char buf[100];
		struct hostent* host;
		struct sockaddr_in serv_addr;
		const char* name="10.11.1.224";
		if((host=gethostbyname(name))==0)
		{
			perror("gethostbyname errors!\n");
			return 1;
		}
		if((sock_fd = socket(AF_INET, SOCK_STREAM,0))==-1)
		{
			perror("socket craetion errors!\n");
			return 1;
		}
		serv_addr.sin_family=AF_INET;
		serv_addr.sin_port=htons(SERVPORT);
		serv_addr.sin_addr=*((struct in_addr*)host->h_addr);
		bzero(&(serv_addr.sin_zero),8);
		if(connect(sock_fd,(struct sockaddr *)&serv_addr, sizeof(struct sockaddr))==-1)
		{
			perror("connection errors!\n");
			return 1;
		}
		while(1)
		{
			if((recvbytes=recv(sock_fd,buf,100,0))==-1)
			{
				perror("recv error!\n");
				return 1;
			}
			buf[recvbytes]='\0';
			printf("Received %d bytes: %s\n",recvbytes,buf);
			if(send(sock_fd,"I received the message!\n",26,0)==-1)
			{
				perror("Send error!\n");
			}
			printf("I have sent the message. \n");
			sleep(1);
		}
		close(sock_fd);

	}

}


