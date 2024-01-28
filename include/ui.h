/*
 * ui.h - game interface for client
 * @alkuzin - 2024
 *
 */

#ifndef UI_H
#define UI_H

#include <iostream>
#include <cstring>
#include <algorithm> // for std::count
#include <vector>

#include "utils.h"

#define WORD_LENGTH      6
#define ATTEMPTS_LIMIT   6
#define WORDLIST_LENGTH  15 

class UI 
{
	std::vector<bool *> previous_letters;
	std::vector<char *> previous_words;
	u32 attempts;

	public:
		UI(void);
		~UI();
		
		bool is_guessed(void);
		void display(void);
		void display_result(const char *hidden_word);

		u32  get_attempts(void);
		void decrement_attempts(void);
		void save_state(const char *current_word, const bool *current_letters);
	
	private:
		void display_letters(void);
		void add_word(char *word);
		void add_letters(bool *letters);
};

#endif // UI_H
