/*
 * wordle_client.h
 * @alkuzin - 2024
 *
 */

#ifndef WORDLE_CLIENT_H
#define WORDLE_CLIENT_H

#include "udp_client.h"
#include "ipc_client.h"

enum transfer_mode {
	UDP,
	IPC
};

class Wordle_Client 
{
	enum   transfer_mode mode;
	Client *client;

	public:
		Wordle_Client(enum transfer_mode mode);
		~Wordle_Client(void);
		
		void init(void);
	
	private:
		void _init_udp(void);
		void _init_ipc(void);
		void _shutdown(void);
};

#endif // WORDLE_CLIENT_H
