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

#define DEFAULT_IP_ADDRESS   "127.0.0.1"
#define CLIENT_INVITATION    "#####"
#define DEFAULT_PORT       	 12345 

class Server 
{
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int  sockfd;
	Game game;

	public:
		Server();
		~Server();
		
		void init(void);
		int get_socket(void);
	
	private:
		void _bind(void);
		void _handle_client(void);
		void _show_server_info(void);
		void _show_client_info(void);
		void _get_client_ip(char *buffer);
};

#endif // SERVER_H
