/*
 * client.h
 * @alkuzin - 2024
 *
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

class Client 
{
	public:
		virtual ~Client() = default;
		void virtual init(void) = 0;
		void virtual recv(char *message, size_t size) = 0;
		void virtual send(char *message, size_t size) = 0;
		void virtual begin(void) = 0;
		void virtual end(void) = 0;
	
	protected:
		void virtual _shutdown(void) = 0;
};

#endif // CLIENT_H
