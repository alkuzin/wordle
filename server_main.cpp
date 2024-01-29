/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/server.h"

int main(int argc, char **argv) 
{
	try {
	
		if(argc > 4) {
    		std::cerr << "server: too many arguments" << std::endl;
        	exit(EXIT_FAILURE);
    	}

		if(argc == 4) {
			char ip_addr[INET_ADDRSTRLEN];
			char wordlist_path[WORDLIST_PATH_LENGTH];
			u16  port;

			std::memset(ip_addr, 0, sizeof(ip_addr));
			std::memset(wordlist_path, 0, sizeof(wordlist_path));

			std::strncpy(ip_addr, argv[1], INET_ADDRSTRLEN);
			std::strncpy(wordlist_path, argv[3], WORDLIST_PATH_LENGTH);
			port = atoi(argv[2]);

			Server server(ip_addr, port, wordlist_path);
			server.init();
		}
		else {
			Server server;
			server.init();
		}
	}
	catch(exception e) 
	{
		std::cerr << "exception: ";
		switch(e) {
			case INVALID_IP_ADDRESS_EXCEPTION:
				std::cerr << "this ip address is invalid" << std::endl;
				break;

			case FORBIDDEN_PORT_EXCEPTION:
				std::cerr << "this port is not allowed" << std::endl;
				break;

			case SOCKET_CREATION_EXCEPTION:
				std::cerr << "socket creation failed" << std::endl;
				break;
			case BIND_EXCEPTION:
				std::cerr << "bind failed" << std::endl;
				break;
			
			case WORDLIST_FILE_OPEN_EXCEPTION:
				std::cerr << "failed to open wordlist file" << std::endl;
				break;
			
			case MEMORY_ALLOCATION_EXCEPTION:
				std::cerr << "memory allocation error" << std::endl;
				break;

			default:
				std::cerr << "some exception occurred" << std::endl;
				break;
		};
	}
	return 0;
}
