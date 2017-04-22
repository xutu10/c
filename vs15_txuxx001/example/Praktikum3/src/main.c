#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../include/vfilesystem_api.h"
#include "../include/vfilesystem_server_marshaller.h"
#include "../include/vfilesystem_server_messages.h"
#include <stdio.h>
#include <string.h>

// get sockaddr, IPv4 or IPv6:
void* get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void perform_request(FileServerMessage* message, int fd) {
	switch(message->payload_type)
	{
		case NEW_FILE_REQUEST: {
			//convert payload back to request struct
			NewFileRequest* request = (NewFileRequest*) message->payload;
			//perform query operations (handle defined as signed integer to catch error return values)
			int32_t handle;
			if(request->name_size > 0)
				handle = fs_new_file((char *) request->name, request->parent);
			else
				handle = -5;
			uint32_t answer_length;
			uint8_t* answer;
			FileServerMessage* fsm;
			if(handle >= 0) {
				//create respone structure
				NewFileResponse* response_struct = (NewFileResponse*) malloc(sizeof(NewFileResponse));
				response_struct->handle = handle;
				//message length + payload type + handle
				answer_length = 4 + 1 + 4;
				answer = malloc(answer_length);
				//create reply object
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = NEW_FILE_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;
			}
			else {
				//create error response structure
				ErrorResponse* response_struct = malloc(sizeof(ErrorResponse));
				response_struct->error_code = handle;
				char* msg;
				if(handle == -2)
					msg = "A file with that name already exists";
				else if (handle == -2)
					msg = "File length cannot be zero";
				else
					msg = "Couldn't create file";
				response_struct->msg_length = strlen(msg);
				response_struct->msg = malloc(strlen(msg));
				strncpy((char *)response_struct->msg, msg, strlen(msg));
				//message length + payload type +error_code + msg_length (variable) + msg (bytes therein)
				answer_length = 4 +1 + 1 + 4 + response_struct->msg_length;
				answer = malloc(answer_length);
				//create reply object
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = ERROR_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;
			}
			//marshall the struct and send it
			marshall(answer, answer_length, fsm);
			if(send(fd, answer, answer_length, 0) == answer_length)
				printf("Sent all data successfully\n");
			free(answer);
			free_message(fsm);
			break;
		}
		case NEW_FOLDER_REQUEST: {
			NewFolderRequest* request = (NewFolderRequest*) message->payload;

			int32_t handle;
			if(request->name_size > 0)
				handle = fs_new_folder((char*) request->name, request->parent);
			else
				handle = -5;
			uint32_t answer_length;
			uint8_t* answer;
			FileServerMessage* fsm;
			if(handle >= 0){
				NewFolderResponse* response_struct = (NewFolderResponse*) malloc(sizeof(NewFolderResponse));
				response_struct->handle = handle;
				//message length + payload type + handle
				answer_length = 4 + 1 + 4;
				answer = (uint8_t*) malloc(answer_length);
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = NEW_FOLDER_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;
			}
			else {
				//create error response structure
				ErrorResponse* response_struct = malloc(sizeof(ErrorResponse));
				response_struct->error_code = handle;
				char* msg;
				if(handle == -2)
					msg = "A folder with that name already exists";
				else if(handle == -2)
					msg = "Folder length cannot be zero";
				else
					msg= "Couldn't create folder";
				response_struct->msg_length = strlen(msg);
				response_struct->msg = malloc(strlen(msg));
				strncpy((char *)response_struct->msg, msg, strlen(msg));
				//message length + payload type +error_code + msg_length (variable) + msg (bytes therein)
				answer_length = 4 +1 + 1 + 4 + response_struct->msg_length;
				answer = malloc(answer_length);
				//create reply object
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = ERROR_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;
			}
			marshall(answer, answer_length, fsm);
			if(send(fd, answer, answer_length, 0) == answer_length)
				printf("Sent all data successfully\n");
			free(answer);
			free_message(fsm);
			break;
		}
		case DELETE_FILE_REQUEST: {
			DeleteFileRequest* request = (DeleteFileRequest*) message->payload;
			int32_t result = fs_delete_file(request->handle);
			uint32_t answer_length;
			uint8_t* answer;
			FileServerMessage* fsm;
			if(result >= 0) {
				//message length + payload type
				answer_length = 4 + 1;
				answer = malloc((answer_length));
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = DELETE_FILE_RESPONSE;
				fsm->payload = NULL;
			}
			else {
				//create error response structure
				ErrorResponse* response_struct = malloc(sizeof(ErrorResponse));
				response_struct->error_code = result;
				char msg[] = "Couldn't create folder";
				response_struct->msg_length = strlen(msg);
				response_struct->msg = malloc(strlen(msg));
				strncpy((char *)response_struct->msg, msg, strlen(msg));
				//message length + payload type +error_code + msg_length (variable) + msg (bytes therein)
				answer_length = 4 +1 + 1 + 4 + response_struct->msg_length;
				answer = malloc(answer_length);
				//create reply object
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = ERROR_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;
			}
			marshall(answer, answer_length, fsm);
			if(send(fd, answer, answer_length, 0) == answer_length)
				printf("Sent all data successfully\n");
			free(answer);
			free_message(fsm);
			break;
		}
		case DELETE_FOLDER_REQUEST: {
			DeleteFolderRequest* request = (DeleteFolderRequest*) message->payload;
			int32_t result = fs_delete_folder(request->handle);
			uint32_t answer_length;
			uint8_t* answer;
			FileServerMessage* fsm;
			if(result >= 0) {
				//message length + payload type
				answer_length = 4 + 1;
				answer = malloc(answer_length);
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = DELETE_FOLDER_RESPONSE;
				fsm->payload = NULL;
			}
			else {
				//create error response structure
				ErrorResponse* response_struct = malloc(sizeof(ErrorResponse));
				response_struct->error_code = result;
				char msg[] = "Couldn't create folder";
				response_struct->msg_length = strlen(msg);
				response_struct->msg = malloc(strlen(msg));
				strncpy((char *)response_struct->msg, msg, strlen(msg));
				//message length + payload type +error_code + msg_length (variable) + msg (bytes therein)
				answer_length = 4 +1 + 1 + 4 + response_struct->msg_length;
				answer = malloc(answer_length);
				//create reply object
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = ERROR_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;				
			}
			marshall(answer, answer_length, fsm);
			if(send(fd, answer, answer_length, 0) == answer_length)
				printf("Sent all data successfully\n");
			free(answer);
			free_message(fsm);
			break;
		}
		case FILE_INFO_REQUEST: {
			FileInfoRequest* request = (FileInfoRequest*) message->payload;
			printf ("request has a handle number %d\n", request->handle);
			int32_t parent = fs_get_file_parent(request->handle);
			int32_t size = fs_get_file_size(request->handle);
			int32_t name_length = fs_get_file_name_length(request->handle);
			uint8_t* name = malloc(name_length);
			int32_t nameResult = fs_get_file_name(request->handle, (char*) name, name_length);
			uint32_t answer_length;
			uint8_t* answer;
			FileServerMessage* fsm;
			if(parent >= 0 && size >= 0 && name_length >= 0 && nameResult == 0) {
				FileInfoResponse* response_struct = malloc(sizeof(FileInfoResponse));
				response_struct->parent = parent;
				response_struct->size = size;
				response_struct->name_length = (uint8_t) name_length;
				response_struct->name = name;
				//message length + payload type + parent + size + name_length (variable)
				//+ name length (all bytes in name)
				answer_length = 4 + 1 + 4 + 4 + 1 + name_length;
				answer = malloc(answer_length);
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = FILE_INFO_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;
			}
			else {
				ErrorResponse* response_struct = malloc(sizeof(ErrorResponse));
				response_struct->error_code = -1;
				char msg[] = "Couldn't get file information";
				response_struct->msg_length = strlen(msg);
				response_struct->msg = malloc(strlen(msg));
				strncpy((char *)response_struct->msg, msg, strlen(msg));
				//message length + payload type +error_code + msg_length (variable) + msg (bytes therein)
				answer_length = 4 +1 + 1 + 4 + response_struct->msg_length;
				answer = malloc(answer_length);
				//create reply object
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = ERROR_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;
			}
			marshall(answer, answer_length, fsm);
			if(send(fd, answer, answer_length, 0) == answer_length)
				printf("Sent all data successfully\n");
			free(answer);
			free_message(fsm);
			break;
		}
		case FOLDER_INFO_REQUEST: {
			//convert payload back to request struct
			FolderInfoRequest* request = (FolderInfoRequest*) message->payload;
			//perform query operations
			int32_t parent = fs_get_folder_parent(request->handle);
			int32_t name_length = fs_get_folder_name_length(request->handle);
			uint8_t* name = malloc(name_length);
			int32_t nameResult = fs_get_folder_name(request->handle, (char*) name, name_length);
			int32_t file_count = fs_get_folder_file_count(request->handle);
			uint32_t* files = malloc(file_count * sizeof(uint32_t));
			int32_t filesResult = fs_get_folder_files(request->handle, (FileID*)files, file_count);
			int32_t folder_count = fs_get_folder_folder_count(request->handle);
			uint32_t* folders = malloc(folder_count * sizeof(uint32_t));
			int32_t foldersResult = fs_get_folder_folders(request->handle, (FolderID*) folders, folder_count);
			uint32_t answer_length;
			uint8_t* answer;
			FileServerMessage* fsm;
			
			if(parent >= 0 && name_length > 0 && nameResult == 0 && file_count >= 0 && filesResult == 0
			   && folder_count >= 0 && foldersResult == 0) {
				//create response structure
				FolderInfoResponse* response_struct = malloc(sizeof(FolderInfoResponse));
				response_struct->parent = parent;
				response_struct->name_length = (uint8_t) name_length;
				response_struct->name = name;
				response_struct->file_count = file_count;
				response_struct->files = files;	
				response_struct->folder_count = folder_count;
				response_struct->folders = folders;
				//message length + payload type + parent + name_length (variable) + name length (all bytes in name)
				//+ file_count (variable) + file count (all files) + folder_count (variable) + folder count
				answer_length = 4 + 1 + 4 + 1 + name_length + 4 + file_count*4 + 4 + folder_count*4;
				answer = malloc(answer_length);
				//create reply message object
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = FOLDER_INFO_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;
				//marshall the struct and send it
			}
			else {
				ErrorResponse* response_struct = malloc(sizeof(ErrorResponse));
				response_struct->error_code = -1;
				char msg[] = "Couldn't get folder information";
				response_struct->msg_length = strlen(msg);
				response_struct->msg = malloc(strlen(msg));
				strncpy((char *)response_struct->msg, msg, strlen(msg));
				//message length + payload type +error_code + msg_length (variable) + msg (bytes therein)
				answer_length = 4 +1 + 1 + 4 + response_struct->msg_length;
				answer = malloc(answer_length);
				//create reply object
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = ERROR_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;								
			}
			marshall(answer, answer_length, fsm);
			if(send(fd, answer, answer_length, 0) == answer_length)
				printf("Sent all data successfully\n");
			free(answer);
			free_message(fsm);
			break;
		}
		case WRITE_FILE_REQUEST: {
			WriteFileRequest* request = (WriteFileRequest*) message->payload;
			int32_t result = fs_write_file(request->handle, request->offset, request->length, request->data);
			uint32_t answer_length;
			uint8_t* answer;
			FileServerMessage* fsm;
			if(result == 0) {
				//message length + payload type
				answer_length = 4 + 1;
				 answer = malloc(answer_length);
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = WRITE_FILE_RESPONSE;
				fsm->payload = NULL;
			}
			else {
				ErrorResponse* response_struct = malloc(sizeof(ErrorResponse));
				response_struct->error_code = -1;
				char msg[] = "Couldn't write to file";
				response_struct->msg_length = strlen(msg);
				response_struct->msg = malloc(strlen(msg));
				strncpy((char *)response_struct->msg, msg, strlen(msg));
				//message length + payload type +error_code + msg_length (variable) + msg (bytes therein)
				answer_length = 4 +1 + 1 + 4 + response_struct->msg_length;
				answer = malloc(answer_length);
				//create reply object
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = ERROR_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;				
			}
			marshall(answer, answer_length, fsm);
			if(send(fd, answer, answer_length, 0) == answer_length)
				printf("Sent all data successfully\n");
			free(answer);
			free_message(fsm);
			break;
		}
		case READ_FILE_REQUEST: {
			ReadFileRequest* request = (ReadFileRequest*) message->payload;
			uint8_t* data = malloc(request->length);
			int32_t result = fs_read_file(request->handle, request->offset, request->length, data);
			int i;
			printf ("length = %d\n", request->length);
			for (i = 0; i < request->length; i++) {
				printf("%c", data[i]);
			}
			printf ("\n");
			uint32_t answer_length;
			uint8_t* answer;
			FileServerMessage* fsm;
			if(result == 0) {
				ReadFileResponse* response_struct = malloc(sizeof(ReadFileResponse));
				response_struct->size = request->length;
				response_struct->data = data;
				//message length + payload type + size + data
				answer_length = 4 + 1 + 4 + response_struct->size;
				answer = malloc(answer_length);
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = READ_FILE_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;
			}
			else {
				ErrorResponse* response_struct = malloc(sizeof(ErrorResponse));
				response_struct->error_code = -1;
				char msg[] = "Couldn' read file";
				response_struct->msg_length = strlen(msg);
				response_struct->msg = malloc(strlen(msg));
				strncpy((char *)response_struct->msg, msg, strlen(msg));
				//message length + payload type +error_code + msg_length (variable) + msg (bytes therein)
				answer_length = 4 +1 + 1 + 4 + response_struct->msg_length;
				answer = malloc(answer_length);
				//create reply object
				fsm = malloc(sizeof(FileServerMessage));
				fsm->payload_type = ERROR_RESPONSE;
				fsm->payload = (uint8_t*) response_struct;				
			}
			marshall(answer, answer_length, fsm);
			if(send(fd, answer, answer_length, 0) == answer_length)
				printf("Sent all data successfully\n");
			free(answer);
			free_message(fsm);
			break;
		}
	}
}

