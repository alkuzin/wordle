/*
 * ui.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/ui.h"

UI::UI(void) {
	attempts = ATTEMPTS_LIMIT;
}

void UI::display(void) 
{
	std::cout << 
			 "\n _       __               ____    \n"
			 "| |     / /___  _________/ / /__    \n"
			 "| | /| / / __ \\/ ___/ __  / / _ \\ \n"
			 "| |/ |/ / /_/ / /  / /_/ / /  __/   \n"
			 "|__/|__/\\____/_/   \\__,_/_/\\___/ \n\n"
		 	 "-----------------------------       \n"
		 	 "|      Guess the word!      |       \n"	
		 	 "-----------------------------" << std::endl;
	std::cout << "|     Attempts left: " << attempts << "      |" << std::endl;
	std::cout << "-----------------------------" << "\n" << std::endl;
	_display_letters();
}

void UI::display_result(void)
{
	if(is_guessed()) {
		std::cout << "-----------------------------" << std::endl;
		std::cout << "|      Word was guessed     |" << std::endl;
		std::cout << "-----------------------------\n\n\n" << std::endl;
	}
	else {
		std::cout << "-----------------------------" << std::endl;
		std::cout << "|         YOU LOST          |" << std::endl;
		std::cout << "-----------------------------\n\n\n" << std::endl;
	}
}

bool UI::is_guessed(void)
{
	bool *temp_letters;
	u32 prod;

	if(attempts && previous_letters.size()) {
		prod = 1;
		temp_letters = previous_letters.back();
		for(int i = 0; i < WORD_LENGTH; i++)
			prod *= (u32)temp_letters[i];

		return (bool)prod;
	}

	return false;
}

void UI::_display_letters(void)
{
	u64 i;
	u32 j, correct_letters;

	for(i = 0; i < previous_words.size(); i++) {
		j = 0;
		correct_letters = 0;
		while(previous_words[i][j]) {
			putchar(' ');
			putchar(previous_words[i][j]);
			j++;
		}
		
		// print spaces in case of length of attempt word is less than WORD_LENGTH
		while(j < WORD_LENGTH) {
			putchar(' ');
			j++;
		}
		putchar('\n');
		
		j = 0;
		while(previous_words[i][j]) {
			putchar(' ');
			if(previous_letters[i][j]) {
				putchar('.');
				correct_letters++;
			}
			else
				putchar('^');
			j++;
		}
		
		// print ^ in case of length of attempt word is less than WORD_LENGTH
		while(j < WORD_LENGTH) {
			putchar(' ');
			putchar('^');
			j++;
		}

		std::cout << "\t- " << correct_letters << "/" << WORD_LENGTH << " correct" << std::endl;
		putchar('\n');
	}

	while(i < ATTEMPTS_LIMIT) {
		putchar('\n');
		putchar('\n');
		putchar('\n');
		i++;
	}	
}

void UI::save_state(const char *current_word, const bool *current_letters)
{
	char *word_copy;
	bool *letters_copy;
	
	word_copy = strdup(current_word);

	if(!word_copy)
		error("memory allocation error");

	_add_word(word_copy);

	letters_copy = static_cast<bool *>(std::malloc(sizeof(bool *) * WORD_LENGTH));

	if(!letters_copy)
		error("memory allocation error");

	for(int i = 0; i < WORD_LENGTH; i++)
		letters_copy[i] = current_letters[i];

	_add_letters(letters_copy);
}

u32 UI::get_attempts(void) {
	return attempts;
}

void UI::decrement_attempts(void) {
	attempts--;
}

void UI::_add_word(char *word) {
	previous_words.push_back(word);
}

void UI::_add_letters(bool *letters) {
	previous_letters.push_back(letters);
}

UI::~UI(void) 
{
	for(u64 i = 0; i < previous_words.size(); i++)
		free(previous_words.at(i));
	
	for(u64 i = 0; i < previous_letters.size(); i++)
		std::free(previous_letters.at(i));
}
