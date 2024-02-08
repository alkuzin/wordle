/*
 * ipc_client.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/ipc_client.h"

IPC_Client::IPC_Client(char *addr, sem_t *sem_s, sem_t *sem_c)
{
	shm_addr = addr;	
	sem_server = sem_s;
	sem_client = sem_c;
}

void IPC_Client::init(void) {
	_log("client", "init client");
}

void IPC_Client::_shutdown(void) 
{
	sem_close(sem_server);
	sem_close(sem_client);
}

IPC_Client::~IPC_Client(void) {
	_shutdown();
}

void IPC_Client::begin(void) {
	std::cout << "Try to guess word with " << WORD_LENGTH << " symbols" << std::endl;
}

void IPC_Client::end(void) 
{
	// liberating server 
	sem_post(sem_server);

	std::cout << "Try again\n" << std::endl;
}

void IPC_Client::recv(char *message, size_t size) 
{
	std::memcpy(message, shm_addr, size);
	
	// liberating server 
	sem_post(sem_server);
	
	// waiting for server
	sem_wait(sem_client);
}

void IPC_Client::send(char *message, size_t size) 
{
	std::memcpy(shm_addr, message, size);
	
	// liberating server 
	sem_post(sem_server);
	
	// waiting for server
	sem_wait(sem_client);
}

/*void IPC_Client::_handle_server(void)
{
	system("clear");
	ui.display_banner();
	ui.set_attempts(ATTEMPTS_LIMIT);

	char word[WORD_LENGTH + 1];
	char buffer[SHARED_MEMORY_BLOCK_SIZE];

	do {
		std::cout << "<" << client_name << ">: ";
		
		// sending word
		_getinput(word, WORD_LENGTH + 1);
		send(word, WORD_LENGTH + 1);
		
		// liberating server 
		sem_post(sem_server);
		// waiting for server
		sem_wait(sem_client);
		
		// receiving bytes array
		recv(buffer, SHARED_MEMORY_BLOCK_SIZE);
		buffer[SHARED_MEMORY_BLOCK_SIZE] = '\0';

		if(std::strncmp(buffer, "WIN", 3) == 0) {
			std::cout << WIN_MESSAGE << std::endl;
			break;
		}
		
		if(std::strncmp(buffer, "LOSE", 4) == 0) {
			std::cout << LOSE_MESSAGE << std::endl;
			break;
		}

		std::cout << buffer;
		
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
}*/
