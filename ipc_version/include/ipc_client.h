/*
 * ipc_client.h - IPC version of client
 * @alkuzin - 2024
 *
 */

#ifndef IPC_CLIENT_H
#define IPC_CLIENT_H

#include <iostream>
#include <cstring>
#include <unistd.h>

#include "client.h"
#include "utils.h"
#include "ui.h"

#define DEFAULT_CLIENT_NAME  "client"
#define CLIENT_NAME_SIZE     256
#define ATTEMPTS_BYTES_SIZE  12

class IPC_Client : public Client
{
	char  client_name[CLIENT_NAME_SIZE];
	sem_t *sem_server;
	sem_t *sem_client;
	char  *shm_addr;
	UI    ui;
	
	public:
		IPC_Client(char *addr, sem_t *sem_s, sem_t *sem_c);
		~IPC_Client() override;		
		
		void init(void) override;
		void recv(char *message, size_t size) override;
		void send(char *message, size_t size) override;
		int  get_socket(void);

	protected:
		void _shutdown(void) override;
	
	private:
		void _set_client_name(void);
		void _handle_server(void);
};

#endif // IPC_CLIENT_H
