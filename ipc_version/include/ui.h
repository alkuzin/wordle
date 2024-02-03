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
};

#endif // UI_H
