CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Werror -Wpedantic -D_GNU_SOURCE \
         `pkg-config --cflags gtk+-3.0` -pthread
LDFLAGS = `pkg-config --libs gtk+-3.0` -lssl -lcrypto

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
NAME = uchat

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(OBJ_DIR) $(NAME)

.PHONY: all clean
