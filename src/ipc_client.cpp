/*
 * ipc_client.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/ipc_client.h"

IPC_Client::IPC_Client(void)
{
	shm_addr   = nullptr;	
	sem_server = nullptr;
	sem_client = nullptr;
   	shmid      = 0;
}

void IPC_Client::init(void) 
{
	key_t key; 
		
	_log("client", "init client");
	key = ftok(SHARED_MEMORY_BLOCK_NAME, SHARED_MEMORY_BLOCK_SIZE);
 
   	// returns an identifier in shmid
   	shmid = shmget(key, SHARED_MEMORY_BLOCK_SIZE, 0666 | IPC_CREAT);
 
   	// attach to shared memory
   	shm_addr = (char *)shmat(shmid, NULL, 0);

	sem_server = sem_open(SEM_SERVER_NAME, O_CREAT, 0777, 0);
	if(sem_server == SEM_FAILED) {
		perror("sem_open/sem_server");
		exit(EXIT_FAILURE);
	}
		
	sem_client = sem_open(SEM_CLIENT_NAME, O_CREAT, 0777, 0);
	if(sem_client == SEM_FAILED) {
		perror("sem_open/sem_client");
		exit(EXIT_FAILURE);
	}
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

void IPC_Client::_shutdown(void) 
{
	// liberating server 
	sem_post(sem_server);

    // detach from shared memory
    shmdt(shm_addr);
	
	sem_close(sem_server);
	sem_close(sem_client);
}

IPC_Client::~IPC_Client(void) {
	_shutdown();
}
