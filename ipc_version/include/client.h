/*
 * client.h
 * @alkuzin - 2024
 *
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <cstring>
#include <unistd.h>

#include "utils.h"
#include "ui.h"

#define DEFAULT_CLIENT_NAME  "client"
#define CLIENT_NAME_SIZE     256
#define ATTEMPTS_BYTES_SIZE  12

class Client 
{
	char  client_name[CLIENT_NAME_SIZE];
	sem_t *sem_server;
	sem_t *sem_client;
	char  *shm_addr;
	UI    ui;
	
	public:
		Client(char *addr, sem_t *sem_s, sem_t *sem_c);
		
		void init(void);
	
	private:
		void _set_client_name(void);
		void _handle_server(void);
};

#endif // CLIENT_H
