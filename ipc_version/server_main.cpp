/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/server.h"

int main(void) 
{
	try {
		key_t key = ftok(SHARED_MEMORY_BLOCK_NAME, SHARED_MEMORY_BLOCK_SIZE);
 
    	// shmget returns an identifier in shmid
    	int shmid = shmget(key, SHARED_MEMORY_BLOCK_SIZE, 0666 | IPC_CREAT);
 
    	// shmat to attach to shared memory
    	char* block = (char *)shmat(shmid, NULL, 0);
		_logf("server", "set shared memory block: <%p>\n", block);
 

		Server server(block);
		server.init();
    	//cout << "Data read from memory:" << str;
 
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
