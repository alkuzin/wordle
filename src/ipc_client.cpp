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
