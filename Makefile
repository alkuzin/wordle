CC=g++
CFLAGS = -Wall -Wextra -Werror -g -lrt

SRC  = wordle_server wordle_client ipc_server ipc_client udp_server udp_client utils game ui
SRCS = $(addsuffix .cpp, $(SRC))
OBJS = $(addsuffix .o,   $(SRC))
SRCS_PREFIXED = $(addprefix src/, $(SRCS))

SRC_PATH=src/

OBJS_DIR = objs/
OBJS_PREFIXED = $(addprefix $(OBJS_DIR), $(OBJS))

$(OBJS_PREFIXED): $(SRCS_PREFIXED)
	mkdir -p $(OBJS_DIR)
	$(CC) $(CFLAGS) -c $(SRCS_PREFIXED)
	mv *.o $(OBJS_DIR)

udpserver: $(OBJS_PREFIXED)
	$(CC) $(CFLAGS) -DUDP_MODE=UDP server_main.cpp $(OBJS_PREFIXED) -o server_udp

udpclient: $(OBJS_PREFIXED)
	$(CC) $(CFLAGS) -DUDP_MODE=UDP client_main.cpp $(OBJS_PREFIXED) -o client_udp

ipcserver: $(OBJS_PREFIXED)
	$(CC) $(CFLAGS) -DIPC_MODE=IPC server_main.cpp $(OBJS_PREFIXED) -o server_ipc

ipcclient: $(OBJS_IPC_PREFIXED)
	$(CC) $(CFLAGS) -DIPC_MODE=IPC client_main.cpp $(OBJS_PREFIXED) -o client_ipc

udp: udpserver udpclient

ipc: ipcserver ipcclient

all: udp ipc

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f server_udp client_udp server_ipc client_ipc

re: clean all
