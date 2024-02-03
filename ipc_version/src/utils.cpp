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
