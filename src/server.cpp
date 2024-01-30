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
	//server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP_ADDRESS);
	server_addr.sin_addr.s_addr = INADDR_ANY;
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

	#define ATTEMPTS_BYTES_SIZE 12
	char attempts_bytes[ATTEMPTS_BYTES_SIZE];
	
	char bytes[WORD_LENGTH + 1];
	u32  client_addr_len;
	u32  attempts;
	bool result;
	
	// show client information
	inet_ntop(AF_INET, &(client_addr.sin_addr.s_addr), ip_addr_buffer, INET_ADDRSTRLEN);

	// set hidden word
	game.update_hidden_word();
	std::memset(hidden_word, 0, sizeof(hidden_word));
	std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);
	ip_addr_buffer[INET_ADDRSTRLEN] = '\0';	
	_logf("server", "receiving message from client [ip: %s port: %hu]\n", ip_addr_buffer,
		  client_addr.sin_port);
	
	_logf("server", "hidden word: '%s'\n", hidden_word);	

	int received_bytes;
	int sent_array_bytes;
	int sent_attempts_bytes;
	
	while((attempts = game.get_attempts())) {
		std::memset(attempts_bytes, 0, sizeof(attempts_bytes));
		std::memset(bytes,   0, sizeof(bytes));
		std::memset(message, 0, sizeof(message));
		std::memset(letters, 0, sizeof(letters));
		
		_logf("server", "set attempts limit: '%hu'\n", attempts);	
		snprintf(attempts_bytes, sizeof(attempts_bytes), "%u", attempts);
		attempts_bytes[ATTEMPTS_BYTES_SIZE] = '\0';
		
		std::cout << "ATTEMPTS BYTES: " << attempts_bytes << std::endl;
		_logf("server", "attempts left: %u\n", attempts);
		
		_log("server", "waiting client response");
		received_bytes = recvfrom(sockfd, message, sizeof(message),
				 MSG_WAITALL, (struct sockaddr *)&client_addr, &client_addr_len);
		message[WORD_LENGTH] = '\0';
		
		std::cout << "ATTEMPTS BYTES AFTER RECEIVING: " << attempts_bytes << std::endl;
	
		// show client information
		inet_ntop(AF_INET, &(client_addr.sin_addr.s_addr), ip_addr_buffer, INET_ADDRSTRLEN);
		ip_addr_buffer[INET_ADDRSTRLEN] = '\0';	
		_logf("server", "receiving message from client [ip: %s port: %hu]\n", ip_addr_buffer,
			  client_addr.sin_port);
		
		_logf("server", "received %d bytes from client\n", received_bytes);	
		_logf("server", "<client>: %s\n", message);
		
		_log("server", "processing word");
		game.process_input(message, letters);
		result = game.is_guessed(letters);
		
		std::cout << "ATTEMPTS BYTES AFTER WORD PROCESSING: " << attempts_bytes << std::endl;
		
		_logf("server", "current result: %d\n", result);	
		
		_logf("server", "%s", "converting bool array to bytes: ");
		//_convert_to_bytes(letters, bytes, WORD_LENGTH);
		
		u32 i;
 
     	i = 0;
     	while(i < WORD_LENGTH) {
         	bytes[i] = letters[i];
         	i++;
    	}

		bytes[WORD_LENGTH] = '\0';
		

		// display bool array of guesssed letters
		_display_bytes(bytes, WORD_LENGTH);	
		
		std::cout << "\nATTEMPTS BYTES AFTER CONVERTING TO BYTES: " << attempts_bytes << std::endl;

		_log("server", "sending bytes array to client");
		sent_array_bytes = sendto(sockfd, bytes, sizeof(bytes), MSG_CONFIRM,
			  (struct sockaddr *)&client_addr, sizeof(client_addr));
		_logf("server", "sent %d bytes of letters to client\n", sent_array_bytes);	
	
		_logf("server", "sending the number of attempts left to client: '%s'\n", attempts_bytes);
		sent_attempts_bytes = sendto(sockfd, attempts_bytes, sizeof(attempts_bytes), MSG_CONFIRM,
			  (struct sockaddr *)&client_addr, sizeof(client_addr));
		_logf("server", "sent %d bytes of attempts to client\n", sent_attempts_bytes);	
		_logf("server", "attempts left: %u\n", attempts);
		
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
