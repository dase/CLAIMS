/*
 * select.cpp
 *
 *  Created on: Aug 15, 2013
 *      Author: wangli
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "../utility/rdtsc.h"

#define MYPORT 1234    // the port users will be connecting to

#define BACKLOG 20     // how many pending connections queue will hold

#define BUF_SIZE 50000

int fd_A[BACKLOG];    // accepted connection fd

int conn_amount;    // current connection amount

unsigned message_count[BACKLOG];

unsigned long received_bytes;

unsigned long long int start;

unsigned long block_count;

void showclient()

{

    int i;

    printf("client amount: %d\n", conn_amount);

    for (i = 0; i < BACKLOG; i++) {

        printf("[%d]:%d  ", i, fd_A[i]);

    }

    printf("\n\n");

}
void* print(void* ){
	while(1){
		printf("Throughput: %5.2f M/s, aggregated blocks: %d \n",(float)received_bytes/1024/1024/getSecond(start),block_count);
		sleep(1);
	}
}


int main234234(int argc, char** argv)
{

	unsigned block_size=atoi(argv[1]);
	start=curtick();
	received_bytes=0;
	block_count=0;
	pthread_t tid;
	pthread_create(&tid,NULL,print,0);
    int sock_fd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct sockaddr_in server_addr;    // server address information
    struct sockaddr_in client_addr; // connector's address information
    socklen_t sin_size;
    int yes = 1;
    char buf[BUF_SIZE];
    int ret;
    int i;
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
       perror("socket");
        exit(1);
    }

    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    server_addr.sin_family = AF_INET;         // host byte order
    server_addr.sin_port = htons(MYPORT);     // short, network byte order
    server_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
    memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

    if (bind(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sock_fd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    printf("listen port %d\n", MYPORT);
    fd_set fdsr;
    int maxsock;
    struct timeval tv;
    conn_amount = 0;
    sin_size = sizeof(client_addr);
    maxsock = sock_fd;

    unsigned selected=0;
    while (1) {
        // initialize file descriptor set
        FD_ZERO(&fdsr);
        FD_SET(sock_fd, &fdsr);
        // timeout setting
        tv.tv_sec = 30;
        tv.tv_usec = 0;

        // add active connection to fd set
        for (i = 0; i < BACKLOG; i++) {
            if (fd_A[i] != 0) {
                FD_SET(fd_A[i], &fdsr);
            }
        }

        ret = select(maxsock + 1, &fdsr, NULL, NULL, &tv);
        if (ret < 0) {
            perror("select");
            break;
        } else if (ret == 0) {
            printf("timeout\n");
            continue;
        }
        selected++;

//        printf("selected!\n");
        // check every fd in the set
        for (i = 0; i < conn_amount; i++) {
//            for (i = 0; i < 2; i++) {
            if (FD_ISSET(fd_A[i], &fdsr)) {
            	message_count[i]++;
//            	printf("count[%d]=%d\n",i,message_count[i]);
//            	continue;
                ret = recv(fd_A[i], buf, block_size, MSG_WAITALL);
                if (ret <= 0) {        // client close
                    printf("client[%d] close\n", i);
                    close(fd_A[i]);
                    FD_CLR(fd_A[i], &fdsr);
                    fd_A[i] = 0;
                } else {        // receive data
        //                        memset(&buf[ret], '\0', 1);
//                    printf("client[%d] send:%d\n", i, *(int*)buf);
                	if(ret!=block_size){
                		printf("data losted! only %d bytes!\n",ret);
                	}
                	received_bytes+=block_size;
                	char c='c';
                	block_count++;
//                	send(fd_A[i],&c,1,0);
                }
                break;
            }
        }

        // check whether a new connection comes
        if (FD_ISSET(sock_fd, &fdsr)) {
            new_fd = accept(sock_fd, (struct sockaddr *)&client_addr, &sin_size);
            if (new_fd <= 0) {
                perror("accept");
                continue;
            }

            // add to fd queue
            if (conn_amount < BACKLOG) {
                fd_A[conn_amount++] = new_fd;
                printf("new connection client[%d] %s:%d\n", conn_amount,
                inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                if (new_fd > maxsock)
                    maxsock = new_fd;
            }

            else {
                printf("max connections arrive, exit\n");
                send(new_fd, "bye", 4, 0);
                close(new_fd);
                printf("in %s:%d", __FILE__, __LINE__);printf("-----for debug: fd %d is closed\n", new_fd);
                break;
            }
        }
//        if(selected%40==0){
//        	for(unsigned i=0;i<BACKLOG;i++){
//        		for(unsigned j=0;j<message_count[i];j++){
//        			ret = recv(fd_A[i], buf, 2048, 0);
//					if (ret <= 0) {        // client close
//						printf("client[%d] close\n", i);
//						close(fd_A[i]);
//						FD_CLR(fd_A[i], &fdsr);
//						fd_A[i] = 0;
//					} else {        // receive data
//			//                        memset(&buf[ret], '\0', 1);
//						printf("client[%d] send:%d\n", i, *(int*)buf);
//					}
//        		}
//        		message_count[i]=0;
//        	}
//        }
       //showclient();

    }



    // close other connections

    for (i = 0; i < BACKLOG; i++) {

        if (fd_A[i] != 0) {

            close(fd_A[i]);

        }

    }



    exit(0);

}
