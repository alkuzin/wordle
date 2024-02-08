/*
 * ipc_server.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/ipc_server.h"

IPC_Server::IPC_Server(char *addr, sem_t *sem_s, sem_t *sem_c)
{
	shm_addr = addr;
	sem_server = sem_s;
	sem_client = sem_c;
}

void IPC_Server::init(void) {
	_log("server", "init server");
}

void IPC_Server::begin(void)
{
	std::memset(shm_addr, 0, SHARED_MEMORY_BLOCK_SIZE);
	
	// waiting for client
	_log("server", "waiting for client");
	_log("server", "sem_wait(sem_server)");
	sem_wait(sem_server);
	
	if(!shm_addr[0])
		sem_wait(sem_server);
}

void IPC_Server::end(void)
{
	// liberate client
	_log("server", "sem_post(sem_client)");
	sem_post(sem_client);
	
	// waiting for client
	_log("server", "sem_wait(sem_server)");
	sem_wait(sem_server);
}

void IPC_Server::send(char *message, size_t size) 
{
	std::memcpy(shm_addr, message, size);
	
	// liberate client
	_log("server", "sem_post(sem_client)");
	sem_post(sem_client);
		
	// waiting for client
	_log("server", "sem_wait(sem_server)");
	sem_wait(sem_server);
}

void IPC_Server::recv(char *message, size_t size) {
	std::memcpy(message, shm_addr, size);
}

void IPC_Server::_shutdown(void) {
	sem_close(sem_server);
	sem_close(sem_client);
}

IPC_Server::~IPC_Server(void) {
	_shutdown();
}
