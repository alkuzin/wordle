/*
 * wordle_server.h
 * @alkuzin - 2024
 *
 */

#ifndef WORDLE_SERVER_H
#define WORDLE_SERVER_H

#include "udp_server.h"
#include "ipc_server.h"
#include "utils.h"

enum transfer_mode {
	UDP,
	IPC
};

class Wordle_Server 
{
	enum   transfer_mode mode;
	Server *server;

	public:
		Wordle_Server(enum transfer_mode mode);
		~Wordle_Server(void);
		
		void init(void);
	
	private:
		void _init_udp(void);
		void _init_ipc(void);
		void _shutdown(void);
};

#endif // WORDLE_SERVER_H
