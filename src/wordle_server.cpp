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

void Wordle_Server::_process(void)
{
	char attempts_bytes[UTOA_SIZE];
	char hidden_word[WORD_LENGTH + 1];
	char message[WORD_LENGTH + 1];
	char bytes[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	u32  attempts;

	// set hidden word
	game.update_hidden_word();
	std::memset(hidden_word, 0, sizeof(hidden_word));
	std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);
	_logf("server", "set hidden word: (%s)\n", hidden_word);	

	attempts = ATTEMPTS_LIMIT;
	game.set_attempts(ATTEMPTS_LIMIT);
	_logf("server", "set attempts limit: (%u)\n", attempts);	

	do {
		std::memset(attempts_bytes, 0, sizeof(attempts_bytes));
		std::memset(message,        0, sizeof(message));
		std::memset(letters,        0, sizeof(letters));
		std::memset(bytes,          0, sizeof(bytes));

		std::cout << std::endl;

		server->begin();
		attempts = game.get_attempts();
		_utoa(attempts, attempts_bytes);
		
		server->recv(message, sizeof(message));
		message[WORD_LENGTH] = '\0';

		if(message[0] == '\0')
			break;
		
		_logf("server", "client send message: (%s)\n", message);
		game.process_input(message, letters);

		_logf("server", "%s", "converting bool array to bytes: ");
		_convert_to_bytes(letters, bytes, WORD_LENGTH);

		// display bool array of guesssed letters
		_display_bytes(bytes, WORD_LENGTH);	
		server->send(bytes, sizeof(bytes));

		game.decrement_attempts();
		_logf("server", "sending the number of attempts left to client: (%s)\n", attempts_bytes);
		server->send(attempts_bytes, sizeof(attempts_bytes));
		
		#ifdef IPC_MODE
			// liberate client
			_log("server", "sem_post(sem_client)");
			sem_post(sem_client);
		
			// waiting for client
			_log("server", "sem_wait(sem_server)");
			sem_wait(sem_server);
		#endif
		_logf("server", "(attempts left: %u)\n", game.get_attempts());
		server->end();
		
		if(game.is_guessed(letters)) {
			game.set_attempts(ATTEMPTS_LIMIT);
			_log("server", "--- client finished game ---");
			break;
		}

	} while(game.get_attempts());
		
	#ifdef IPC_MODE
		// liberate client
		_log("server", "sem_post(sem_client)");
		sem_post(sem_client);
		
		// waiting for client
		_log("server", "sem_wait(sem_server)");
		sem_wait(sem_server);
			
		//liberate server
		_log("server", "sem_post(sem_server)");
		sem_post(sem_server);
	#endif
}

void Wordle_Server::_init_udp(void) 
{
	try {
		server = new UDP_Server();
		server->init();
		
		while(true) {
			_log("server", "--- handle client ---");	
			_process();
			std::cout << std::endl;
		}
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
	server = new IPC_Server();
	server->init();
		
	while(true) {
		_log("server", "--- handle client ---");	
		_process();
		std::cout << std::endl;
	}
}

void Wordle_Server::_shutdown(void) {
	delete server;
}

Wordle_Server::~Wordle_Server(void) {
	_shutdown();
}
