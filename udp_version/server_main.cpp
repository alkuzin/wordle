/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/server.h"

int main(void) 
{
	try {
		Server server;
		server.init();
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