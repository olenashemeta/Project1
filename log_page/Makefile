CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -pthread
LDFLAGS = `pkg-config --libs gtk+-3.0`
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = inc
LIB_DIR = libmx
LIB_MX = $(LIB_DIR)/libmx.a
CLIENT = client

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
INC = $(INC_DIR) $(LIB_DIR)/inc

all: install

install: $(LIB_MX) $(CLIENT)

$(CLIENT): $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) -L$(LIB_DIR) -lmx -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)  # Ensure the object directory exists
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_DIR) -I$(LIB_DIR)/inc  # Include both directories

$(LIB_MX):
	@make -sC $(LIB_DIR)

clean:
	@rm -rf $(OBJ_DIR) $(CLIENT)

uninstall:
	@make -sC $(LIB_DIR) uninstall
	@rm -rf $(OBJ_DIR) $(CLIENT)

reinstall: uninstall all


