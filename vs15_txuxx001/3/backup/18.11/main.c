#include <vfilesystem_api.h>
#include <vfilesystem_server_messages.h>
#include <vfilesystem_server_marshaller.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BACKLOG 10

int main(int argc, char **args) {
	if (argc != 2 || (strncmp(args[1], "-h", 2) == 0) || (strncmp(args[1], "--help", 6) == 0)) {
		printf("Benutzung: %s <port>\n", args[0]);
		exit(-1);
	}

	int port = atoi(args[1]);

	printf("Starting VDiskServer\n");

	fs_init();
	int sockfd, new_fd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_add;
	socklen_t sin_size;
	int rv;
	memset(&hints,0,sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flag = NULL;
	if(0 != (rv = getaddrinfo(NULL,port,&hints,&servinfo))){
		printf("getaddinfo error\n");
		return 1;
	}

	for(p = servinfo; NULL != p; p = p-> ai_next){
		if(-1 == (sockfd = socket(p-> ai_family,p-> ai_socktype,p-> ai_protocol)))
			continue;

		if(-1 == bind(sockfd,p-> ai_addr,p-> ai_addrlen)){
			close(sockfd);
			continue;
		}
		break;
	}

	if(NULL == p){
		printf("failed to bind\n");
		return 2;
	}
	freeaddrinfo(servinfo);
	if(-1 == listen(sockfd, BACKLOG)){
		printf("failed to listen\n");
		exit(-1);
	}
	printf("server: waiting for connections...\n");
	
	while(1){
		sin_size = sizeof(their_add);
		new_fd = accept(sockfd,(struct sockaddr*)&their_add,&sin_size);
		
		if(-1 == new_fd){
			printf("failed to accept\n");
			continue;
		}

		if(!fork()){
			close(sockfd); //???
			if(-1 == send(new_fd,"server get ready",16,0)){
				printf("failed to send\n");
				close(new_fd);
				exit(0);
			}
			
			uint8_t data[255];
			if(-1 == revc(new_fd,data,255,0 )){ // length???
				send(new_fd,"failed to receive",17,0);
				close(new_fd);
				exit(0);
			}

			uint8_t *buffer; 
			int bufferLen = 0;
			FileServerMessage *msg = malloc(sizeof(FileServerMessage));

			int tmp = data[0];
			bufferLen = ntohl(tmp);
			buffer = malloc(bufferLen*sizeof(uint8_t));    // the length of message from client

			int i;
			for(i=0;i<bufferLen;i++)
				buffer[i] = data[i] ;

			if(!(bufferLen = unmarshall(data,bufferLen,&msg))){
				send(new_fd,"failed to handle",16,0);
				close(new_fd);
				exit(0);
			}

			free(buffer);
			buffer = malloc(bufferLen*sizeof(uint8_t));	// the length of message to be sented to client from server

			if(! marshall(buffer, bufferLen, msg)){
				send(new_fd,"failed to handle",16,0);
				close(new_fd);
				exit(0);
			}

			send(new_fd,buffer,htonl(bufferLen),0);
			close(new_fd);
			exit(0);
		}
		close(new_fd);
	}

	return 0;
}

