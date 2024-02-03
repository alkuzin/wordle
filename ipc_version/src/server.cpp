/*
 * server.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/server.h"

Server::Server(char *addr, sem_t *sem_s, sem_t *sem_c)
{
	shm_addr = addr;
	sem_server = sem_s;
	sem_client = sem_c;
}

void Server::init(void)
{
	while(true) {
		
		_log("server", "--- handle client ---");	
		_handle_client();
		std::cout << std::endl;
		
	}

	sem_close(sem_client);
}

void Server::send(char *message) {
	std::memcpy(shm_addr, message, SHARED_MEMORY_BLOCK_SIZE);
}

void Server::_handle_client(void)
{
	char result[SHARED_MEMORY_BLOCK_SIZE];
	char hidden_word[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	u32  attempts;

	// set hidden word
	game.update_hidden_word();
	std::memset(hidden_word, 0, sizeof(hidden_word));
	std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);
	_logf("server", "set hidden word: (%s)\n", hidden_word);	

	attempts = ATTEMPTS_LIMIT;
	_logf("server", "set attempts limit: (%u)\n", attempts);	
	
	char buffer[UTOA_SIZE];

	do {
		std::memset(result, 0, sizeof(result));
		std::memset(shm_addr, 0, SHARED_MEMORY_BLOCK_SIZE);
		std::memset(buffer, 0, sizeof(buffer));
		attempts = game.get_attempts();
		
		// waiting for client
		_log("server", "waiting for client");
		std::cout << std::endl;
		_log("server", "[sem_wait(sem_server)]");
		sem_wait(sem_server);

		// receiving & processing word 
		_logf("server", "client send message: (%s)\n", shm_addr);
		game.process_input(shm_addr, letters, result, SHARED_MEMORY_BLOCK_SIZE);
		_logf("server", "processing result: \"%s\"\n", result);	
		send(result);
		
		// liberate client
		std::cout << std::endl;
		_log("server", "[sem_post(sem_client)]");
		sem_post(sem_client);
		
		// waiting for client
		std::cout << std::endl;
		_log("server", "[sem_wait(sem_server)]");
		sem_wait(sem_server);
	
		// sending the number of attempts left
		_logf("server", "attempts left: (%u)\n", attempts);
		
		snprintf(buffer, UTOA_SIZE, "%u", attempts);
		std::strncpy(shm_addr, buffer, UTOA_SIZE);	
		std::cout << "shm_addr: " << shm_addr << std::endl;
		std::cout << "buffer: " << buffer << std::endl;
		std::cout << "shm_addr: " << shm_addr << std::endl;

		if(game.is_guessed(letters))
			break;

		game.decrement_attempts();
		_logf("server", "(attempts left: %u)\n", game.get_attempts());
		
		// liberate client
		std::cout << std::endl;
		_log("server", "[sem_post(sem_client)]");
		sem_post(sem_client);
		
		// waiting for client
		std::cout << std::endl;
		_log("server", "[sem_wait(sem_server)]");
		sem_wait(sem_server);

	} while(game.get_attempts());
	
	game.set_attempts(ATTEMPTS_LIMIT);
	_log("server", "--- client finished game ---");
}
