/*
 * server.h
 * @alkuzin - 2024
 *
 */

#ifndef SERVER_H
#define SERVER_H

#include <iostream>

class Server 
{
	public:
		virtual ~Server() = default;
		void virtual init(void) = 0;
		void virtual recv(char *message, size_t size) = 0;
		void virtual send(char *message, size_t size) = 0;
		void virtual begin(void) = 0;
		void virtual end(void) = 0;
	
	protected:
		void virtual _shutdown(void) = 0;
};

#endif // SERVER_H
