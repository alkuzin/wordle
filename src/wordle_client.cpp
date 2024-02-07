/*
 * wordle_client.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/wordle_client.h"

Wordle_Client::Wordle_Client(enum transfer_mode mode)
{
	this->mode = mode;
	client = nullptr;
}

void Wordle_Client::init(void)
{
	switch(mode) {
		case UDP:
			_init_udp();
			break;
		
		case IPC:
			_init_ipc();
			break;
		
		default:
			std::cerr << "incorrect client mode" << std::endl;
			exit(EXIT_FAILURE);
			break;
	}
}

void Wordle_Client::_init_udp(void) 
{
	try {
		client = new UDP_Client();
		client->init();
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
		
			default:
				std::cerr << "some exception occurred" << std::endl;
				break;
		};
	}
}

void Wordle_Client::_init_ipc(void)
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
		
	client = new IPC_Client(block, sem_server, sem_client);
	client->init();
 
    // detach from shared memory
    shmdt(block);
}

void Wordle_Client::_shutdown(void) {
	delete client;
}

Wordle_Client::~Wordle_Client(void) {
	_shutdown();
}
