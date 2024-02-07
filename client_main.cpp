/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/wordle_client.h"
#include <signal.h>

void sigint_handler(int signal);

enum transfer_mode mode = UDP;
Wordle_Client wordle_client(mode);
	
int main(void) 
{
	signal(SIGINT, sigint_handler);
	wordle_client.init();
}

void sigint_handler(int signal) 
{
	std::cout << std::endl;
	_logf("client", "received signal %d, exiting\n", signal);
	
	exit(EXIT_SUCCESS);
}
