CC = gcc

PKG_CONFIG = pkg-config
GTK_FLAGS = $(shell $(PKG_CONFIG) --cflags --libs gtk+-3.0)
RSVG_FLAGS = $(shell $(PKG_CONFIG) --cflags --libs librsvg-2.0)
CFLAGS = -pthread $(GTK_FLAGS) $(RSVG_FLAGS)

SRC_DIR = src
OBJ_DIR = obj

NAME = client

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(NAME)
	rm -rf $(OBJ_DIR)

.PHONY: all clean