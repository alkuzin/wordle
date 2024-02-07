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

server: $(OBJS_PREFIXED) 
	$(CC) $(CFLAGS) server_main.cpp $(OBJS_PREFIXED) -o server

client: $(OBJS_PREFIXED)
	$(CC) $(CFLAGS) client_main.cpp $(OBJS_PREFIXED) -o client

all: server client

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -f server client

re: clean all
