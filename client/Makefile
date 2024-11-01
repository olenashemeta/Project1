CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -pthread
LDFLAGS = `pkg-config --libs gtk+-3.0`
SRC_DIR = src
NAME = client
SRCS = $(wildcard $(SRC_DIR)/*.c)

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(NAME)
