/*
 * ipc_server.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/ipc_server.h"

IPC_Server::IPC_Server(char *addr, sem_t *sem_s, sem_t *sem_c)
{
	shm_addr = addr;
	sem_server = sem_s;
	sem_client = sem_c;
}

void IPC_Server::init(void)
{
	while(true) {
		_log("server", "--- handle client ---");	
		_handle_client();
		std::cout << std::endl;
	}
	sem_close(sem_client);
	sem_close(sem_server);
}

void IPC_Server::_shutdown(void) {
	sem_close(sem_server);
	sem_close(sem_client);
}

IPC_Server::~IPC_Server(void) {
	_shutdown();
}

void IPC_Server::send(char *message, size_t size) {
	std::memcpy(shm_addr, message, size);
}

void IPC_Server::recv(char *message, size_t size) {
	std::memcpy(message, shm_addr, size);
}

void IPC_Server::_handle_client(void)
{
	char result[SHARED_MEMORY_BLOCK_SIZE];
	char hidden_word[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	char attempts_buffer[UTOA_SIZE];
	char buffer[WORD_LENGTH + 1];
	u32  attempts;

	// set hidden word
	game.update_hidden_word();
	std::memset(hidden_word, 0, sizeof(hidden_word));
	std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);
	_logf("server", "set hidden word: (%s)\n", hidden_word);	

	attempts = ATTEMPTS_LIMIT;
	_logf("server", "set attempts limit: (%u)\n", attempts);	
	
	do {
		std::memset(attempts_buffer, 0, sizeof(attempts_buffer));
		std::memset(shm_addr, 0, SHARED_MEMORY_BLOCK_SIZE);
		std::memset(result, 0, sizeof(result));

		attempts = game.get_attempts();
		
		// waiting for client
		_log("server", "waiting for client");
		_log("server", "sem_wait(sem_server)");
		sem_wait(sem_server);
		
		if(!shm_addr[0]) {
			_log("server", "sem_wait(sem_server)");
			sem_wait(sem_server);
		}

		// receiving & processing word 
		recv(buffer, WORD_LENGTH + 1);
		std::cout << std::endl;
		_logf("server", "client send message: (%s)\n", buffer);
		
		game.process_input(buffer, letters, result, SHARED_MEMORY_BLOCK_SIZE);
		
		if(game.is_guessed(letters)) {
			_log("server", "client win the game");	
			send((char *)"WIN", 4);
			break;
		}
		
		if((attempts == 1) && !game.is_guessed(letters)) {
			_log("server", "client lose the game");	
			send((char *)"LOSE", 5);
			break;
		}

		_logf("server", "processing result: (%s)\n", result);	
		send(result, SHARED_MEMORY_BLOCK_SIZE);
		
		// liberate client
		_log("server", "sem_post(sem_client)");
		sem_post(sem_client);
		
		// waiting for client
		_log("server", "sem_wait(sem_server)");
		sem_wait(sem_server);
	
		// sending the number of attempts left
		snprintf(attempts_buffer, UTOA_SIZE, "%u", attempts);
		send(attempts_buffer, UTOA_SIZE);

		game.decrement_attempts();
		_logf("server", "(attempts left: %u)\n", game.get_attempts());
		
		// liberate client
		_log("server", "sem_post(sem_client)");
		sem_post(sem_client);
		
		// waiting for client
		_log("server", "sem_wait(sem_server)");
		sem_wait(sem_server);

	} while(game.get_attempts());
	
	// liberate client
	_log("server", "sem_post(sem_client)");
	sem_post(sem_client);
		
	// waiting for client
	_log("server", "sem_wait(sem_server)");
	sem_wait(sem_server);
			
	//liberate server
	_log("server", "sem_post(sem_server)");
	sem_post(sem_server);
	
	game.set_attempts(ATTEMPTS_LIMIT);
	_log("server", "--- client finished game ---");
}
