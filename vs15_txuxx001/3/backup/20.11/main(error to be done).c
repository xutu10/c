#include <vfilesystem_api.h>
#include <vfilesystem_server_messages.h>
#include <vfilesystem_server_marshaller.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>


#define BACKLOG 10

uint32_t request_handle(FileServerMessage* msg, FileServerMessage *rMsg){
switch(msg-> payload_type){
			case NEW_FILE_REQUEST: {							
					NewFileRequest *tmp = (NewFileRequest*)msg-> payload;
					rMsg-> payload_type = NEW_FILE_RESPONSE;
					NewFileResponse* tmpRsp = malloc(sizeof(NewFileResponse));
					tmpRsp-> handle = fs_new_file(tmp-> name,tmp-> parent);
					rMsg-> payload = (uint8_t*)tmpRsp;
					free(tmp-> name);
					return 9;								// x(this case handle - 4 bytes) + type(1 byte)+ length of message(4)
				}											// return the length for the whole message, which to be sented to client
			case NEW_FOLDER_REQUEST: {	
					NewFolderRequest* tmp = (NewFolderRequest*)msg-> payload;
					rMsg-> payload = NEW_FOLDER_RESPONSE;

					NewFolderResponse* tmpRsp = malloc(sizeof(NewFolderResponse));
					tmpRsp-> handle = fs_new_folder(msg-> name, msg-> parent);
					rMsg-> payload = (uint8_t*)tmpRsp;
					free(tmp-> name);
					return 9;
				}
	
			case DELETE_FILE_REQUEST: {
				DeleteFileRequest* tmp = (DeleteFileRequest*)msg-> payload;
				fs_delete_file(tmp-> handle); 
				return 1;
			}
				
			case DELETE_FOLDER_REQUEST: {
				DeleteFolderRequest* tmp =(DeleteFolderRequest*)msg-> payload;
				fs_delete_folder(tmp-> handle);
				return 1;
			}
			case FILE_INFO_REQUEST: {	
					FileInfoRequest* tmp = (FileInfoRequest*)msg-> payload;
					rMsg-> payload_type = FILE_INFO_RESPONSE;

					FileInfoResponse* tmpRsp = malloc(sizeof(FileInfoResponse));
					tmpRsp-> payload_type = FILE_INFO_RESPONSE
					tmpRsp-> parent = fs_get_file_parent(tmp-> handle);
					tmpRsp-> size = fs_get_file_size(tmp-> handle);
					tmpRsp-> name_length = fs_get_file_name_length(tmp-> handle);
					char *name = malloc(name_length);
					fs_get_file_name(tmp-> handle, name, name_length);
					tmpRsp-> name =(uint8_t*)name;

					rMsg-> payload = (uint8_t*)tmpRsp;
					uint32_t rsp_msg_length = 5+sizeof(tmpRsp-> parent)+ sizeof(tmpRsp-> size)+ sizeof(tmpRsp->name_length)+name_length;
					return rsp_msg_length;
					
				}
			case FOLDER_INFO_REQUEST: {	
					FolderInfoRequest* tmp = (FolderInfoRequest*)msg-> payload;
					FolderInfoResponse* tmpRsp = malloc(sizeof(FolderInfoResponse));

					tmpRsp-> parent = fs_get_folder_parent(tmp-> handle);
					tmpRsp-> name_length = fs_get_folder_name_length(tmp-> handle);
					char *name = malloc(name_length);
					fs_get_folder_name(tmp-> handle, name, name_length);
					tmpRsp-> name = (uint8_t*)name;
					tmpRsp-> file_count = fs_get_folder_file_count(tmp-> handle);
					tmpRsp-> files = malloc(file_count*sizeof(uint32_t));
					fs_get_folder_files(tmp-> handle, tmpRsp-> files, file_count)
					tmpRsp-> folder_count = fs_get_folder_folder_count(tmp->handle);
					tmpRsp-> folders = malloc(sizeof(uint32_t));
					fs_get_folder_folders(tmp-> handle, tmpRsp-> folders, tmpRsp-> folder_count);

					rMsg-> payload = (uint8_t*)tmpRsp;
					uint32_t rsp_msg_length = 5+sizeof(tmpRsp-> parent)+sizeof(tmpRsp-> name_length)+name_length
												+sizeof(tmpRsp->file_count)+4*tmpRsp-> file_count+sizeof(tmpRsp-> folder_count)
												+4*tmpRsp->folder_count;
					
					return rsp_msg_length;
				}
			case WRITE_FILE_REQUEST: {
				WriteFileRequest* tmp = (WriteFileRequest*)msg-> payload;
				FolderInfoResponse* tmpRsp = malloc(sizeof(FolderInfoResponse));

				fs_write_file(tmp-> handle, tmp-> offset, tmp-> length, tmp-> data);

				free(tmp-> data);
				return 1;
			}
			case READ_FILE_REQUEST: {
					ReadFileRequest* tmp = (ReadFileRequest*)msg-> payload;
					rMsg-> payload_type = READ_FILE_RESPONSE;

					ReadFileResponse* tmpRsp = malloc(sizeof(ReadFileResponse));
					tmpRsp-> size = fs_get_file_size(tmp-> handle);
					tmpRsp-> data = malloc(tmpRsp-> size);

					fs_read_file(tmp-> handle, tmp-> offset , tmpRsp-> size, tmpRsp-> data);
					rMsg-> payload = (uint8_t*)tmpRsp;

					uint32_t rsp_msg_length = 5+sizeof(tmpRsp-> size)+size;

				return rsp_msg_length;
			}
			default: printf("wrong request type \n"); 
	}
}

	if (sa->sa_family == AF_INET) 
		return &(((struct sockaddr_in*)sa)->sin_addr);
		
		return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char **args) {
	if (argc != 2 || (strncmp(args[1], "-h", 2) == 0) || (strncmp(args[1], "--help", 6) == 0)) {
		printf("Benutzung: %s <port>\n", args[0]);
		exit(-1);
	}

	int port = atoi(args[1]);
	printf("Starting VDiskServer\n");
	fs_init();

	int sockfd, new_fd;
	struct addrinfo hints;
	struct addrinfo *servinfo, *p;
	struct sockaddr_storage their_addr; 
	socklen_t sin_size;  												
												
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	rv = getaddrinfo(NULL,args[1],&hints,&servinfo);
	if(0 != rv){
		printf("getaddrinfo error\n");
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
		sin_size = sizeof(their_addr);
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
			
			uint32_t *msgLen = 0;
			if(-1 == recv(new_fd,msgLen,4,0 )){ // tips, length, tip2??? void *
				send(new_fd,"failed to receive",17,0);
				close(new_fd);
				exit(0);
			}

			uint8_t buffer[ntohl(msgLen)] ; // must malloc???
			if(-1 == recv(new_fd,buffer,msgLen,0 )){ // length???
				send(new_fd,"failed to receive",17,0);
				close(new_fd);
				exit(0);
			}

			FileServerMessage* msg = NULL;

			if(!unmarshall(buffer,msgLen,&msg)){
				send(new_fd,"failed to handle",16,0);
				close(new_fd);
				exit(0);
			}

			FileServerMessage* response_msg = malloc(sizeof(FileServerMessage));
			uint32_t response_buffer_length = 0;
			
			if(0 == (response_buffer_length =  request_handle(msg, response_msg)));{			// call the filesystem
				send(new_fd,"failed to handle",16,0);
				close(new_fd);
				exit(0);				
			}
			free_message(msg);
			if( 1 != response_buffer_length){											// no response for delete request
				uint8_t *response_buffer = malloc(response_buffer_length);		

				if(! marshall(response_buffer, response_buffer_length-4, response_msg)){		// except the length note 4 bytes
					send(new_fd,"failed to handle",16,0);
					close(new_fd);
					exit(0);
				}
				free_message(response_msg);

				send(new_fd,response_buffer,htonl(bufferLen),0);
				close(new_fd);
				exit(0);
			}
		}
		close(new_fd);
	}

	return 0;
}

