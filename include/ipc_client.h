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

#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

#include "client.h"
#include "utils.h"
#include "ui.h"

#define SHARED_MEMORY_BLOCK_NAME "wordle_shared_memory"
#define SHARED_MEMORY_BLOCK_SIZE 128
#define IPC_RESULT_ERROR         -1
#define SEM_SERVER_NAME 		 "/sem_server"
#define SEM_CLIENT_NAME 		 "/sem_client"
#define ATTEMPTS_BYTES_SIZE      12

class IPC_Client : public Client
{
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
		void begin(void) override;
		void end(void) override;

	protected:
		void _shutdown(void) override;
};

#endif // IPC_CLIENT_H
