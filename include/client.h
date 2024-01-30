/*
 * client.h
 * @alkuzin - 2024
 *
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include "utils.h"
#include "ui.h"

#define DEFAULT_IP_ADDRESS   "127.0.0.1"
#define DEFAULT_PORT         12345
#define DEFAULT_CLIENT_NAME  "client"
#define CLIENT_NAME_SIZE     256
#define ATTEMPTS_BYTES_SIZE  12

class Client 
{
	struct sockaddr_in server_addr;
	char   client_name[CLIENT_NAME_SIZE];
	int    sockfd;	
	UI     ui;

	public:
		Client();
		Client(const char *ip_addr, u16 port);
		~Client();
		
		void init(void);
	
	private:
		void _set_client_name(void);
		void _handle_server(void);
};

#endif // CLIENT_H
