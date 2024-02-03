/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/server.h"

int main(void) 
{
	try {
		key_t key; 
    	int shmid;
		char *block;
		
		sem_t *sem_server;
		sem_t *sem_client;
		
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
	}
	catch(exception e) 
	{
		std::cerr << "exception: ";
		switch(e) {

			case WORDLIST_FILE_OPEN_EXCEPTION:
				std::cerr << "failed to open wordlist file" << std::endl;
				break;
			
			case MEMORY_ALLOCATION_EXCEPTION:
				std::cerr << "memory allocation error" << std::endl;
				break;

			default:
				std::cerr << "some exception occurred" << std::endl;
				break;
		};
	}
	return 0;
}
