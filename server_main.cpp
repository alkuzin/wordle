/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/wordle_server.h"
#include <signal.h>

void sigint_handler(int signal);

enum transfer_mode mode = UDP;
Wordle_Server wordle_server(mode);

int main(void) 
{
	signal(SIGINT, sigint_handler);
	wordle_server.init();
}

void sigint_handler(int signal) 
{
	std::cout << std::endl;
	_logf("server", "received signal %d, exiting\n", signal);
	
	exit(EXIT_SUCCESS);
}
