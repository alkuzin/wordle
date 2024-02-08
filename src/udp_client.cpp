/*
 * udp_client.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/udp_client.h"

UDP_Client::UDP_Client(void)
{
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(DEFAULT_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

void UDP_Client::_shutdown(void) {
	close(sockfd);	
}

UDP_Client::~UDP_Client(void) {
	_shutdown();
}

void UDP_Client::begin(void) {
	std::cout << "Try to guess word with " << WORD_LENGTH << " symbols" << std::endl;
}

void UDP_Client::end(void) {
	std::cout << "Try again\n" << std::endl;
}

int UDP_Client::get_socket(void) {
	return sockfd;
}

void UDP_Client::init(void)
{
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		throw SOCKET_CREATION_EXCEPTION;
}

void UDP_Client::recv(char *message, size_t size) 
{	
	u32  server_addr_len;

	recvfrom(sockfd, message, size, MSG_WAITALL,
	(struct sockaddr *)&server_addr, &server_addr_len);
}

void UDP_Client::send(char *message, size_t size) 
{	
	sendto(sockfd, message, size, MSG_CONFIRM,
	(struct sockaddr *)&server_addr, sizeof(server_addr));
}
