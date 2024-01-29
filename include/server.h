/*
 * server.h
 * @alkuzin - 2024
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "client.h"
#include "utils.h"
#include "game.h"

#define DEFAULT_IP_ADDRESS  "127.0.0.1"
#define DEFAULT_PORT        8080
#define WORD_LENGTH         5
#define EXIT_CODE "-----"

class Server 
{
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int sockfd;	
	Game game;

	public:
		Server();
		Server(const char *ip_addr, u16 port);
		~Server();
		void init(void);
	
	private:
		void _bind(void);
		void _handle_client(void);
};

#endif // SERVER_H
