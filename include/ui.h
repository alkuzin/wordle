/*
 * ui.h - game interface for client
 * @alkuzin - 2024
 *
 */

#ifndef UI_H
#define UI_H

#include "game.h"

class UI : public Game 
{
	public:
		UI(void);
		
		void display_banner(void);
		void display_result(const bool *letters);
		void display_word(const char* word, const bool *letters);
};

#endif // UI_H
