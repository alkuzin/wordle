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
	//server_addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
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
	log("server", "shutdown");
}

void Server::init(void)
{
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		throw SOCKET_CREATION_EXCEPTION;	

	log("server", "socket creation successfull");	

	// Display server ip address and port
	char ip_addr_buffer[INET_ADDRSTRLEN];

	// Convert IP address to string
	inet_ntop(AF_INET, &(server_addr.sin_addr.s_addr), ip_addr_buffer, INET_ADDRSTRLEN);
	ip_addr_buffer[INET_ADDRSTRLEN] = '\0';
	logf("server", "ip: %s port: %hu\n", ip_addr_buffer, server_addr.sin_port);

	_bind();

	while(true)
		_handle_client();
}

void Server::_bind(void) 
{
	if((bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr))) < 0)
		throw BIND_EXCEPTION;	

	log("server", "bind successfull");
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
	
	logf("server", "connected client <client> [ip: %s port: %hu]\n", ip_addr_buffer,
		 client_addr.sin_port);
		
	game.update_hidden_word();
	std::memset(hidden_word, 0, sizeof(hidden_word));
	std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);
	hidden_word[WORD_LENGTH] = '\0';
	
	logf("server", "hidden word: '%s'\n", hidden_word);

	while(game.get_attempts()) {
		std::memset(message, 0, sizeof(message));
		std::memset(letters, 0, sizeof(letters));
		std::memset(bytes,   0, sizeof(bytes));
	
		log("server", "waiting client response");
		recvfrom(sockfd, message, sizeof(message),
				 MSG_WAITALL, (struct sockaddr *)&client_addr, &client_addr_len);
		message[WORD_LENGTH] = '\0';
		
		logf("server", "<client>: %s\n", message);
		log("server", "processing word");
		game.process_input(message, letters);
		result = game.is_guessed(letters);
		
		logf("server", "%s", "converting bool array to bytes: ");
		convert_to_bytes(letters, bytes, sizeof(bytes));
		
		// display bool array of guesssed letters
		putchar('{');
		for(int i = 0; i < WORD_LENGTH; i++) {
			printf(" %d", bytes[i]);
			if(i < WORD_LENGTH - 1)
				putchar(',');
		}
		putchar(' ');
		putchar('}');
		putchar('\n');
		
		log("server", "sending bytes array to client");
		int sent_bytes = sendto(sockfd, bytes, sizeof(bytes),
		MSG_CONFIRM, (struct sockaddr *)&client_addr, sizeof(client_addr));

		logf("server", "sent %d bytes to client\n", sent_bytes);
	
		if(result) {
			log("server", "client finished game");
			std::cout << std::endl;
			log("server", "setting new hidden word");
			
			game.update_hidden_word();
			std::memset(hidden_word, 0, sizeof(hidden_word));
			std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);
			hidden_word[WORD_LENGTH] = '\0';
			logf("server", "hidden word: '%s'\n", hidden_word);
			continue;
		}
		game.decrement_attempts();
	}
	log("server", "reset attempts");
	game.reset_attempts();
}
