LIB     = libmx.a

CFLAGS  = -std=c11 -Wall -Wextra -Werror -Wpedantic
CC      = clang

SRC_DIR  = src
INC_DIR  = inc
OBJ_DIR  = obj

INC      = $(INC_DIR)/libmx.h

SRC      = $(wildcard $(SRC_DIR)/*.c)

OBJ      = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: install

install: $(LIB)

$(LIB): $(OBJ)
	@ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ -I$(INC_DIR)

clean:
	@rm -rf $(OBJ_DIR)

uninstall: clean
	@rm -rf $(LIB)

reinstall: uninstall install


