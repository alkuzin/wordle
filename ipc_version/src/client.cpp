/*
 * client.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/client.h"

Client::Client(char *addr)
{
	shm_addr = addr;	
	_set_client_name();
}

void Client::_set_client_name(void)
{
	std::memset(client_name, 0, sizeof(client_name));
	
	if (getlogin_r(client_name, sizeof(client_name)) != 0)
    	std::strncpy(client_name, DEFAULT_CLIENT_NAME, sizeof(client_name));
}

void Client::init(void)
{

	_handle_server();
}

void Client::_handle_server(void)
{
	//system("clear");
	ui.display_banner();

	do {
		std::cout << "<" << client_name << ">: ";
		
		// sending word
		std::cin.getline(shm_addr, SHARED_MEMORY_BLOCK_SIZE);

		_logf("client", "sent message: (%s)\n", shm_addr);
		
		// receiving bytes array
		 std::cout << "Result: " << shm_addr << std::endl;
		 
		 std::cout << "Attempts: " << shm_addr << std::endl;

		//receiving number of attempts left
		ui.set_attempts(std::atoi(shm_addr));
		_logf("client", "attempts left: %u\n", ui.get_attempts());
		
	} while(ui.get_attempts());
}
