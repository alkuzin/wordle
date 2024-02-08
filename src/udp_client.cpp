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

/*
void UDP_Client::_handle_server(void)
{
	char attempts_bytes[ATTEMPTS_BYTES_SIZE];
	char message[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	char bytes[WORD_LENGTH + 1];
		
	system("clear");
	ui.display_banner();

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
		send(message, sizeof(message));

		// receiving bytes array
		recv(bytes, sizeof(bytes));
		bytes[WORD_LENGTH] = '\0';
		
		// converting bytes array to bool array
		_convert_to_bool(bytes, letters, sizeof(letters));
		
		//receiving number of attempts left
		recv(attempts_bytes, sizeof(attempts_bytes));
		attempts_bytes[ATTEMPTS_BYTES_SIZE] = '\0';
		
		// update attempts
		ui.set_attempts(std::atoi(attempts_bytes));
		
		ui.display_word(message, letters);
		
		if(ui.is_guessed(letters))
			break;
		
		ui.decrement_attempts();

	} while(ui.get_attempts());
	
	ui.display_result(letters);
}*/
