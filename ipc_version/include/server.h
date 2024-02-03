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
	char *shm_addr;
	Game game;

	public:
		Server(char *addr);
		
		void init(void);
		void send(char *message);

	private:
		void _handle_client(void);
};

#endif // SERVER_H
