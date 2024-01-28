/*
 * client_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/client.h"

int main(int argc, char **argv) 
{
	if(argc > 1) {
		std::cout << argv[0] << std::endl;
	}
	
	std::cout << "RUN CLIENT" << std::endl;		

	Client client;
	//Client client(argv[1], atoi(argv[2]));

	client.init();
	
	return 0;
}
