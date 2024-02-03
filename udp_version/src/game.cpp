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
	set_attempts(ATTEMPTS_LIMIT);

	_set_hidden_words();
}

void Game::process_input(const char *input, bool *letters)
{
	u32 i;

	i = 0;
	while(input[i]) {
		letters[i] = (hidden_word[i] == input[i]);	
		i++;
	}
	
	while(i < WORD_LENGTH) {
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
	u32 random_word_index; 	

	srand(time(NULL));
	random_word_index = rand() % (WORDLIST_LENGTH - 1) + 1;

	std::strncpy(hidden_word, hidden_words.at(random_word_index).c_str(), WORD_LENGTH);
	hidden_word[WORD_LENGTH] = '\0';
}

void Game::_set_hidden_words(void)
{
	std::ifstream wordlist_file;
	std::string word;

	wordlist_file.open(wordlist_path, std::ios::in);
	
	if(!wordlist_file.is_open())
		throw WORDLIST_FILE_OPEN_EXCEPTION;

	while(std::getline(wordlist_file, word))
		hidden_words.push_back(word);	
	
	wordlist_file.close();	
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

void Game::set_attempts(u32 attempts) {
	this->attempts = attempts;
}

u32 Game::get_attempts(void) {
	return attempts;
}

void Game::decrement_attempts(void) {
	if(attempts)
		attempts--;
}
