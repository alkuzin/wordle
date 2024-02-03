/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/server.h"

int main(void) 
{
	sem_t *sem_server;
	sem_t *sem_client;
	char *block;
	key_t key; 
   	int shmid;
		
	key = ftok(SHARED_MEMORY_BLOCK_NAME, SHARED_MEMORY_BLOCK_SIZE);
 
   	// shmget returns an identifier in shmid
   	shmid = shmget(key, SHARED_MEMORY_BLOCK_SIZE, 0666 | IPC_CREAT);
 
   	// shmat to attach to shared memory
   	block = (char *)shmat(shmid, NULL, 0);
	_logf("server", "set shared memory block: <%p>\n", block);

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
		
	Server server(block, sem_server, sem_client);
	server.init();
 
    // detach from shared memory
    shmdt(block);
 
    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);
	
	return 0;
}
