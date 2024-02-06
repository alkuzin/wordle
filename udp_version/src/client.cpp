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
	server_addr.sin_port        = htons(DEFAULT_PORT);
	server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	
	_set_client_name();
}

void Client::_set_client_name(void)
{
	std::memset(client_name, 0, sizeof(client_name));
	
	if (getlogin_r(client_name, sizeof(client_name)) != 0)
    	std::strncpy(client_name, DEFAULT_CLIENT_NAME, sizeof(client_name));
}

int Client::get_socket(void) {
	return sockfd;
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

	// send client invitation message 
	std::memset(message, 0, sizeof(message));	
	std::strncpy(message, CLIENT_INVITATION, WORD_LENGTH + 1);
	sendto(sockfd, message, sizeof(message), MSG_CONFIRM,
		  (struct sockaddr *)&server_addr, sizeof(server_addr));

	do {
		std::memset(attempts_bytes, 0, sizeof(attempts_bytes));
		std::memset(letters, 0, sizeof(letters));
		std::memset(message, 0, sizeof(message));
		std::memset(bytes,   0, sizeof(bytes));

		// get user input
		std::cout << "<" << client_name << ">: ";
		_getinput(message, sizeof(message));
		message[WORD_LENGTH] = '\0';

		if(message[0] == '\0')
			continue;
	
		// sending word
		sendto(sockfd, message, sizeof(message), MSG_CONFIRM,
			  (struct sockaddr *)&server_addr, sizeof(server_addr));
		
		// receiving bytes array
		recvfrom(sockfd, bytes, sizeof(bytes), MSG_WAITALL,
		        (struct sockaddr *)&server_addr, &server_addr_len);
		bytes[WORD_LENGTH] = '\0';
		
		// converting bytes array to bool array
		_convert_to_bool(bytes, letters, sizeof(letters));
		
		//receiving number of attempts left
		recvfrom(sockfd, attempts_bytes, sizeof(attempts_bytes), MSG_WAITALL,
		        (struct sockaddr *)&server_addr, &server_addr_len);
		attempts_bytes[ATTEMPTS_BYTES_SIZE] = '\0';
		
		// update attempts
		ui.set_attempts(std::atoi(attempts_bytes));
		
		ui.display_word(message, letters);
		
		if(ui.is_guessed(letters))
			break;
		
		ui.decrement_attempts();

	} while(ui.get_attempts());
	
	ui.display_result(letters);
}
