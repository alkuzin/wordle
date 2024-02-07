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

typedef unsigned long   u64;
typedef unsigned int    u32;
typedef unsigned short  u16;
typedef unsigned char   u8;

#define UTOA_SIZE 12

enum exception {
	INVALID_IP_ADDRESS_EXCEPTION,
	FORBIDDEN_PORT_EXCEPTION,
	SOCKET_CREATION_EXCEPTION,
	BIND_EXCEPTION,
	WORDLIST_FILE_OPEN_EXCEPTION
};

// unsigned int (u32) to string
void _utoa(u32 n, char *buffer);

// record logs
void _log(const char *src, const char *log_msg);

// record formatted logs
void _logf(const char *src, const char *log_msg_fmt, ...);

// get correct user input
void _getinput(char *input, u32 size);

// display bytes array
void _display_bytes(const char *bytes, u32 size);

// convert bool array to byte array
void _convert_to_bytes(const bool *bool_arr, char *buffer, u32 size);

// convert byte array to bool array
void _convert_to_bool(const char *byte_arr, bool *buffer, u32 size);

#endif // UTILS_H
