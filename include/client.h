/*
 * client.h
 * @alkuzin - 2024
 *
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "utils.h"
#include "ui.h"

class Client 
{
	int client_fd;	
	struct sockaddr_in client_addr;
	u32 addr_len;
	UI ui;

	public:
		Client();
		Client(int client_fd);
		Client(const char *ip_addr, u16 port);
		~Client();
		
		void init(void);
	
	private:
		void _connect(void);
		void handle_server(int server_fd);
};

#endif // CLIENT_H
