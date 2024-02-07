/*
 * udp_server.h - UDP version of server
 * @alkuzin - 2024
 *
 */

#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

#include "server.h"
#include "utils.h"
#include "game.h"

#define DEFAULT_PORT 12345 

class UDP_Server : public Server 
{
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int  sockfd;
	Game game;

	public:
		UDP_Server(void);
		~UDP_Server(void) override;
		int get_socket(void);

		void init(void) override;
		void recv(char *message, size_t size) override;
		void send(char *message, size_t size) override;
	
	protected:
		void _shutdown(void) override;
	
	private:
		void _bind(void);
		void _handle_client(void);
		void _show_server_info(void);
		void _show_client_info(void);
		void _get_client_ip(char *buffer);
};

#endif // UDP_SERVER_H
