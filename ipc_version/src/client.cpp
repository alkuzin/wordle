/*
 * client.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/client.h"

Client::Client(char *addr, sem_t *sem_s, sem_t *sem_c)
{
	shm_addr = addr;	
	sem_server = sem_s;
	sem_client = sem_c;

	_set_client_name();
}

void Client::_set_client_name(void)
{
	std::memset(client_name, 0, sizeof(client_name));
	
	if (getlogin_r(client_name, sizeof(client_name)) != 0)
    	std::strncpy(client_name, DEFAULT_CLIENT_NAME, sizeof(client_name));
}

void Client::init(void) {
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
		
		// liberating server 
		_log("client", "[sem_post(sem_server)]");
		sem_post(sem_server);
		
		// waiting for server
		_log("client", "[sem_wait(sem_client)]");
		sem_wait(sem_client);
		
		// receiving bytes array
		std::cout << shm_addr << std::endl;
		
		// liberating server 
		_log("client", "[sem_post(sem_server)]");
		sem_post(sem_server);
		
		// waiting for server
		_log("client", "[sem_wait(sem_client)]");
		sem_wait(sem_client);
		
		//receiving number of attempts left
		std::cout << shm_addr << std::endl;
		ui.set_attempts(std::atoi(shm_addr));
		ui.decrement_attempts();
		std::cout << "(" << ui.get_attempts() << " attempts left)" << std::endl;

		// liberating server 
		_log("server", "[sem_post(sem_server)]");
		sem_post(sem_server);

	} while(ui.get_attempts());

	sem_close(sem_server);
	sem_close(sem_client);
}
