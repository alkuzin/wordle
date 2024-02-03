/*
 * client_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/client.h"

int main(void) 
{
	try {
		key_t key = ftok(SHARED_MEMORY_BLOCK_NAME, SHARED_MEMORY_BLOCK_SIZE);
 
    	// shmget returns an identifier in shmid
    	int shmid = shmget(key, SHARED_MEMORY_BLOCK_SIZE, 0666 | IPC_CREAT);
 
    	// shmat to attach to shared memory
    	char* block = (char *)shmat(shmid, NULL, 0);
		_logf("server", "set shared memory block: <%p>\n", block);
 
		Client client(block);
		client.init();
 
    	// detach from shared memory
    	shmdt(block);

	}
	catch(exception e) 
	{
		std::cerr << "exception: ";
		
		switch(e) {
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
