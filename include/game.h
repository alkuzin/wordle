/*
 * game.h
 * @alkuzin - 2024
 *
 */

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>
#include <ctime>
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
	
	protected:
		char hidden_word[WORD_LENGTH + 1];
		u32  attempts;

	public:
		Game(void);
		
		void process_input(const char *input, bool *letters);
		void set_wordlist_path(const char *wordlist_path);
		bool is_guessed(const bool *letters);
		void decrement_attempts(void);
		void update_hidden_word(void);
		char *get_hidden_word(void);
		void reset_attempts(void);
		u32  get_attempts(void);
};

#endif // GAME_H
