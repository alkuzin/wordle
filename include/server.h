/*
 * server.h
 * @alkuzin - 2024
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "client.h"
#include "utils.h"
#include "game.h"

#define CLIENT_LIMIT  5
#define INPUT_SIZE    64

class Server 
{
	int server_fd;	
	struct sockaddr_in server_addr;
	u32 addr_len;
	Game game;

	public:
		Server();
		Server(const char *ip_addr, u16 port);
		~Server();
		
		void init(void);
	
	private:
		void _bind(void);
		void _listen(void);
		void handle_client(int client_fd);
};

#endif // SERVER_H
