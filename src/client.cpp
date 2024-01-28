/*
 * client.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/client.h"

Client::Client(void)
{
	std::memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = DEFAULT_PORT;
	client_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP_ADDRESS);
	addr_len = sizeof(client_addr);
	client_fd = -1;
}

Client::Client(int client_fd)
{
	this->client_fd = client_fd;
	std::memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = DEFAULT_PORT;
	client_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP_ADDRESS);
	addr_len = sizeof(client_addr);
}

Client::Client(const char *ip_addr, u16 port)
{
	// validate IP address
	struct sockaddr_in addr;
	
	if((inet_pton(AF_INET, ip_addr, &(addr.sin_addr)) == 0))
		error("this ip client_addr is invalid");

	// validate port
	if(port <= 1024)
		error("this port is not allowed");

	std::memset(&client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = port;
	client_addr.sin_addr.s_addr = inet_addr(ip_addr);
	addr_len = sizeof(client_addr);
	
	client_fd = -1;
}

Client::~Client(void) 
{
	close(client_fd);
	log("client", "exit");
}

void Client::init(void)
{
	if(client_fd == -1) {
		if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
			error("socket failed");
	}

	log("client", "initialization successfull");	
	_connect();
}

void Client::_connect(void)
{
	
	if((connect(client_fd, (sockaddr *)&client_addr, addr_len)) < 0)
		error("connect error");

	log("client", "connection successfull");
	
	// Display server ip address and port
	char buffer[64];
	
	// Convert IP address to string
	char ip_addr_buffer[INET_ADDRSTRLEN];
	struct in_addr addr;
	
	addr.s_addr = client_addr.sin_addr.s_addr;
	inet_ntop(AF_INET, &(addr.s_addr), ip_addr_buffer, INET_ADDRSTRLEN);

	snprintf(buffer, sizeof(buffer), "connected to server [ip: %s port: %hu]",
			ip_addr_buffer, client_addr.sin_port);
	log("client", buffer);

	handle_server(client_fd);
}

void Client::handle_server(int server_fd)
{
	char buffer[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	u8 bytes[WORD_LENGTH + 1];
	char hidden_word[WORD_LENGTH + 1];

	while(ui.get_attempts()) {
		std::memset(buffer, 0, sizeof(buffer));
		std::memset(letters, 0, sizeof(letters));
		std::memset(bytes, 0, sizeof(bytes));

		system("clear");
		ui.display();

		printf("<client>: ");
		getinput(buffer, sizeof(buffer));

		if(buffer[0] == '\0')
			continue;
		
		//log("client", "sending word");
		send(server_fd, buffer, sizeof(buffer), 0);
		
		//log("client", "receiving bytes array");
		recv(server_fd, bytes, sizeof(bytes), 0);	

		//log("client", "converting bytes array to bool array");
		convert_to_bool(bytes, letters, sizeof(letters));
		
		//log("client", "saving current state");
		ui.save_state(buffer, letters);

		if(ui.is_guessed())
			break;

		if(std::strncmp("exit", buffer, 4) == 0)
			break;

		ui.decrement_attempts();
	}

	system("clear");
	ui.decrement_attempts();
	ui.display();

	std::strncpy(buffer, "finish", sizeof(buffer));
	send(server_fd, buffer, sizeof(buffer), 0);
	
	// receive hidden word after the game was finished
	std::memset(hidden_word, 0, WORD_LENGTH);
	recv(server_fd, hidden_word, sizeof(hidden_word), 0);
	ui.display_result(hidden_word);
}
