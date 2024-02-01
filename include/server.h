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

#include <sys/mman.h>
#include <fcntl.h>

#define SHARED_MEMORY_OBJECT_NAME "server_shared_memory"
#define SHARED_MEMORY_OBJECT_SIZE 128

#define DEFAULT_IP_ADDRESS   "127.0.0.1"
#define DEFAULT_PORT       	 12345 
#define WORD_LENGHT          5
#define ATTEMPTS_BYTES_SIZE  12
#define CLIENT_INVITATION    "#####"

class Server 
{
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int  sockfd;
	Game game;
	
	int _shm;
	char *_addr;

	public:
		Server();
		Server(const char *ip_addr, u16 port, const char *wordlist_path);
		~Server();
		
		void init(void);
		void init_shared_memory(int shm, char *addr);
	
	private:
		void _bind(void);
		void _handle_client(void);
		void _show_server_info(void);
		void _show_client_info(void);
		void _get_client_ip(char *buffer);

		void _open_shared_memory(void);
		void _create_shared_memory(void);
		void _fill_shared_memory(char *data, size_t size);
		void _close_shared_memory(void);
};

#endif // SERVER_H
