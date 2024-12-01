CC = gcc
CFLAGS = -Wall
SRC_DIR = src
NAME = server

all: $(NAME)

$(NAME): $(SRC_DIR)/server.c
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -f $(NAME)
