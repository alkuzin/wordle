/*
 * udp_client.h - UDP version of client
 * @alkuzin - 2024
 *
 */

#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

#include "client.h"
#include "utils.h"
#include "ui.h"

#define DEFAULT_PORT         12345
#define ATTEMPTS_BYTES_SIZE  12

class UDP_Client : public Client
{
	struct sockaddr_in server_addr;
	int    sockfd;	
	UI     ui;

	public:
		UDP_Client();
		~UDP_Client() override;
		
		void init(void) override;
		void recv(char *message, size_t size) override;
		void send(char *message, size_t size) override;
		int  get_socket(void);
		void begin(void) override;
		void end(void) override;
	
	protected:
		void _shutdown(void) override;
	
	private:
		void _show_server_info(void);
		void _handle_server(void);
};

#endif // UDP_CLIENT_H
