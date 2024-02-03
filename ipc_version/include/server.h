/*
 * server.h
 * @alkuzin - 2024
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include "utils.h"
#include "game.h"

class Server 
{
	sem_t *sem_server;
	sem_t *sem_client;
	char *shm_addr;
	Game game;

	public:
		Server(char *addr, sem_t *sem_s, sem_t *sem_c);

		void init(void);
		void send(const char *message);

	private:
		void _handle_client(void);
};

#endif // SERVER_H
