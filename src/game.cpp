/*
 * game.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/game.h"

Game::Game(void) 
{
	std::strncpy(wordlist_path, DEFAULT_WORDLIST_PATH, WORDLIST_PATH_LENGTH);
	attempts = ATTEMPTS_LIMIT;
	set_word();
}

Game::Game(const char *wordlist_path, u32 attempts) 
{
	std::strncpy(this->wordlist_path, wordlist_path, WORDLIST_PATH_LENGTH);
	this->attempts = attempts;
	set_word();
}

void Game::process_input(const char *input, bool *letters)
{
	int i;

	i = 0;
	while(input[i]) {
		letters[i] = (word[i] == input[i]);	
		i++;
	}
	
	while(i < WORDLIST_LENGTH) {
		letters[i] = false;	
		i++;
	}
}

char *Game::get_hidden_word(void) {
	return word;
}

void Game::set_word(void)
{
	u32 i; 	
	u32 random_word_index; 	
	std::string line;
	std::ifstream wordlist_file(wordlist_path);

	if(!wordlist_file.is_open())
		throw WORDLIST_FILE_OPEN_EXCEPTION;

	i = 0;
	srand(time(NULL));
	random_word_index = rand() % (WORDLIST_LENGTH - 1) + 1;

	while (std::getline(wordlist_file, line) && (i++ < random_word_index))
		continue;
	
	wordlist_file.close();
	
	std::strncpy(word, line.c_str(), WORD_LENGTH);
	word[WORD_LENGTH] = '\0';
}
