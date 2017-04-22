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
					uint32_t rsp_msg_length = 0;					
					uint32_t rsltFromFilesytem = 0;
					char* name = (char*)tmp-> name;
					if((rsltFromFilesytem = fs_new_file(name,tmp-> parent))){
						NewFileResponse* tmpRsp = malloc(sizeof(NewFileResponse));						
						tmpRsp-> handle = rsltFromFilesytem;

						rMsg-> payload_type = NEW_FILE_RESPONSE;
						rMsg-> payload = (uint8_t*)tmpRsp;			// x(this case handle - 4 bytes) + type(1 byte)+ length of message(4)
						
						rsp_msg_length = 9;						// return the length for the whole message, which to be sented to client
					}	
					else{
						ErrorResponse* tmpE = malloc(sizeof(ErrorResponse));
						tmpE-> error_code = 19;						// requestcode + 9 is error_code
						tmpE-> msg_length = 23;
						char* msg = malloc(tmpE-> msg_length);

						sprintf(msg,"new file request failed" );
						tmpE->msg = (uint8_t*)msg;

						rMsg-> payload_type = ERROR_RESPONSE;						
						rMsg-> payload = (uint8_t*)tmpE;

						rsp_msg_length = 5 +sizeof(tmpE-> error_code) + sizeof(tmpE-> msg_length) + tmpE-> msg_length;
														// length of the whole message(4 bytes)+ type(1 byte) + x 
					}	
					free(tmp-> name);	
					return 	rsp_msg_length;				
				}											
			case NEW_FOLDER_REQUEST: {	
					NewFolderRequest* tmp = (NewFolderRequest*)msg-> payload;
					uint32_t rsp_msg_length = 0;					
					uint32_t rsltFromFilesytem = 0;					
					char* name = (char*)tmp-> name;
					
					if((rsltFromFilesytem = fs_new_folder(name, tmp-> parent))){
						NewFolderResponse* tmpRsp = malloc(sizeof(NewFolderResponse));
						tmpRsp-> handle = rsltFromFilesytem;

						rMsg-> payload_type = NEW_FOLDER_RESPONSE;
						rMsg-> payload = (uint8_t*)tmpRsp;
						
						rsp_msg_length = 9;							
					}
					else{
						ErrorResponse* tmpE = malloc(sizeof(ErrorResponse));
						tmpE-> error_code = 39;						
						tmpE-> msg_length = 25;
						char* msg = malloc(tmpE-> msg_length);
						
						sprintf(msg,"new folder request failed" );
						tmpE->msg = (uint8_t*)msg;

						rMsg-> payload_type = ERROR_RESPONSE;						
						rMsg-> payload = (uint8_t*)tmpE;

						rsp_msg_length = 5 +sizeof(tmpE-> error_code) + sizeof(tmpE-> msg_length) + tmpE-> msg_length;
					}
					free(tmp-> name);
					return rsp_msg_length;
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
					uint32_t rsp_msg_length = 0;					
					uint32_t parentFromFilesytem = fs_get_file_parent(tmp-> handle);
					uint32_t sizeFromFilesytem = fs_get_file_size(tmp-> handle);
					uint32_t nameLengthFromFilesytem = fs_get_file_name_length(tmp-> handle);

					if(parentFromFilesytem && sizeFromFilesytem && parentFromFilesytem){
						FileInfoResponse* tmpRsp = malloc(sizeof(FileInfoResponse));
						tmpRsp-> parent = parentFromFilesytem;
						tmpRsp-> size = sizeFromFilesytem;
						tmpRsp-> name_length = nameLengthFromFilesytem;
						char *name = malloc(tmpRsp-> name_length);
						fs_get_file_name(tmp-> handle, name, tmpRsp-> name_length);  //??? check file name
						tmpRsp-> name =(uint8_t*)name;

						rMsg-> payload_type = FILE_INFO_RESPONSE;
						rMsg-> payload = (uint8_t*)tmpRsp;
						rsp_msg_length = 5+sizeof(tmpRsp-> parent)+ sizeof(tmpRsp-> size)+ sizeof(tmpRsp->name_length)+tmpRsp-> name_length;
					}
					else{
						ErrorResponse* tmpE = malloc(sizeof(ErrorResponse));
						tmpE-> error_code = 89;						
						tmpE-> msg_length = 24;
						char* msg = malloc(tmpE-> msg_length);
						sprintf(msg,"file info request failed" );
						tmpE->msg = (uint8_t*)msg;
						rMsg-> payload_type = ERROR_RESPONSE;						
						rMsg-> payload = (uint8_t*)tmpE;

						rsp_msg_length = 5 +sizeof(tmpE-> error_code) + sizeof(tmpE-> msg_length) + tmpE-> msg_length;
					}

					return rsp_msg_length;
					
				}
			case FOLDER_INFO_REQUEST: {	
					FolderInfoRequest* tmp = (FolderInfoRequest*)msg-> payload;
					uint32_t rsp_msg_length = 0;				
					uint32_t parent = fs_get_folder_parent(tmp-> handle);	
					uint32_t name_length = fs_get_folder_name_length(tmp-> handle);
					uint32_t file_count = fs_get_folder_file_count(tmp-> handle);
					uint32_t folder_count =fs_get_folder_folder_count(tmp->handle);

					if(parent && name_length && file_count && folder_count){
						FolderInfoResponse* tmpRsp = malloc(sizeof(FolderInfoResponse));
						tmpRsp-> parent = parent;
						tmpRsp-> name_length = name_length;
						char *name = malloc(name_length);
						fs_get_folder_name(tmp-> handle, name, name_length);
						tmpRsp-> name = (uint8_t*)name;
						tmpRsp-> file_count = file_count;
						tmpRsp-> files = malloc(file_count*sizeof(uint32_t));
						fs_get_folder_files(tmp-> handle, tmpRsp-> files, file_count);
						tmpRsp-> folder_count = folder_count;
						tmpRsp-> folders = malloc(sizeof(uint32_t));
						fs_get_folder_folders(tmp-> handle, tmpRsp-> folders, tmpRsp-> folder_count);

						rMsg-> payload_type = FOLDER_INFO_RESPONSE; 
						rMsg-> payload = (uint8_t*)tmpRsp;
						rsp_msg_length = 5+sizeof(tmpRsp-> parent)+sizeof(tmpRsp-> name_length)+name_length
												+sizeof(tmpRsp->file_count)+4*tmpRsp-> file_count+sizeof(tmpRsp-> folder_count)
												+4*tmpRsp->folder_count;					
					}
					else{
						ErrorResponse* tmpE = malloc(sizeof(ErrorResponse));
						tmpE-> error_code = 99;						
						tmpE-> msg_length = 26;
						char* msg = malloc(tmpE-> msg_length);
						sprintf(msg,"folder info request failed" );
						tmpE->msg = (uint8_t*)msg;

						rMsg-> payload_type = ERROR_RESPONSE;						
						rMsg-> payload = (uint8_t*)tmpE;

						rsp_msg_length = 5 +sizeof(tmpE-> error_code) + sizeof(tmpE-> msg_length) + tmpE-> msg_length;
					}

					return rsp_msg_length;
				}
			case WRITE_FILE_REQUEST: {
				WriteFileRequest* tmp = (WriteFileRequest*)msg-> payload;	
				uint32_t rsp_msg_length = 1;				
				uint32_t rsltFromFilesytem = fs_write_file(tmp-> handle,tmp-> offset,tmp-> length, tmp-> data);
				
				if(!rsltFromFilesytem){
					ErrorResponse* tmpE = malloc(sizeof(ErrorResponse));
					tmpE-> error_code = 169;						
					tmpE-> msg_length = 25;
					char* msg = malloc(tmpE-> msg_length);
					sprintf(msg,"write file request failed" );
					tmpE->msg = (uint8_t*)msg;

					rMsg-> payload_type = ERROR_RESPONSE;						
					rMsg-> payload = (uint8_t*)tmpE;

					rsp_msg_length = 5 +sizeof(tmpE-> error_code) + sizeof(tmpE-> msg_length) + tmpE-> msg_length;

				}
				free(tmp-> data);
				return rsp_msg_length;
			}
			case READ_FILE_REQUEST: {
					ReadFileRequest* tmp = (ReadFileRequest*)msg-> payload;
					uint32_t rsp_msg_length = 0;					
					uint32_t size = fs_get_file_size(tmp-> handle);

					if(size){
						ReadFileResponse* tmpRsp = malloc(sizeof(ReadFileResponse));
						tmpRsp-> size = size ;
						tmpRsp-> data = malloc(tmpRsp-> size);

						fs_read_file(tmp-> handle, tmp-> offset , tmpRsp-> size, tmpRsp-> data);

						rMsg-> payload_type = READ_FILE_RESPONSE;
						rMsg-> payload = (uint8_t*)tmpRsp;

						rsp_msg_length = 5+sizeof(tmpRsp-> size)+size;
				}
				else{
						ErrorResponse* tmpE = malloc(sizeof(ErrorResponse));
						tmpE-> error_code = 129;						
						tmpE-> msg_length = 24;
						char* msg = malloc(tmpE-> msg_length);
						sprintf(msg,"read file request failed" );
						tmpE->msg = (uint8_t*)msg;

						rMsg-> payload_type = ERROR_RESPONSE;						
						rMsg-> payload = (uint8_t*)tmpE;

						rsp_msg_length = 5 +sizeof(tmpE-> error_code) + sizeof(tmpE-> msg_length) + tmpE-> msg_length;
					}

				return rsp_msg_length;
			}
			default: return 0; 
	}
}

void *get_in_addr(struct sockaddr *sa){
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
		new_fd = accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);
		
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
			
			uint32_t *msg_len = 0;
			if(-1 == recv(new_fd,msg_len,4,0 )){ // tips, length, tip2??? void *
				send(new_fd,"failed to receive",17,0);
				close(new_fd);
				exit(0);
			}
			uint32_t msgLen = *msg_len;

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
			uint32_t responseBufferLength = 0;
			
			if(!(responseBufferLength =  request_handle(msg, response_msg)));{			// call the filesystem
				send(new_fd,"failed to handle",16,0);
				close(new_fd);
				exit(0);				
			}
			free_message(msg);
			if( 1 != responseBufferLength){											// 1 for delete request and wirte
				uint8_t *response_buffer = malloc(responseBufferLength);		

				if(! marshall(response_buffer, responseBufferLength-4, response_msg)){		// except the length note 4 bytes
					send(new_fd,"failed to handle",16,0);
					close(new_fd);
					exit(0);
				}
				free_message(response_msg);

				send(new_fd,response_buffer,htonl(responseBufferLength),0);
				close(new_fd);
				exit(0);
			}
		}
		close(new_fd);
	}

	return 0;
}

