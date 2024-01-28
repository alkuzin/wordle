/*
 * server_main.cpp
 * @alkuzin - 2024
 *
 */

#include "include/game.h"
#include "include/server.h"

int main(int argc, char **argv) 
{
	
	if(argc > 1) {
		std::cout << argv[1] << std::endl;
	}

	//Server server(argv[1], atoi(argv[2]));
	Server server;

	server.init();
	
	/*
	bool arr[6] = {true, false, true, false, true, false};

	u8 buffer[7];

	convert_to_bytes(arr, buffer, 6);

	for(int i = 0; i < 6; i++)
		printf(" %02x", buffer[i]);
	putchar('\n');
*/
	return 0;
}
