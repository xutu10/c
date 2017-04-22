#include <vfilesystem_server_messages.h>
#include <vfilesystem_server_marshaller.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

int32_t unmarshall(uint8_t *data, uint32_t size, FileServerMessage** msg_out) {
	*msg_out = malloc(sizeof(FileServerMessage));
	(*msg_out)-> payload_type = data[0];
	switch(data[0]){
		case NEW_FILE_REQUEST: {
			NewFileRequest* tmp = malloc(sizeof(NewFileRequest));
			uint32_t parent = ((data[4] << 24)				// alread reverse from net to host
                   | (data[3] << 16)
                   | (data[2] << 8)
                   | data[1] ); 
				
			tmp-> parent = parent;
			tmp-> name_size = data[5];
			tmp-> name = malloc(tmp-> name_size);	
			uint8_t i;
			for(i=0;i<tmp-> name_size;i++)
				tmp-> name[0] = data[sizeof(uint32_t)+2+i];
								
			(*msg_out)-> payload = (uint8_t*)tmp;
			return 1;
		}
		case NEW_FOLDER_REQUEST:{ 
			NewFolderRequest* tmp = malloc(sizeof(NewFolderRequest));
			uint32_t parent =  ((data[4] << 24)
                    | (data[3] << 16)
                    | (data[2] << 8)
                    | data[1] ) ;
			tmp-> parent = parent;
			tmp-> name_size = data[5];
			tmp-> name = malloc(tmp-> name_size);
			uint8_t i;
			for(i=0;i<tmp-> name_size;i++)
					tmp-> name[0] = data[sizeof(uint32_t)+2+i];
									
			(*msg_out)-> payload = (uint8_t*)tmp;
			return 1;
		}
		case DELETE_FILE_REQUEST:{ 
			DeleteFileRequest* tmp = malloc(sizeof(DeleteFileRequest));
			tmp-> handle = (data[1]
				|(data[2] << 8)
				|(data[3] << 16)
				|(data[4] << 24)) ;

			(*msg_out)-> payload = (uint8_t*)tmp;
			return 1;
		}
		case DELETE_FOLDER_REQUEST:{
			DeleteFolderRequest* tmp = malloc(sizeof(DeleteFolderRequest));
			tmp-> handle = (data[1]
				|(data[2] << 8)
				|(data[3] << 16)
				|(data[4] << 24)) ;

			(*msg_out)-> payload = (uint8_t*)tmp;
			return 1;
		}
		case FILE_INFO_REQUEST:{ 
			FileInfoRequest* tmp = malloc(sizeof(FileInfoRequest));
			tmp-> handle = (data[1]
				|(data[2] << 8)
				|(data[3] << 16)
				|(data[4] << 24)) ;

			(*msg_out)-> payload = (uint8_t*)tmp;
			return 1;
		}
		case FOLDER_INFO_REQUEST:{ 
			FolderInfoRequest* tmp = malloc(sizeof(FolderInfoRequest));
			tmp-> handle = (data[1]
				|(data[2] << 8)
				|(data[3] << 16)
				|(data[4] << 24)) ;

			(*msg_out)-> payload = (uint8_t*)tmp;
			return 1;
		}
		case WRITE_FILE_REQUEST:{ 
			WriteFileRequest* tmp = malloc(sizeof(WriteFileRequest));
			tmp-> handle = (data[1]
				|(data[2] << 8)
				|(data[3] << 16)
				|(data[4] << 24)) ;
			tmp-> offset = (data[5]
				|(data[6] << 8)
				|(data[7] << 16)
				|(data[8] << 24)) ;
			tmp-> length = (data[9]
				|(data[10] << 8)
				|(data[11] << 16)
				|(data[12] << 24)) ;

			tmp-> data = malloc(tmp-> length);
			uint32_t i;
			for(i=0;i<tmp-> length;i++)
				tmp->data[i] = data[13+i];

			(*msg_out)-> payload = (uint8_t*)tmp;
			return 1;
		}
		case READ_FILE_REQUEST:{
			ReadFileRequest* tmp = malloc(sizeof(ReadFileRequest));
			tmp-> handle = (data[1]
				|(data[2] << 8)
				|(data[3] << 16)
				|(data[4] << 24)) ;
			tmp-> offset = (data[5]
				|(data[6] << 8)
				|(data[7] << 16)
				|(data[8] << 24)) ;
			tmp-> length = (data[9]
				|(data[10] << 8)
				|(data[11] << 16)
				|(data[12] << 24)) ;

			(*msg_out)-> payload = (uint8_t*)tmp;
			return 1;
			}		
	}	
	return 0;
}

