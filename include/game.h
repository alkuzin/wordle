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

#define WORD_LENGTH            6
#define ATTEMPTS_LIMIT         6
#define WORDLIST_LENGTH        15 
#define WORDLIST_PATH_LENGTH   128
#define DEFAULT_WORDLIST_PATH  "wordlist.txt"

class Game 
{
	char wordlist_path[WORDLIST_PATH_LENGTH];
	char word[WORD_LENGTH + 1];
	u32  attempts;

	public:
		Game(void);
		Game(const char *wordlist_path, u32 attempts);
		void process_input(const char *input, bool *letters);
		void set_wordlist_path(const char *wordlist_path);
		char *get_hidden_word(void);
		void set_word(void);
};

#endif // GAME_H
