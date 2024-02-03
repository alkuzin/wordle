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
