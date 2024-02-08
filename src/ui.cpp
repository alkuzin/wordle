/*
 * ui.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/ui.h"

UI::UI(void) {
	set_attempts(ATTEMPTS_LIMIT);
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
		 	 "-----------------------------\n\n" << std::endl;
}

void UI::display_result(const bool *letters)
{
	if(is_guessed(letters))
		std::cout << WIN_MESSAGE << std::endl;
	else
		std::cout << LOSE_MESSAGE << std::endl;
}

void UI::display_word(const char* word, const bool *letters)
{
	u32 counter;

	counter = 0;
	std::cout << "\"" << word << "\" - ";
	for(u32 i = 0; i < WORD_LENGTH; i++) {
		if(letters[i]) {
			std::cout << " " << i + 1;
			
			if(i < WORD_LENGTH - 1)
				std::cout << ",";
			counter++;
		}
	}
	
	if(!counter)
		std::cout << "there isn't any correct letters";
	else
		std::cout << " letters are correct";

	std::cout << " (" << attempts - 1 << " attempts left)" << std::endl;
}
