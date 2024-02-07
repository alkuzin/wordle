/*
 * ipc_server.h - IPC version of server
 * @alkuzin - 2024
 *
 */

#ifndef IPC_SERVER_H
#define IPC_SERVER_H

#include <iostream>
#include <cstring>
#include <unistd.h>

#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>

#include "server.h"
#include "utils.h"
#include "game.h"

#define SHARED_MEMORY_BLOCK_NAME "wordle_shared_memory"
#define SHARED_MEMORY_BLOCK_SIZE 128
#define IPC_RESULT_ERROR         -1
#define SEM_SERVER_NAME 		 "/sem_server"
#define SEM_CLIENT_NAME 		 "/sem_client"

class IPC_Server : public Server
{
	sem_t *sem_server;
	sem_t *sem_client;
	char  *shm_addr;
	Game  game;

	public:
		IPC_Server(char *addr, sem_t *sem_s, sem_t *sem_c);
		~IPC_Server(void) override;

		void init(void) override;
		void send(char *message, size_t size) override;
		void recv(char *message, size_t size) override;

	protected:
		void _shutdown(void) override;

	private:
		void _handle_client(void);
};

#endif // IPC_SERVER_H
