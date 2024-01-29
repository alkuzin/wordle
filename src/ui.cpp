/*
 * ui.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/ui.h"

UI::UI(void) {
	attempts = ATTEMPTS_LIMIT;
}

void UI::display_banner(void) 
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
}

void UI::display_result(const bool *letters)
{
	if(is_guessed(letters)) {
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

void UI::display_word(const char* word, const bool *letters)
{
	u32 counter;

	counter = 0;
	std::cout << "[server]: \"" << word << "\" - ";
	for(u32 i = 0; i < WORD_LENGTH; i++) {
		if(letters[i]) {
			std::cout << " " << i + 1;
			
			if(i < WORD_LENGTH - 1)
				std::cout << ",";
			counter++;
		}
	}
	
	if(!counter)
		std::cout << "there isn't any correct letters" << std::endl;
	else
		std::cout << " letters are correct" << std::endl;
}
