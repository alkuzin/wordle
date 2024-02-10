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
	try {
		switch(mode) {
			case UDP:
				client = new UDP_Client();
				break;
		
			case IPC:
				client = new IPC_Client();
				break;
		
			default:
				std::cerr << "incorrect client mode" << std::endl;
				exit(EXIT_FAILURE);
				break;
		}
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

void Wordle_Client::_shutdown(void) {
	delete client;
}

Wordle_Client::~Wordle_Client(void) {
	_shutdown();
}
