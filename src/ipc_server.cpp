/*
 * ipc_server.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/ipc_server.h"

IPC_Server::IPC_Server(void)
{
	shm_addr   = nullptr;
	sem_server = nullptr;
	sem_client = nullptr;
	shmid      = 0;
}

void IPC_Server::init(void) 
{
	key_t key; 
		
	_log("server", "init server");
	key = ftok(SHARED_MEMORY_BLOCK_NAME, SHARED_MEMORY_BLOCK_SIZE);
 
   	// returns an identifier in shmid
   	shmid = shmget(key, SHARED_MEMORY_BLOCK_SIZE, 0666 | IPC_CREAT);
 
   	// attach to shared memory
   	shm_addr = (char *)shmat(shmid, NULL, 0);
	_logf("server", "set shared memory block: <%p>\n", shm_addr);

	// setup semaphores
	sem_unlink(SEM_SERVER_NAME);
	sem_unlink(SEM_CLIENT_NAME);
		
	sem_server = sem_open(SEM_SERVER_NAME, O_CREAT, 0777, 0);
	if(sem_server == SEM_FAILED) {
		perror("sem_open/sem_server");
		exit(EXIT_FAILURE);
	}
	_log("server", "sem_server opened");
		
	sem_client = sem_open(SEM_CLIENT_NAME, O_CREAT, 0777, 0);
	if(sem_client == SEM_FAILED) {
		perror("sem_open/sem_client");
		exit(EXIT_FAILURE);
	}
	_log("server", "sem_client opened");
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

IPC_Server::~IPC_Server(void) 
{
	_shutdown();
    
	// detach from shared memory
    shmdt(shm_addr);
 
    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);
}
