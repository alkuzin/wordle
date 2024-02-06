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

#include "server.h"
#include "utils.h"
#include "game.h"

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
