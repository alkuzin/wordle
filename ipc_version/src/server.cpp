/*
 * server.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/server.h"

Server::Server(char *addr) {
	shm_addr = addr;
}

void Server::init(void)
{
	_logf("server", "set shared memory address: <%p>\n", shm_addr);
	
	while(true) {
		_log("server", "--- handle client ---");	
		_handle_client();
		std::cout << std::endl;
	}
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

	do {
		std::memset(result, 0, sizeof(result));
		
		attempts = game.get_attempts();

		_logf("server", "client send message: (%s)\n", shm_addr);
		
		game.process_input(shm_addr, letters, result, SHARED_MEMORY_BLOCK_SIZE);
		_logf("server", "processing result: \"%s\"\n", result);	
		send(result);
		
		_logf("server", "sending the number of attempts left to client: (%u)\n", attempts);
		snprintf(shm_addr, UTOA_SIZE, "%u", attempts);

		if(game.is_guessed(letters))
			break;

		game.decrement_attempts();
		_logf("server", "(attempts left: %u)\n", game.get_attempts());
		std::memset(shm_addr, 0, SHARED_MEMORY_BLOCK_SIZE);
		
		exit(0);
	} while(game.get_attempts());
	
	game.set_attempts(ATTEMPTS_LIMIT);
	_log("server", "--- client finished game ---");
}
