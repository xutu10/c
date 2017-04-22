#include <vfilesystem_server_messages.h>
#include <vfilesystem_server_marshaller.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

int32_t unmarshall(uint8_t *data, uint32_t size, FileServerMessage **msg_out) {

	switch(data[1]){  /
		case 1: {	
					uint8_t parent  = data[2];
					uint8_t name_size  = data[3];
					char *name = malloc(data[3]*sizeof(uint8_t));
					int i ;
					for(i = 0; i<name_size;i++)
						name[i] = data[i+3];
					
					(*msg_out)-> payload_type = 2;
					(*msg_out)-> payload = malloc(sizeof(uint8_t));
					uint8_t *tmp = (*msg_out)-> payload;
					tmp[0] = 3;
					tmp[1] = 4;
					tmp[2] = fs_new_file(name,parent);
					free(name);
					return 3;
				}
		case 3: {	
					uint8_t parent  = data[2];
					uint8_t name_size  = data[3];
					char *name = malloc(data[3]*sizeof(uint8_t));
					int i ;
					for(i = 0; i<name_size;i++)
						name[i] = data[i+3];

					(*msg_out)-> payload_type = 4;
					(*msg_out)-> payload = malloc(3*sizeof(uint8_t));
					uint8_t *tmp = (*msg_out)-> payload;
					tmp[0] = 3;
					tmp[1] = 4;
					tmp[2] = fs_new_folder(name,parent);
					free(name);
					return 3;
				}
	
		case 5: fs_delete_file(data[2]); return 1;
				
		case 6: fs_delete_folder(data[2]); return 1;
		case 7: {	
					uint32_t parent = fs_get_file_parent(data[1]);
					uint32_t size = fs_get_file_size(data[1]);
					uint8_t name_length = fs_get_file_name_length(data[1]);
					uint8_t *name = malloc(name_length* sizeof(uint8_t));
					fs_get_file_name(data[1],name,name_length);
					uint8_t tmpLen = 5 + name_length;				// the length of to be sented message

					(*msg_out)-> payload_type = 8;
					(*msg_out)-> payload = malloc(tmpLen*sizeof(uint8_t));
					uint8_t *tmp = (*msg_out)-> payload;
					tmp[0] = tmpLen;
					tmp[1] = 8;
					tmp[2] = parent;
					tmp[3] = size;
					tmp[4] = name_length;
					int i;
					for(i = 0 ;i< name_length ;i++)
						tmp[i+5] = name[i];
					free(name);
					return tmpLen;
					
				}
		case 9: {	
					uint32_t parent = fs_get_folder_parent(data[3]);
					uint8_t name_length = fs_get_folder_name_length(data[3]);
					uint8_t *name = malloc(name_length * sizeof(uint8_t));
					uint32_t file_count = fs_get_folder_file_count(data[3]);
					uint32_t *files = malloc(file_count * sizeof(uint32_t));
					uint32_t folder_count = fs_get_folder_folder_count(data[3];
					uint32_t *folders = malloc(file_count * sizeof(uint32_t));

					uint8_t tmpLen = 6 + name_length + file_count+ folder_count;
					(*msg_out)-> payload_type = 10;
					(*msg_out)-> payload = malloc(tmpLen*sizeof(uint8_t));
					uint8_t *tmp = (*msg_out)-> payload;
					tmp[0] = tmpLen;
					tmp[1] = 10;
					tmp[2] = parent;
					tmp[3] = size;
					tmp[4] = name_length;
					int i,j=0;
					for(i = 5 ;j< name_length ;i++){
						tmp[i] = name[j];
						j++;
					}
					tmp[i] = file_count;
					j = 0;
					for(i;j < file_count;i++){
						tmp[i] = files[j];
						j++;
					}
					tmp[i] = folder_count;
					j = 0;
					for(i;j < file_count;i++){
						tmp[i] = folders[j];
						j++;
					}
					free(name);
					free(files);
					free(folders);
					return tmpLen;
				}
		case 11: 
				return 1;
		case 12: 
				return 1;
		default: printf("wrong request type \n"); 
	}

	return 0;
}

void free_message(FileServerMessage *msg) {
	free(msg->payload);
	free(msg);
}

int32_t marshall(uint8_t *data, uint32_t size, FileServerMessage *msg) {
	switch(msg -> payload_type){
		case 2: {	
					uint8_t *tmp = msg -> payload; 
					int i;
					for(i=0;i<size;i++)
						data[i] = tmp[0];
					free_message(msg); 
					return 1;
				}
		case 4: {	
					uint8_t *tmp = msg -> payload; 
					int i;
					for(i=0;i<size;i++)
						data[i] = tmp[0];
					free_message(msg); 
					return 1;
				}
		case 8:{	
					uint8_t *tmp = msg -> payload;
					int i;
					for(i=0;i<size;i++)
						data[i] = tmp[0];
					free_message(msg); 
					return 1;
				}
		case 10: {	
					uint8_t *tmp = msg -> payload;
					int i;
					for(i=0;i<size;i++)
						data[i] = tmp[0];
					free_message(msg); 
					return 1;
				}
		case 13: 
				return 1;
		case 14:
				return 1;
		case 15: 
				return 1;
		case 16: 
				return 1;
		case 17: 
				return 1;		
		default: printf("wrong response type \n"); 
	}
	return 0;
}

