#include "../include/vfilesystem_server_messages.h"
#include "../include/vfilesystem_server_marshaller.h"
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>

int32_t unmarshall(uint8_t *data, uint32_t size, FileServerMessage** msg_out) {
	*msg_out = malloc(sizeof(FileServerMessage));
	//first byte is always the payload type
	(*msg_out)->payload_type = data[0];
	printf("Received request number %d\n", data[0]);
	uint32_t i;
	printf ("With data:\n");
	for(i = 0; i < size; i++) {
		printf ("%d ", data[i]);
	}
	printf ("\n");
	uint32_t currentPos = 0; //current position in the data array
	switch(data[currentPos++])
	{
		case NEW_FILE_REQUEST:	{
			NewFileRequest* request = malloc(sizeof(NewFileRequest));
			uint32_t parent;
			memcpy(&parent, data + currentPos, sizeof(uint32_t));
			request->parent = ntohl(parent);
			currentPos += sizeof(uint32_t);
			request->name_size = data[currentPos++];
			request->name = malloc(request->name_size + 1);
			memcpy(request->name, data + currentPos, request->name_size);
			request->name[request->name_size] = '\0';
			(*msg_out)->payload = (uint8_t*) request;
			break;
		}
		case NEW_FOLDER_REQUEST: {
			NewFolderRequest* request = malloc(sizeof(NewFolderRequest));
			uint32_t parent;
			memcpy(&parent, data + currentPos, sizeof(uint32_t));
			request->parent = ntohl(parent);
			currentPos += sizeof(uint32_t);
			request->name_size = data[currentPos++];
			request->name = malloc(request->name_size + 1);
			memcpy(request->name, data + currentPos, request->name_size);
			request->name[request->name_size] = '\0';			
			(*msg_out)->payload = (uint8_t*) request;
			break;
		}
		case DELETE_FILE_REQUEST: {
			DeleteFileRequest* request = malloc(sizeof(DeleteFileRequest));
			uint32_t handle;
			memcpy(&handle, data + currentPos, sizeof(handle));
			request->handle = ntohl(handle);
			(*msg_out)->payload = (uint8_t*) request;
			break;
		}
		case DELETE_FOLDER_REQUEST: {
			DeleteFolderRequest* request = malloc(sizeof(DeleteFolderRequest));
			uint32_t handle;
			memcpy(&handle, data + currentPos, sizeof(handle));
			request->handle = ntohl(handle);
			(*msg_out)->payload = (uint8_t*) request;
			break;
		}
		case FILE_INFO_REQUEST: {
			FileInfoRequest* request = malloc(sizeof(FileInfoRequest));
			uint32_t handle;
			memcpy(&handle, data + currentPos, sizeof(handle));
			request->handle = ntohl(handle);
			(*msg_out)->payload = (uint8_t*) request;
			break;
		}
		case FOLDER_INFO_REQUEST: {
			FolderInfoRequest* request = malloc(sizeof(FolderInfoRequest));
			uint32_t handle;
			memcpy(&handle, data + currentPos, sizeof(handle));
			request->handle = ntohl(handle);
			(*msg_out)->payload = (uint8_t*) request;
			break;
		}
		case WRITE_FILE_REQUEST: {
			WriteFileRequest* request = malloc(sizeof(WriteFileRequest));
			uint32_t handle;
			memcpy(&handle, data + currentPos, sizeof(handle));
			request->handle = ntohl(handle);
			currentPos += sizeof(uint32_t);
			uint32_t offset;
			memcpy(&offset, data + currentPos, sizeof(uint32_t));
			request->offset = ntohl(offset);
			currentPos += sizeof(uint32_t);
			uint32_t length;
			memcpy(&length, data + currentPos, sizeof(uint32_t));
			request->length = ntohl(length);
			currentPos += sizeof(uint32_t);
			request->data = malloc(request->length);
			memcpy(request->data, data + currentPos, request->length);
			(*msg_out)->payload = (uint8_t*) request;
			break;
		}
		case READ_FILE_REQUEST: {
			ReadFileRequest* request = malloc(sizeof(ReadFileRequest));
			uint32_t handle;
			memcpy(&handle, data + currentPos, sizeof(uint32_t));
			request->handle = ntohl(handle);
			currentPos += sizeof(uint32_t);
			uint32_t offset;
			memcpy(&offset, data + currentPos, sizeof(uint32_t));
			request->offset = ntohl(offset);
			currentPos += sizeof(uint32_t);
			uint32_t length;
			memcpy(&length, data + currentPos, sizeof(uint32_t));
			request->length = ntohl(length);
			(*msg_out)->payload = (uint8_t*) request;
		}
	}
	return 0;
}

