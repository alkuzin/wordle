/*
 * client_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/client.h"
#include <signal.h>

void sigint_handler(int signal);

Client client;

int main(void) 
{
	try {
		signal(SIGINT, sigint_handler);
		client.init();
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

			default:
				std::cerr << "some exception occurred" << std::endl;
				break;
		};
	}
	return 0;
}

void sigint_handler(int signal) {
	std::cout << std::endl;
	_logf("client", "received signal %d, exiting\n", signal);
	close(client.get_socket());
	exit(EXIT_SUCCESS);
}
