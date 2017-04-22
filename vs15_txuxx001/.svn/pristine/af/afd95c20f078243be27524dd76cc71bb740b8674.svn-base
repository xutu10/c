/*
 * marshaller.h
 *
 *  Created on: Jan 17, 2014
 *      Author: meyfa011
 */

#ifndef MARSHALLER_H_
#define MARSHALLER_H_

#include <stdlib.h>
#include <vfilesystem_server_messages.h>

int32_t unmarshall(uint8_t *data, uint32_t size, FileServerMessage **msg_out);
void free_message(FileServerMessage *msg);

int32_t marshall(uint8_t *data, uint32_t size, FileServerMessage *msg);


#endif /* MARSHALLER_H_ */
