/*
 *	utils.h - auxillary functions
 *	@alkuzin - 2024
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include <ctime>

typedef unsigned long   u64;
typedef unsigned int    u32;
typedef unsigned short  u16;
typedef unsigned char   u8;

#define DATE_SIZE 32

enum exception {
	INVALID_IP_ADDRESS_EXCEPTION,
	FORBIDDEN_PORT_EXCEPTION,
	SOCKET_CREATION_EXCEPTION,
	BIND_EXCEPTION,
	WORDLIST_FILE_OPEN_EXCEPTION,
	MEMORY_ALLOCATION_EXCEPTION
};

// print errors
void error(const char *error_msg);

// record logs
void log(const char *src, const char *log_msg);

// record formatted logs
void logf(const char *src, const char *log_msg_fmt, ...);

// get correct user input
void getinput(char *buffer, u32 size);

// convert bool array to byte array
void convert_to_bytes(const bool *bool_arr, u8 *buffer, u32 size);

// convert byte array to bool array
void convert_to_bool(const u8 *byte_arr, bool *buffer, u32 size);

#endif // UTILS_H
