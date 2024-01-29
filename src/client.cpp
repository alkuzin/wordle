/*
 * client.cpp
 * @alkuzin - 2024
 *
 */

#include "../include/client.h"

Client::Client(void)
{
	std::memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = DEFAULT_PORT;
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
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = port;
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
	char buffer[WORD_LENGTH + 1];
	bool letters[WORD_LENGTH];
	u8 bytes[WORD_LENGTH + 1];
	u32 server_addr_len;

	while(ui.get_attempts()) {
		std::memset(letters, 0, sizeof(letters));
		std::memset(buffer, 0, sizeof(buffer));
		std::memset(bytes, 0, sizeof(bytes));

		system("clear");
		ui.display();

		printf("<client>: ");
		getinput(buffer, sizeof(buffer));
		buffer[WORD_LENGTH] = '\0';

		if(buffer[0] == '\0')
			continue;
	
		logf("client", "sending word '%s'\n", buffer);
		// sending word
		int sent_bytes = sendto(sockfd, buffer, sizeof(buffer), MSG_CONFIRM,
			  (struct sockaddr *)&server_addr, sizeof(server_addr));
		
		if(std::strncmp(EXIT_CODE, buffer, 5) == 0)
			break;
		
		logf("client", "sent %d bytes \n", sent_bytes);

		// receiving bytes array
		log("client", "receiving bytes array");
		int recv_bytes = recvfrom(sockfd, bytes, sizeof(bytes), MSG_WAITALL,
		    (struct sockaddr *)&server_addr, &server_addr_len);
		bytes[WORD_LENGTH] = '\0';

		logf("client", "received %d bytes \n", recv_bytes);

		log("client", "received bytes array");
		for(int i = 0; i < WORD_LENGTH; i++)
             printf(" %02x", bytes[i]);
        putchar('\n');

		// converting bytes array to bool array
		convert_to_bool(bytes, letters, sizeof(letters));
		
		// saving current state
		ui.save_state(buffer, letters);

		if(ui.is_guessed())
			break;

		ui.decrement_attempts();
	}
	
	std::strncpy(buffer, EXIT_CODE, sizeof(buffer));
	sendto(sockfd, buffer, sizeof(buffer), MSG_CONFIRM,
		  (struct sockaddr *)&server_addr, sizeof(server_addr));

	system("clear");
	ui.display();
	ui.display_result();
}
