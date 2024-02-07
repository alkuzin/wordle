/*
 * wordle_server.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/wordle_server.h"

Wordle_Server::Wordle_Server(enum transfer_mode mode)
{
	this->mode = mode;
	server = nullptr;
}

void Wordle_Server::init(void)
{
	switch(mode) {
		case UDP:
			_init_udp();
			break;
		
		case IPC:
			_init_ipc();
			break;
		
		default:
			std::cerr << "incorrect server mode" << std::endl;
			exit(EXIT_FAILURE);
			break;
	}
}

void Wordle_Server::_init_udp(void) 
{
	try {
		server = new UDP_Server();
		server->init();
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

			default:
				std::cerr << "some exception occurred" << std::endl;
				break;
		};
	}
}

void Wordle_Server::_init_ipc(void)
{
	sem_t *sem_server;
	sem_t *sem_client;
	char  *block;
	key_t key; 
   	int   shmid;
		
	key = ftok(SHARED_MEMORY_BLOCK_NAME, SHARED_MEMORY_BLOCK_SIZE);
 
   	// returns an identifier in shmid
   	shmid = shmget(key, SHARED_MEMORY_BLOCK_SIZE, 0666 | IPC_CREAT);
 
   	// attach to shared memory
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
		
	server = new IPC_Server(block, sem_server, sem_client);
	server->init();
 
    // detach from shared memory
    shmdt(block);
 
    // destroy the shared memory
    shmctl(shmid, IPC_RMID, NULL);
}

void Wordle_Server::_shutdown(void) {
	delete server;
}

Wordle_Server::~Wordle_Server(void) {
	_shutdown();
}
