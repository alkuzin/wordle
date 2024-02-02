/*
 * client.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/client.h"

Client::Client(void)
{
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = DEFAULT_PORT;
	server_addr.sin_addr.s_addr = inet_addr(DEFAULT_IP_ADDRESS);
	
	_set_client_name();
}

void Client::_set_client_name(void)
{
	std::memset(client_name, 0, sizeof(client_name));
	
	if (getlogin_r(client_name, sizeof(client_name)) != 0)
    	std::strncpy(client_name, DEFAULT_CLIENT_NAME, sizeof(client_name));
}

Client::Client(const char *ip_addr, u16 port)
{
	// validate IP address
	struct sockaddr_in addr;
	
	if((inet_pton(AF_INET, ip_addr, &(addr.sin_addr)) == 0))
		throw INVALID_IP_ADDRESS_EXCEPTION;

	// validate port
	if(port <= 1024)
		throw FORBIDDEN_PORT_EXCEPTION;

	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family      = AF_INET;
	server_addr.sin_port        = port;
	server_addr.sin_addr.s_addr = inet_addr(ip_addr);
	
	_set_client_name();
}

Client::~Client(void) {
	close(sockfd);
}

void Client::init(void)
{
	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
		throw SOCKET_CREATION_EXCEPTION;

	_handle_server();
}

void Client::_handle_server(void)
{
	char attempts_bytes[ATTEMPTS_BYTES_SIZE];
	char message[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	char bytes[WORD_LENGTH + 1];
	u32  server_addr_len;
		
	system("clear");
	ui.display_banner();
	
	char ip_addr[INET_ADDRSTRLEN];

	std::memset(ip_addr, 0, sizeof(ip_addr));	
	inet_ntop(AF_INET, &(server_addr.sin_addr.s_addr), ip_addr, INET_ADDRSTRLEN);
	ip_addr[INET_ADDRSTRLEN] = '\0';

	_logf("client", "connected to server[ip: %s port: %hu]\n", ip_addr,
           server_addr.sin_port);

	//_logf("client", "attempts set: %u\n", ui.get_attempts());
	std::memset(message, 0, sizeof(message));	
	std::strncpy(message, CLIENT_INVITATION, WORD_LENGTH + 1);
	sendto(sockfd, message, sizeof(message), MSG_CONFIRM,
		  (struct sockaddr *)&server_addr, sizeof(server_addr));
	//_logf("client", "sent %d bytes\n", sent_bytes);

	do {
		std::memset(attempts_bytes, 0, sizeof(attempts_bytes));
		std::memset(letters, 0, sizeof(letters));
		std::memset(message, 0, sizeof(message));
		std::memset(bytes,   0, sizeof(bytes));

		printf("<%s>: ", client_name);
		_getinput(message, sizeof(message));
		message[WORD_LENGTH] = '\0';

		if(message[0] == '\0')
			continue;
	
		//_logf("client", "sending message: (%s)\n", message);
		// sending word
		sendto(sockfd, message, sizeof(message), MSG_CONFIRM,
			  (struct sockaddr *)&server_addr, sizeof(server_addr));
		
		//_logf("client", "sent %d bytes\n", sent_bytes);
		
		// receiving bytes array
		recvfrom(sockfd, bytes, sizeof(bytes), MSG_WAITALL,
		        (struct sockaddr *)&server_addr, &server_addr_len);
		bytes[WORD_LENGTH] = '\0';
		
		//_logf("client", "received %d bytes of array\n", received_array_bytes);
		
		// converting bytes array to bool array
		_convert_to_bool(bytes, letters, sizeof(letters));
		
		//receiving number of attempts left
		recvfrom(sockfd, attempts_bytes, sizeof(attempts_bytes), MSG_WAITALL,
		        (struct sockaddr *)&server_addr, &server_addr_len);
		attempts_bytes[ATTEMPTS_BYTES_SIZE] = '\0';
		
		//_logf("client", "received %d bytes of attempts\n", received_attempts_bytes);
		//_logf("client", "bytes of attempts '%s'\n", attempts_bytes);
		ui.set_attempts(std::atoi(attempts_bytes));
		
		//_logf("client", "attempts left: %u\n", ui.get_attempts());
		
		ui.display_word(message, letters);
		
		if(ui.is_guessed(letters))
			break;
		
		ui.decrement_attempts();

	} while(ui.get_attempts());
	
	ui.display_result(letters);
}
