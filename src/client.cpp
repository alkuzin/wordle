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
	char message[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	u8   bytes[WORD_LENGTH + 1];
	u32  server_addr_len;
		
	system("clear");
	ui.display_banner();

	while(ui.get_attempts()) {
		std::memset(letters, 0, sizeof(letters));
		std::memset(message, 0, sizeof(message));
		std::memset(bytes,   0, sizeof(bytes));

		printf("<client>: ");
		getinput(message, sizeof(message));
		message[WORD_LENGTH] = '\0';

		if(message[0] == '\0')
			continue;
	
		//logf("client", "sending word '%s'\n", message);
		// sending word
		sendto(sockfd, message, sizeof(message), MSG_CONFIRM,
			  (struct sockaddr *)&server_addr, sizeof(server_addr));
		
		//logf("client", "sent %d bytes \n", sent_bytes);

		// receiving bytes array
		//log("client", "receiving bytes array");
		recvfrom(sockfd, bytes, sizeof(bytes), MSG_WAITALL,
		        (struct sockaddr *)&server_addr, &server_addr_len);
		bytes[WORD_LENGTH] = '\0';
			
		//logf("client", "received %d bytes \n", recv_bytes);
		//log("client", "received bytes array");
		/*
		for(int i = 0; i < WORD_LENGTH; i++)
             printf(" %02x", bytes[i]);
        putchar('\n');
		*/

		// converting bytes array to bool array
		convert_to_bool(bytes, letters, sizeof(letters));
		
		ui.display_word(message, letters);
		
		if(ui.is_guessed(letters))
			break;

		ui.decrement_attempts();
	}
	//system("clear");
	//ui.display();
	ui.display_result(letters);
}
