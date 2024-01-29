/*
 *	utils.c - auxillary functions
 *	@alkuzin - 2024
 *
 */

#include "../include/utils.h"

static void get_date(char *date);

static void get_date(char *date)
{
    time_t current_time;

	current_time = time(NULL);
    strftime(date, DATE_SIZE, "%c", localtime(&current_time));
}

void error(const char *error_msg)
{
	fprintf(stderr, "error: %s\n", error_msg);
	exit(EXIT_FAILURE);
}

void log(const char *src, const char *log_msg)
{
	char date[DATE_SIZE];

	get_date(date);
	printf("[%s] %s: %s\n", date, src, log_msg);
}

void logf(const char *src, const char *log_msg_fmt, ...)
{
	va_list args;
	char    date[DATE_SIZE];

	get_date(date);
	printf("[%s] %s: ", date, src);

	va_start(args, log_msg_fmt);
	vprintf(log_msg_fmt, args);
	va_end(args);
}

void getinput(char *buffer, u32 size) 
{
	u32 i;

    i = 0;
	fgets(buffer, size, stdin);

	if (buffer) {
        
		while (buffer[i] != '\n' && buffer[i] != '\0')
			i++;
        
		if (buffer[i] == '\n')
            buffer[i] = '\0';
		else {
            while (getchar() != '\n')
                continue;
        }
    }
}

void convert_to_bytes(const bool *bool_arr, u8 *buffer, u32 size)
{
	u32 i;

	i = 0;
	while(i < size) {
		buffer[i] = (u8)bool_arr[i];
		i++;
	}
	buffer[size] = '\0';
}

// convert byte array to bool array
void convert_to_bool(const u8 *byte_arr, bool *buffer, u32 size)
{
	u32 i;

	i = 0;
	while(i < size) {
		buffer[i] = (bool)byte_arr[i];
		i++;
	}
}
