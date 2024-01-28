/*
 * server.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/server.h"

Server::Server(void)
{
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = DEFAULT_PORT;
	server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP_ADDRESS);
	addr_len = sizeof(server_addr);

	log("server", "default constructor");	
}

Server::Server(const char *ip_addr, u16 port)
{
	// validate IP server_addr
	struct sockaddr_in addr;

	if((inet_pton(AF_INET, ip_addr, &(addr.sin_addr)) == 0))
		error("this ip server_addr is invalid");

	// validate port
	if(port <= 1024)
		error("this port is not allowed");

	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
	server_addr.sin_addr.s_addr = inet_addr(ip_addr);
	addr_len = sizeof(server_addr);
	
	log("server", "custom constructor");	
}

Server::~Server(void) 
{
	close(server_fd);
	shutdown(server_fd, SHUT_RDWR);

	log("server", "shutdown");
}

void Server::init(void)
{
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		error("socket failed");

	log("server", "initialization successfull");	

	// Display server ip address and port
	char buffer[64];
	
	// Convert IP address to string
	char ip_addr_buffer[INET_ADDRSTRLEN];
	struct in_addr addr;
	
	addr.s_addr = server_addr.sin_addr.s_addr;
	inet_ntop(AF_INET, &(addr.s_addr), ip_addr_buffer, INET_ADDRSTRLEN);

	snprintf(buffer, sizeof(buffer), "ip: %s port: %hu", ip_addr_buffer, server_addr.sin_port);
	log("server", buffer);

	_bind();
	_listen();
}

void Server::_bind(void) 
{
	if((bind(server_fd, (struct sockaddr *)&server_addr, addr_len)) < 0)
		error("bind error");		

	log("server", "bind successfull");
}

void Server::_listen(void) 
{
	int new_fd;
	char buffer[INPUT_SIZE + 1];
	char client_info[128];
	char client_ip_addr_buffer[INET_ADDRSTRLEN];

	struct sockaddr_in client_addr;
	u32 client_addr_len;


	memset(buffer, 0, INPUT_SIZE);
	log("server", "listening...");

	do {

		if((listen(server_fd, CLIENT_LIMIT)) < 0) {
			error("listen error");		
		}

		if((new_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len)) < 0) {
			error("accept error");		
		}

		memset(client_info, 0, 128);
		memset(&client_addr, 0, sizeof(client_addr));
		
		inet_ntop(AF_INET, &(client_addr.sin_addr.s_addr), client_ip_addr_buffer, INET_ADDRSTRLEN);
		snprintf(client_info, 128, "connected client <client-%d> [ip: %s port: %hu]",
				 new_fd, client_ip_addr_buffer, client_addr.sin_port);
		log("server", client_info);

		handle_client(new_fd);	
	} 
	while(true);
}

void Server::handle_client(int client_fd)
{
	char client_buffer[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	char log_buffer[128];
	char hidden_word[WORD_LENGTH + 1];
	u8 bytes[WORD_LENGTH + 1];
	Client client(client_fd); 
	
	game.set_word();
	std::memset(hidden_word, 0, sizeof(hidden_word));
	std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);
	hidden_word[WORD_LENGTH] = '\0';

	std::cout << "HIDDEN_WORD: " << hidden_word << std::endl; 
	while(true) {
		std::memset(log_buffer, 0, sizeof(log_buffer));
		std::memset(client_buffer, 0, sizeof(client_buffer));
		std::memset(letters, 0, sizeof(letters));
		std::memset(bytes, 0, sizeof(bytes));

		log("server", "receiving word from client");
		recv(client_fd, client_buffer, sizeof(client_buffer), 0);
		client_buffer[WORD_LENGTH] = '\0';
		
		snprintf(log_buffer, sizeof(log_buffer), "<client-%d>: %s", client_fd, client_buffer);
		log("server", log_buffer);
		
		if(std::strncmp("exit", client_buffer, 4) == 0) {
			log("server", "client quit");
			break;
		}
		
		if(std::strncmp("finish", client_buffer, WORD_LENGTH) == 0) {
			// send client hidden word
			std::memset(hidden_word, 0, sizeof(hidden_word));
			std::strncpy(hidden_word, game.get_hidden_word(), WORD_LENGTH);
			hidden_word[WORD_LENGTH] = '\0';
			send(client_fd, hidden_word, sizeof(hidden_word), 0);
			
			log("server", "client finished playing");
			break;
		}
		
		//if(client_buffer[0] == '\0')
		//	continue;

		log("server", "processing word");
		game.process_input(client_buffer, letters);
		
		log("server", "converting bool array to bytes");
		convert_to_bytes(letters, bytes, sizeof(bytes));

		
		for(int i = 0; i < WORD_LENGTH; i++)
			printf(" %02x", bytes[i]);
		putchar('\n');

		log("server", "sending bytes array to client");
		send(client_fd, bytes, sizeof(bytes), 0);
	}
}
