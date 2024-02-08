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

#define DEFAULT_CLIENT_NAME  "client"
#define CLIENT_NAME_SIZE     256


class Wordle_Client 
{
	char   client_name[CLIENT_NAME_SIZE];
	enum   transfer_mode mode;
	Client *client;
	UI     ui;

	public:
		Wordle_Client(enum transfer_mode mode);
		~Wordle_Client(void);
		
		void init(void);
	
	private:
		void _set_client_name(void);
		void _init_udp(void);
		void _init_ipc(void);
		void _process(void);
		void _shutdown(void);
};

#endif // WORDLE_CLIENT_H
