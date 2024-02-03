/*
 * game.h
 * @alkuzin - 2024
 *
 */

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include "utils.h"

#define DEFAULT_WORDLIST_PATH  "wordlist.txt"
#define WORDLIST_PATH_LENGTH   128
#define WORDLIST_LENGTH        50 
#define ATTEMPTS_LIMIT         5
#define WORD_LENGTH            5 

class Game 
{
	private:
		char wordlist_path[WORDLIST_PATH_LENGTH];
		std::vector<std::string> hidden_words;
		char hidden_word[WORD_LENGTH + 1];
	
	protected:
		u32  attempts;

	public:
		Game(void);
		
		void process_input(char *input, bool *letters, char *result, u32 size);
		void set_wordlist_path(const char *wordlist_path);
		bool is_guessed(const bool *letters);
		void update_hidden_word(void);
		char *get_hidden_word(void);
		void set_attempts(u32 attempts);
		void decrement_attempts(void);
		u32  get_attempts(void);
	
	private:
		void _set_hidden_words(void);
};

#endif // GAME_H
