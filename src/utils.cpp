/*
 *	utils.c - auxillary functions
 *	@alkuzin - 2024
 *
 */

#include "../include/utils.h"

static void _get_date(char *date);

static void _get_date(char *date)
{
    time_t current_time;

	current_time = time(NULL);
    strftime(date, DATE_SIZE, "%c", localtime(&current_time));
}

void _error(const char *error_msg)
{
	fprintf(stderr, "error: %s\n", error_msg);
	exit(EXIT_FAILURE);
}

void _log(const char *src, const char *log_msg)
{
	char date[DATE_SIZE];

	_get_date(date);
	printf("[%s] %s: %s\n", date, src, log_msg);
}

void _logf(const char *src, const char *log_msg_fmt, ...)
{
	va_list args;
	char    date[DATE_SIZE];

	_get_date(date);
	printf("[%s] %s: ", date, src);

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
