/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/server.h"
#include <signal.h>

void handle_sigint(int signal);

int shm;
char *addr;

int main(int argc, char **argv) 
{
	try {
	
		if(argc > 4) {
    		std::cerr << "server: too many arguments" << std::endl;
        	exit(EXIT_FAILURE);
    	}
		
		// keyboard interruption handling
		signal(SIGINT, handle_sigint);
	
		shm = 0;
		addr = NULL;
		
		if(!shm) {
			munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
			close(shm);
			shm_unlink(SHARED_MEMORY_OBJECT_NAME);
			_log("server", "unlink shared memory successfull");
		}

		if((shm = shm_open(SHARED_MEMORY_OBJECT_NAME, O_CREAT|O_RDWR, 0777)) == -1) {
			perror("error (shm_open)");
			shm_unlink(SHARED_MEMORY_OBJECT_NAME);
			exit(EXIT_FAILURE);
		}
	
		if(!shm) {
			if((shm = ftruncate(shm, SHARED_MEMORY_OBJECT_SIZE + 1)) == -1) {
				perror("error (ftruncate)");
				exit(EXIT_FAILURE);
			}
			_log("server", "ftruncate successfull");
		}
		
		addr = (char *)mmap(0, SHARED_MEMORY_OBJECT_SIZE+1, PROT_WRITE|PROT_READ,
		MAP_SHARED, shm, 0);
    	
		if (addr == (char *) - 1) {
        	perror("error (mmap)");
        	exit(EXIT_FAILURE);
    	}
		
		_logf("server", "set shared memory address (addr): <%p>\n", addr);

		if(argc == 4) {
			char wordlist_path[WORDLIST_PATH_LENGTH];
			char ip_addr[INET_ADDRSTRLEN];
			u16  port;

			std::memset(ip_addr, 0, sizeof(ip_addr));
			std::memset(wordlist_path, 0, sizeof(wordlist_path));

			std::strncpy(ip_addr, argv[1], INET_ADDRSTRLEN);
			std::strncpy(wordlist_path, argv[3], WORDLIST_PATH_LENGTH);
			port = atoi(argv[2]);

			Server server(ip_addr, port, wordlist_path);
			server.init_shared_memory(shm, addr);
			server.init();
		}
		else {
			Server server;
			server.init_shared_memory(shm, addr);
			server.init();
		}

		munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
		close(shm);
		shm_unlink(SHARED_MEMORY_OBJECT_NAME);
		_log("server", "closing shared memory successfull");
	}
	catch(exception e) 
	{
		std::cerr << "exception: ";
		switch(e) {
			case INVALID_IP_ADDRESS_EXCEPTION:
				std::cerr << "this ip address is invalid" << std::endl;
				break;

			case FORBIDDEN_PORT_EXCEPTION:
				std::cerr << "this port is not allowed" << std::endl;
				break;

			case SOCKET_CREATION_EXCEPTION:
				std::cerr << "socket creation failed" << std::endl;
				break;
			case BIND_EXCEPTION:
				std::cerr << "bind failed" << std::endl;
				break;
			
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

void handle_sigint(int signal) 
{
	std::cout << "\nkeyboard interruption (" << signal << ")" << std::endl;

	munmap(addr, SHARED_MEMORY_OBJECT_SIZE);
	close(shm);
	_log("server", "closing shared memory successfull");
	exit(EXIT_SUCCESS);
}
