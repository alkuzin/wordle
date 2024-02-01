/*
 * client_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/client.h"

int main(int argc, char **argv) 
{
	try {

		if(argc > 3) {
			std::cerr << "client: too many arguments" << std::endl;
			exit(EXIT_FAILURE);
		}

		if(argc == 3) {
			char ip_addr[INET_ADDRSTRLEN];
			u16  port;	

			std::memset(ip_addr, 0, sizeof(ip_addr));
			std::strncpy(ip_addr, argv[1], INET_ADDRSTRLEN);
			port = atoi(argv[2]);

			Client client(ip_addr, port);
			client.init();
		}
		else {
			Client client;
			client.init();
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
