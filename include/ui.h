/*
 * ui.h - game interface for client
 * @alkuzin - 2024
 *
 */

#ifndef UI_H
#define UI_H

#include <iostream>
#include <cstring>
#include <vector>
#include "utils.h"

#define WORD_LENGTH      5
#define ATTEMPTS_LIMIT   5

class UI 
{
	std::vector<bool *> previous_letters;
	std::vector<char *> previous_words;
	u32 attempts;

	public:
		UI(void);
		~UI();
		
		void display(void);
		bool is_guessed(void);
		u32  get_attempts(void);
		void decrement_attempts(void);
		void save_state(const char *current_word, const bool *current_letters);
		void display_result(void);
	
	private:
		void _display_letters(void);
		void _add_word(char *word);
		void _add_letters(bool *letters);
};

#endif // UI_H
