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

#define DEFAULT_IP_ADDRESS  "127.0.0.123"
#define DEFAULT_PORT        12345

class Client 
{
	int sockfd;	
	struct sockaddr_in server_addr;
	UI ui;

	public:
		Client();
		Client(const char *ip_addr, u16 port);
		~Client();
		
		void init(void);
	
	private:
		void _handle_server(void);
};

#endif // CLIENT_H
