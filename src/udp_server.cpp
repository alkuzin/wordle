/*
 * udp_server.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/udp_server.h"

UDP_Server::UDP_Server(void) : Server()
{
	std::memset(&server_addr, 0, sizeof(server_addr));
	std::memset(&client_addr, 0, sizeof(client_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = htons(DEFAULT_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
}

void UDP_Server::_shutdown(void)
{
	close(sockfd);	
	_log("server", "shutdown");
}

UDP_Server::~UDP_Server(void) {
	_shutdown();
}

int UDP_Server::get_socket(void) {
	return sockfd;
}

void UDP_Server::_bind(void) 
{
	if((bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0)
		throw BIND_EXCEPTION;	

	_log("server", "bind successfull");
}

void UDP_Server::_show_server_info(void) 
{
	// Display server ip address and port
	char ip_addr[INET_ADDRSTRLEN];

	// Convert IP address to string
	inet_ntop(AF_INET, &(server_addr.sin_addr.s_addr), ip_addr, INET_ADDRSTRLEN);
	_logf("server", "ip: %s port: %u\n", ip_addr, server_addr.sin_port);
}

void UDP_Server::init(void)
{
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		throw SOCKET_CREATION_EXCEPTION;	

	_log("server", "socket creation successfull");	

	_show_server_info();
	_bind();

	while(true) {
		_log("server", "--- handle client ---");	
		_handle_client();
		std::cout << std::endl;
	}
}

void UDP_Server::_show_client_info(void)
{
	char ip_addr[INET_ADDRSTRLEN];
	
	_get_client_ip(ip_addr);
	std::cout << std::endl;
	_logf("server", "receiving message from client (ip: %s port: %u)\n", ip_addr,
	client_addr.sin_port);
}

void UDP_Server::_get_client_ip(char *buffer) {
	inet_ntop(AF_INET, &(client_addr.sin_addr.s_addr), buffer, INET_ADDRSTRLEN);
}

void UDP_Server::recv(char *message, size_t size) 
{	
	u32  client_addr_len;
	int received_bytes;

	received_bytes = recvfrom(sockfd, message, size, MSG_WAITALL,
	(struct sockaddr *)&client_addr, &client_addr_len);
	_logf("server", "received %d bytes from client\n", received_bytes);	
}

void UDP_Server::send(char *message, size_t size) 
{	
	int sent_bytes;
		
	sent_bytes = sendto(sockfd, message, size, MSG_CONFIRM,
	(struct sockaddr *)&client_addr, sizeof(client_addr));
	_logf("server", "sent %d bytes to client\n", sent_bytes);	
}

void UDP_Server::_handle_client(void)
{
	char attempts_bytes[UTOA_SIZE];
	char hidden_word[WORD_LENGTH + 1];
	char message[WORD_LENGTH + 1];
	char bytes[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	u32  attempts;

	// set hidden word
	game.update_hidden_word();
	std::memset(hidden_word, 0, sizeof(hidden_word));
	std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);
	_logf("server", "set hidden word: (%s)\n", hidden_word);	

	attempts = ATTEMPTS_LIMIT;
	_logf("server", "set attempts limit: (%u)\n", attempts);	

	do {
		std::memset(attempts_bytes, 0, sizeof(attempts_bytes));
		std::memset(&client_addr, 0, sizeof(client_addr));
		std::memset(message, 0, sizeof(message));
		std::memset(letters, 0, sizeof(letters));
		std::memset(bytes,   0, sizeof(bytes));

		attempts = game.get_attempts();
		_utoa(attempts, attempts_bytes);
		
		recv(message, sizeof(message));
		message[WORD_LENGTH] = '\0';

		_show_client_info();
		_logf("server", "client send message: (%s)\n", message);
		
		game.process_input(message, letters);
		
		_logf("server", "%s", "converting bool array to bytes: ");
		_convert_to_bytes(letters, bytes, WORD_LENGTH);

		// display bool array of guesssed letters
		_display_bytes(bytes, WORD_LENGTH);	
		send(bytes, sizeof(bytes));

		_logf("server", "sending the number of attempts left to client: (%s)\n", attempts_bytes);
		send(attempts_bytes, sizeof(attempts_bytes));
		
		if(game.is_guessed(letters))
			break;

		game.decrement_attempts();
		_logf("server", "(attempts left: %u)\n", game.get_attempts());

	} while(game.get_attempts());
	
	game.set_attempts(ATTEMPTS_LIMIT);
	_log("server", "--- client finished game ---");
}
