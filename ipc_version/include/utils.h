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
	WORDLIST_FILE_OPEN_EXCEPTION,
	MEMORY_ALLOCATION_EXCEPTION
};

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <semaphore.h>

#define SHARED_MEMORY_BLOCK_SIZE 128
#define SHARED_MEMORY_BLOCK_NAME "wordle_shared_memory"
#define IPC_RESULT_ERROR         -1

#define SEM_SERVER_NAME "/sem_server"
#define SEM_CLIENT_NAME "/sem_client"

// unsigned int (u32) to string
void _utoa(u32 n, char *buffer);

// record logs
void _log(const char *src, const char *log_msg);

// record formatted logs
void _logf(const char *src, const char *log_msg_fmt, ...);

// get correct user input
void _getinput(char *input, u32 size);

#endif // UTILS_H