void free_message(FileServerMessage *msg) {
	//this method assumes all pointers are not NULL
	//since this is always true in the use cases of this program
	switch(msg->payload_type) {
		case NEW_FILE_REQUEST: {
			NewFileRequest* request = (NewFileRequest*) msg->payload;
			free(request->name);
			free(request);
			break;
		}
		case NEW_FILE_RESPONSE: {
			free(msg->payload);
			break;
		}
		case NEW_FOLDER_REQUEST: {
			NewFolderRequest* request = (NewFolderRequest*) msg->payload;
			free(request->name);
			free(request);
			break;
		}
		case NEW_FOLDER_RESPONSE: {
			free(msg->payload);
			break;
		}
		case DELETE_FILE_REQUEST: {
			free(msg->payload);
			break;
		}
		case DELETE_FOLDER_REQUEST: {
			free(msg->payload);
			break;
		}
		case FILE_INFO_REQUEST: {
			free(msg->payload);
			break;
		}
		case FILE_INFO_RESPONSE: {
			FileInfoResponse* response = (FileInfoResponse*) msg->payload;
			free(response->name);
			free(response);
			break;
		}
		case FOLDER_INFO_REQUEST: {
			free(msg->payload);
			break;
		}
		case FOLDER_INFO_RESPONSE: {
			FolderInfoResponse* response = (FolderInfoResponse*) msg->payload;
			free(response->name);
			free(response->files);
			free(response->folders);
			free(response);
			break;
		}
		case WRITE_FILE_REQUEST: {
			WriteFileRequest* request = (WriteFileRequest*) msg->payload;
			free(request->data);
			free(request);
			break;
		}
		case READ_FILE_REQUEST: {
			free(msg->payload);
			break;
		}
		case READ_FILE_RESPONSE: {
			ReadFileResponse* response = (ReadFileResponse*) msg->payload;
			free(response->data);
			free(response);
			break;
		}
		case ERROR_RESPONSE: {
			ErrorResponse* response = (ErrorResponse*) msg->payload;
			free(response->msg);
			free(response);
			break;
		}
	}
	free(msg);
}

