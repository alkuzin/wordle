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

void Client::init(void) 
{
	_handle_server();
	sem_close(sem_server);
	sem_close(sem_client);
}

void Client::_handle_server(void)
{
	system("clear");
	ui.display_banner();
	ui.set_attempts(ATTEMPTS_LIMIT);

	char word[WORD_LENGTH + 1];

	do {
		std::cout << "<" << client_name << ">: ";
		
		// sending word
		_getinput(word, WORD_LENGTH + 1);
		std::strncpy(shm_addr, word, WORD_LENGTH + 1);
		
		// liberating server 
		sem_post(sem_server);
		// waiting for server
		sem_wait(sem_client);
		
		// receiving bytes array
		if(std::strncmp(shm_addr, "WIN", 3) == 0) {
			std::cout << WIN_MESSAGE << std::endl;
			break;
		}
		
		if(std::strncmp(shm_addr, "LOSE", 4) == 0) {
			std::cout << LOSE_MESSAGE << std::endl;
			break;
		}

		std::cout << "\n" << shm_addr;
		
		// liberating server 
		sem_post(sem_server);
		// waiting for server
		sem_wait(sem_client);
		
		//receiving number of attempts left
		ui.set_attempts(std::atoi(shm_addr));
		ui.decrement_attempts();
		std::cout << " (" << ui.get_attempts() << " attempts left)\n" << std::endl;

		// liberating server 
		sem_post(sem_server);

	} while(ui.get_attempts());
	
	// liberating server 
	sem_post(sem_server);
}
