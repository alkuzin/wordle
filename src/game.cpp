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
	update_hidden_word();
}

void Game::process_input(const char *input, bool *letters)
{
	u32 i;

	i = 0;
	while(input[i]) {
		letters[i] = (hidden_word[i] == input[i]);	
		i++;
	}
	
	while(i < WORDLIST_LENGTH) {
		letters[i] = false;	
		i++;
	}
}

void Game::set_wordlist_path(const char *wordlist_path)
{
	std::memset(this->wordlist_path, 0, sizeof(wordlist_path));
	std::strncpy(this->wordlist_path, wordlist_path, WORDLIST_PATH_LENGTH);
}

void Game::update_hidden_word(void)
{
	std::ifstream wordlist_file(wordlist_path);
	u32 random_word_index; 	
	std::string line;
	u32 i; 	

	if(!wordlist_file.is_open())
		throw WORDLIST_FILE_OPEN_EXCEPTION;

	i = 0;
	srand(time(NULL));
	random_word_index = rand() % (WORDLIST_LENGTH - 1) + 1;

	while (std::getline(wordlist_file, line) && (i++ < random_word_index))
		continue;
	
	wordlist_file.close();
	
	std::strncpy(hidden_word, line.c_str(), WORD_LENGTH);
	hidden_word[WORD_LENGTH] = '\0';
}

char *Game::get_hidden_word(void) {
	return hidden_word;
}

bool Game::is_guessed(const bool *letters)
{
	for(int i = 0; i < WORD_LENGTH; i++) {
		if(!letters[i])
			return false;
	}
	
	return true;
}

u32 Game::get_attempts(void) {
	return attempts;
}

void Game::decrement_attempts(void) {
	attempts--;
}

void Game::reset_attempts(void) {
	attempts = ATTEMPTS_LIMIT;
}
