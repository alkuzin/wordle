/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/wordle_server.h"
#include <signal.h>

void sigint_handler(int signal);

#ifdef UDP_MODE
	enum transfer_mode mode = UDP;
#elif defined IPC_MODE
	enum transfer_mode mode = IPC;
#else
	enum transfer_mode mode = IPC;
#endif

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
