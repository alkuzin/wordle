/*
 *	utils.c - auxillary functions
 *	@alkuzin - 2024
 *
 */

#include "../include/utils.h"

void error(const char *error_msg)
{
	fprintf(stderr, "error: %s\n", error_msg);
	exit(EXIT_FAILURE);
}

void log(const char *src, const char *log_msg)
{
    time_t current_time;
    char current_date[32];

	current_time = time(NULL);
    strftime(current_date, 32, "%c", localtime(&current_time));

	printf("[%s] %s: %s\n", current_date, src, log_msg);
}

void getinput(char *buffer, u32 size) 
{
	int i;

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
