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
	_set_client_name();
}

void Wordle_Client::_set_client_name(void)
{
	std::memset(client_name, 0, sizeof(client_name));
	
	if (getlogin_r(client_name, sizeof(client_name)) != 0)
    	std::strncpy(client_name, DEFAULT_CLIENT_NAME, sizeof(client_name));
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

void Wordle_Client::_process(void)
{
	char attempts_bytes[ATTEMPTS_BYTES_SIZE];
	char message[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	char bytes[WORD_LENGTH + 1];
		
	system("clear");
	ui.display_banner();
	ui.set_attempts(ATTEMPTS_LIMIT);

	do {
		std::memset(attempts_bytes, 0, sizeof(attempts_bytes));
		std::memset(letters,        0, sizeof(letters));
		std::memset(message,        0, sizeof(message));
		std::memset(bytes,          0, sizeof(bytes));

		client->begin();

		// get user input
		std::cout << "<" << client_name << ">: ";
		_getinput(message, sizeof(message));
		message[WORD_LENGTH] = '\0';

		if(message[0] == '\0')
			continue;
	
		// sending word
		client->send(message, sizeof(message));

		// receiving bytes array
		client->recv(bytes, sizeof(bytes));
		bytes[WORD_LENGTH] = '\0';
		
		// converting bytes array to bool array
		_convert_to_bool(bytes, letters, sizeof(letters));
		
		#ifdef IPC_MODE
			// liberating server
			sem_post(sem_server);

			// waiting for server
			sem_wait(sem_client);
		#endif
		
		// receiving number of attempts left
		client->recv(attempts_bytes, sizeof(attempts_bytes));
		attempts_bytes[ATTEMPTS_BYTES_SIZE] = '\0';
	
		// update attempts
		ui.set_attempts(std::atoi(attempts_bytes));
		ui.display_word(message, letters);
		ui.decrement_attempts();
		
		if(ui.is_guessed(letters))
			break;
			
		#ifdef IPC_MODE
			// liberating server 
			sem_post(sem_server);
		#endif
			client->end();

	} while(ui.get_attempts());
	
	ui.display_result(letters);
	
	#ifdef IPC_MODE
		// liberating server 
		sem_post(sem_server);
	#endif
}

void Wordle_Client::_init_udp(void) 
{
	try {
		client = new UDP_Client();
		client->init();
		
		_process();
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
	
	_process();
	
	// liberating server 
	sem_post(sem_server);

    // detach from shared memory
    shmdt(block);
}

void Wordle_Client::_shutdown(void) {
	delete client;
}

Wordle_Client::~Wordle_Client(void) {
	_shutdown();
}