int32_t marshall(uint8_t *data, uint32_t size, FileServerMessage *msg) {
	//message_length doesn't include its own length
	uint32_t message_length = size - 4;
	uint32_t currentPos = 0; //current position in the data array
	//copy the message size in big endian
	data[currentPos++] = message_length >> 24;
	data[currentPos++] = message_length >> 16;
	data[currentPos++] = message_length >> 8;
	data[currentPos++] = message_length;
	data[currentPos++] = msg->payload_type;
	switch(msg->payload_type) {
		case NEW_FILE_RESPONSE: {
			NewFileResponse* rsp = (NewFileResponse*) msg->payload;
			//copy the handle
			rsp->handle = htonl(rsp->handle);
			memcpy(data + currentPos, &rsp->handle, sizeof(uint32_t));
			currentPos += sizeof(uint32_t);
			if(currentPos == size)
				printf ("Copied all data sucessfully\n");
			break;
		}
		case NEW_FOLDER_RESPONSE: {
			NewFolderResponse* rsp = (NewFolderResponse*) msg->payload;
			rsp->handle = htonl(rsp->handle);
			memcpy(data + currentPos, &rsp->handle, sizeof(uint32_t));
			currentPos += sizeof(uint32_t);
			if(currentPos == size)
				printf ("Copied all data sucessfully\n");
			break;
		}
		case DELETE_FILE_RESPONSE: {
			if(currentPos == size)
				printf ("Copied all data sucessfully\n");
			break;			
		}
		case DELETE_FOLDER_RESPONSE: {
			if(currentPos == size)
				printf ("Copied all data sucessfully\n");
			break;			
		}
		case FILE_INFO_RESPONSE: {
			FileInfoResponse* rsp = (FileInfoResponse*) msg->payload;
			rsp->parent = htonl(rsp->parent);
			memcpy(data + currentPos, &rsp->parent, sizeof(uint32_t));
			currentPos += sizeof(uint32_t);
			rsp->size = htonl(rsp->size);
			memcpy(data + currentPos, &rsp->size, sizeof(uint32_t));
			currentPos += sizeof(uint32_t);
			data[currentPos++] = rsp->name_length;
			memcpy(data + currentPos, rsp->name, rsp->name_length);
			currentPos += rsp->name_length;
			printf("currentpos = %d size = %d\n", currentPos, size);			
			if(currentPos == size)
				printf ("Copied all data sucessfully\n");
			break;
		}
		case FOLDER_INFO_RESPONSE: {
			FolderInfoResponse* rsp = (FolderInfoResponse*) msg->payload;
			//copy the parent
			rsp->parent = htonl(rsp->parent);
			memcpy(data + currentPos, &rsp->parent, sizeof(uint32_t));
			currentPos += sizeof(uint32_t);
			//copy the name length
			data[currentPos++] = rsp->name_length;
			//copy the acutal name
			memcpy(data + currentPos, rsp->name, rsp->name_length);
			currentPos += rsp->name_length;
			//copy the file count
			uint32_t reversedValue = htonl(rsp->file_count);
			memcpy(data + currentPos, &reversedValue, sizeof(uint32_t));
			currentPos += sizeof(uint32_t);
			//copy the actual file array
			uint32_t i;
			for(i = 0; i < rsp->file_count; i++)
				rsp->files[i] = htonl(rsp->files[i]);
			memcpy(data + currentPos, rsp->files, rsp->file_count * sizeof(uint32_t));
			currentPos += rsp->file_count * sizeof(uint32_t);
			//copy the folder count
			reversedValue = htonl(rsp->folder_count);
			memcpy(data + currentPos, &reversedValue, sizeof(uint32_t));
			currentPos += sizeof(uint32_t);
			//copy the actual folder array
			for(i = 0; i < rsp->folder_count; i++)
				rsp->folders[i] = htonl(rsp->folders[i]);
			memcpy(data + currentPos, rsp->folders, rsp->folder_count * sizeof(uint32_t));
			currentPos += rsp->folder_count * sizeof(uint32_t);
			printf("currentpos = %d size = %d\n", currentPos, size);
			if(currentPos == size)
				printf ("Copied all data sucessfully\n");
			break;
		}
		case WRITE_FILE_RESPONSE: {
			if(currentPos == size)
				printf ("Copied all data sucessfully\n");
			break;
		}
		case READ_FILE_RESPONSE: {
			ReadFileResponse* rsp = (ReadFileResponse*) msg->payload;
			uint32_t reversedValue = htonl(rsp->size);
			memcpy(data + currentPos, &reversedValue, sizeof(uint32_t));
			currentPos += sizeof(uint32_t);
			memcpy(data + currentPos, rsp->data, rsp->size);
			currentPos += rsp->size;
			if(currentPos == size)
				printf ("Copied all data sucessfully\n");
			break;			
		}
		case ERROR_RESPONSE: {
			ErrorResponse* rsp = (ErrorResponse*) msg->payload;
			data[currentPos++] = rsp->error_code;
			uint32_t reversedValue = htonl(rsp->msg_length);
			memcpy(data + currentPos, &reversedValue, sizeof(uint32_t));
			currentPos += sizeof(uint32_t);
			memcpy(data + currentPos, rsp->msg, rsp->msg_length);
			currentPos += rsp->msg_length;
			if(currentPos == size)
				printf ("Copied all data sucessfully\n");
			break;
		}
	}
	return 0;
}
