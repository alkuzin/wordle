/*
 * server.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/server.h"

Server::Server(void)
{
	std::memset(&server_addr, 0, sizeof(server_addr));
	std::memset(&client_addr, 0, sizeof(client_addr));
	
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = DEFAULT_PORT;
	server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP_ADDRESS);
}

Server::Server(const char *ip_addr, u16 port, const char *wordlist_path)
{
	// validate IP server_addr
	struct sockaddr_in addr;

	if((inet_pton(AF_INET, ip_addr, &(addr.sin_addr)) == 0))
		throw INVALID_IP_ADDRESS_EXCEPTION;	

	// validate port
	if(port <= 1024)
		throw FORBIDDEN_PORT_EXCEPTION;	
	
	std::memset(&server_addr, 0, sizeof(server_addr));
	std::memset(&client_addr, 0, sizeof(client_addr));
	
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = port;
	server_addr.sin_addr.s_addr = inet_addr(ip_addr);
	
	game.set_wordlist_path(wordlist_path);
}

Server::~Server(void) 
{
	close(sockfd);
	shutdown(sockfd, SHUT_RDWR);
	
	_log("server", "shutdown");
}

void Server::init(void)
{
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		throw SOCKET_CREATION_EXCEPTION;	

	_log("server", "socket creation successfull");	

	// Display server ip address and port
	char ip_addr_buffer[INET_ADDRSTRLEN];

	// Convert IP address to string
	inet_ntop(AF_INET, &(server_addr.sin_addr.s_addr), ip_addr_buffer, INET_ADDRSTRLEN);
	_logf("server", "ip: %s port: %hu\n", ip_addr_buffer, server_addr.sin_port);

	_bind();

	while(true)
		_handle_client();
}

void Server::_bind(void) 
{
	if((bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0)
		throw BIND_EXCEPTION;	

	_log("server", "bind successfull");
}

void Server::_handle_client(void)
{
	char ip_addr_buffer[INET_ADDRSTRLEN];
	char hidden_word[WORD_LENGTH + 1];
	char message[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	u8   bytes[WORD_LENGTH + 1];
	u32  client_addr_len;
	bool result;
	
	// show client information
	inet_ntop(AF_INET, &(client_addr.sin_addr.s_addr), ip_addr_buffer, INET_ADDRSTRLEN);
	
	game.update_hidden_word();
	std::memset(hidden_word, 0, sizeof(hidden_word));
	std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);
	
	u32 attempts;
	_logf("server", "hidden word: '%s'\n", hidden_word);
	char attempts_bytes[32];
	
	attempts = ATTEMPTS_LIMIT; 
	while((attempts = game.get_attempts())) {
		std::memset(message, 0, sizeof(message));
		std::memset(letters, 0, sizeof(letters));
		std::memset(bytes,   0, sizeof(bytes));
		
		snprintf(attempts_bytes, sizeof(attempts_bytes), "%u", attempts);
		attempts_bytes[32] = '\0'; 
		_logf("server", "attempts left: %u\n", attempts);
		
		_log("server", "waiting client response");
		recvfrom(sockfd, message, sizeof(message),
				 MSG_WAITALL, (struct sockaddr *)&client_addr, &client_addr_len);
		
		_logf("server", "receiving message from client [ip: %s port: %hu]\n", ip_addr_buffer,
			  client_addr.sin_port);
		_logf("server", "<client>: %s\n", message);
		
		_log("server", "processing word");
		game.process_input(message, letters);
		result = game.is_guessed(letters);
		
		_logf("server", "%s", "converting bool array to bytes: ");
		_convert_to_bytes(letters, bytes, sizeof(bytes));
	
		// display bool array of guesssed letters
		_display_bytes(bytes, WORD_LENGTH);	

		_log("server", "sending bytes array to client");
		sendto(sockfd, bytes, sizeof(bytes),
		MSG_CONFIRM, (struct sockaddr *)&client_addr, sizeof(client_addr));

		sendto(sockfd, attempts_bytes, sizeof(attempts_bytes), MSG_CONFIRM,
			   (struct sockaddr *)&client_addr, sizeof(client_addr));
		
		if(result) {
			_log("server", "client finished game");
			std::cout << std::endl;
			
			_log("server", "setting new hidden word");
			
			game.update_hidden_word();
			std::memset(hidden_word, 0, sizeof(hidden_word));
			std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);

			_logf("server", "hidden word: '%s'\n", hidden_word);

			_log("server", "reset attempts");
			game.set_attempts(ATTEMPTS_LIMIT);
			continue;
		}
		game.decrement_attempts();
	}
}
