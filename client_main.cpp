/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/wordle_client.h"
#include <signal.h>

void sigint_handler(int signal);

#ifdef UDP_MODE
	enum transfer_mode mode = UDP;
#elif defined IPC_MODE
	enum transfer_mode mode = IPC;
#else
	enum transfer_mode mode = IPC;
#endif

// IPC - default mode
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
