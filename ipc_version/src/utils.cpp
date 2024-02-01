/*
 *	utils.c - auxillary functions
 *	@alkuzin - 2024
 *
 */

#include "../include/utils.h"

void _utoa(u32 n, char *buffer)
{
	snprintf(buffer, UTOA_SIZE, "%u", n);
	buffer[UTOA_SIZE] = '\0';
}

void _log(const char *src, const char *log_msg) {
	std::cout << "[" << src << "]: " << log_msg << std::endl;
}

void _logf(const char *src, const char *log_msg_fmt, ...)
{
	va_list args;
	std::cout << "[" << src << "]: "; 

	va_start(args, log_msg_fmt);
	vprintf(log_msg_fmt, args);
	va_end(args);
}

void _getinput(char *input, u32 size) 
{
	u32 i;

    i = 0;
	fgets(input, size, stdin);

	if (input) {
        
		while (input[i] != '\n' && input[i] != '\0')
			i++;
        
		if (input[i] == '\n')
            input[i] = '\0';
		else {
            while (getchar() != '\n')
                continue;
        }
    }
}

void _display_bytes(const char *bytes, u32 size)
{
	putchar('{');
	for(u32 i = 0; i < size; i++) {
    	printf(" %d", bytes[i]);
        	if(i < (size - 1))
         		putchar(',');
	}
	putchar(' ');
    putchar('}');
    putchar('\n');
}

void _convert_to_bytes(const bool *bool_arr, char *buffer, u32 size)
{
	u32 i;

	i = 0;
	while(i < size) {
		buffer[i] = (u8)bool_arr[i];
		i++;
	}
	buffer[size] = '\0';
}

void _convert_to_bool(const char *byte_arr, bool *buffer, u32 size)
{
	u32 i;

	i = 0;
	while(i < size) {
		buffer[i] = (bool)byte_arr[i];
		i++;
	}
}
