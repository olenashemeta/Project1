CC = gcc
SRC_DIR = src
NAME = client

all: $(NAME)

$(NAME): $(SRC_DIR)/client.c
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(NAME)