int main(int argc, char **args) {
	if (argc != 2 || (strncmp(args[1], "-h", 2) == 0) || (strncmp(args[1], "--help", 6) == 0)) {
		printf("Benutzung: %s <port>\n", args[0]);
		exit(-1);
	}
	printf("Starting VDiskServer\n");
	fs_init();

	int status;
	struct addrinfo hints;
	struct addrinfo* result, *rp;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	status = getaddrinfo(NULL, args[1], &hints, &result);
	if(status != 0) {
		fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
		return -1;
	}
	int socketfd;
	//walk through the list until an address that can be used is found
	for(rp = result; rp != NULL; rp = rp->ai_next) {
		socketfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		//on error try the next address
		if(socketfd == -1)
			continue;
		//success
		if(bind(socketfd, rp->ai_addr, rp->ai_addrlen) == 0)
			break;
		//close this one and try the next address
		close(socketfd);
	}
	//don't need this struct anymore
	freeaddrinfo(result);
	if(rp == NULL) {
		fprintf(stderr, "Couldn't bind socket to any address\n");
		return -1;
	}
	if(listen(socketfd, 5) == -1) {
		fprintf(stderr, "listen error");
		return -1;
	}
	printf("Server waiting for connections..\n");
	struct sockaddr_storage client_info;
	socklen_t client_size = sizeof(client_info);
	char s[INET6_ADDRSTRLEN];
	//start accepting connections
	while(1) {
		//clientfd will be the socket representing the connection to the client
		int clientfd = accept(socketfd, (struct sockaddr*) &client_info, &client_size);
		if(clientfd == -1) {
			fprintf(stderr, "Accept error\n");
			continue;
		}
		inet_ntop(client_info.ss_family, get_in_addr((struct sockaddr*) &client_info), s, sizeof(s));
		printf("Started connection with %s\n", s);
		//begin by reading the size of the whole message (stored in the first 4 bytes)
		uint32_t message_length;
		ssize_t read_size;
		while((read_size = recv(clientfd, &message_length, sizeof(uint32_t), MSG_WAITALL)) == 4) {
			message_length = ntohl(message_length);
			printf ("Got a message of length %d\n", message_length);
			FileServerMessage* message;
			uint8_t* data = malloc(message_length);
			if(recv(clientfd, data, message_length, 0) < message_length) {
				fprintf(stderr, "Couldn't read sufficient bytes\n");
				return -1;
			}
			unmarshall(data, message_length, &message);
			free(data);
			perform_request(message, clientfd);
			free_message(message);
		}
		fs_destroy();
		close(clientfd);
		if(read_size == 0) {
			printf("Client closed connection\n");
			return 0;
		}
		if(read_size == -1) {
			fprintf(stderr, "Error receiving message\n");
			return -1;
		}

		fprintf(stderr, "Couldn't read sufficient bytes");
		return -1;
	}
	return 0;
}