void free_message(FileServerMessage *msg) {
	free(msg->payload);
	free(msg);
}

int32_t marshall(uint8_t *data, uint32_t size, FileServerMessage *r_msg) {
	data[0] = size;
	data[1] = size >> 8;
	data[2] = size >> 16;
	data[3] = size >> 24;
	data[4] = r_msg-> payload_type;
	switch(data[4]){
		case NEW_FILE_RESPONSE:{
			NewFileResponse *tmp = (NewFileResponse*)r_msg-> payload;
			data[5] = tmp-> handle;
			data[6] = tmp-> handle >> 8;
			data[7] = tmp-> handle >> 16;
			data[8] = tmp-> handle >> 24;

			return 1;
		}
		case NEW_FOLDER_RESPONSE:{
			NewFolderResponse* tmp = (NewFolderResponse*)r_msg-> payload;
			data[5] = tmp-> handle;
			data[6] = tmp-> handle >> 8;
			data[7] = tmp-> handle >> 16;
			data[8] = tmp-> handle >> 24;		

			return 1;
		}
		case FILE_INFO_RESPONSE:{
			FileInfoResponse* tmp = (FileInfoResponse*)r_msg-> payload;
			data[5] = tmp-> parent;
			data[6] = tmp-> parent >> 8;
			data[7] = tmp-> parent >> 16;
			data[8] = tmp-> parent >> 24;
			data[9] = tmp-> size;
			data[10] = tmp-> size >> 8;
			data[11] = tmp-> size >> 16;
			data[12] = tmp-> size >> 24;
			data[13] = tmp-> name_length;
			uint8_t i;
			for(i=0; i<tmp->name_length;i++)
				data[14+i] = tmp-> name[i];

			free(tmp-> name);
			return 1;
		}
		case FOLDER_INFO_RESPONSE:{
			FolderInfoResponse* tmp = (FolderInfoResponse*)r_msg-> payload;
			data[5] = tmp-> parent;
			data[6] = tmp-> parent >> 8;
			data[7] = tmp-> parent >> 16;
			data[8] = tmp-> parent >> 24;
			data[9] = tmp-> name_length;
			uint8_t i;
			for(i=0; i<tmp->name_length;i++)
				data[10+i] = tmp-> name[i];
			data[11+i] = tmp-> file_count;
			data[12+i] = tmp-> file_count >> 8 ;
			data[13+i] = tmp-> file_count >> 16 ;
			data[14+i] = tmp-> file_count >> 24 ;
			uint8_t j;
			for(j=0; j<tmp->file_count;j++)
				data[15+i+j] = tmp-> files[j];
			data[16+i+j] = tmp-> folder_count;
			data[17+i+j] = tmp-> folder_count >> 8;
			data[18+i+j] = tmp-> folder_count >> 16;
			data[19+i+j] = tmp-> folder_count >> 24;
			uint8_t m;
			for(m=0; m<tmp->folder_count;m++)
				data[20+i+j+m] = tmp-> folders[m];	

			free(tmp-> name);
			free(tmp-> files);
			free(tmp-> folders);
			return 1;
		}
		case READ_FILE_RESPONSE:{
			ReadFileResponse* tmp = (ReadFileResponse*)r_msg-> payload;
			data[5] = tmp-> size;
			data[6] = tmp-> size >> 8;
			data[7] = tmp-> size >> 16;
			data[8] = tmp-> size >> 24;
			uint8_t i;
			for(i=0; i<tmp->size;i++)
				data[9+i] = tmp-> data[i];

			free(tmp-> data);
			return 1;
		}
		case ERROR_RESPONSE:{
			ErrorResponse* tmp = (ErrorResponse*)r_msg-> payload;
			data[5] = tmp-> error_code;
			data[6] = tmp-> msg_length;
			data[7] = tmp-> msg_length >> 8;
			data[8] = tmp-> msg_length >> 16;
			data[9] = tmp-> msg_length >> 24;
			uint8_t i;
			for(i=0; i< tmp->msg_length;i++)
				data[10+i] = tmp-> msg[i];

			free(tmp-> msg);
			return 1;
		}
	}
	
	return 0;
}

