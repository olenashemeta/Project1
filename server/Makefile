CC = clang
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -I../libs/libmx/ -I../libs/Sqlite3Lib/ -I./src -I./src/database -I./src/database/crud/db_crud -I./src/database/crud/model_crud -I./src/database/migrations -I./src/model_functions -D_GNU_SOURCE
SRC_DIR = src
OBJ_DIR = obj
NAME = uchat_server

LIB_DIR = ../libs/JsonLib
LIB_MX_DIR = ../libs/libmx
SQLITE_DIR = ../libs/Sqlite3Lib

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Darwin)
    LDFLAGS = -L$(LIB_DIR) -L$(LIB_MX_DIR) -L$(SQLITE_DIR) -lssl -lcrypto -lcjson -lmx -lsqlite3 -lm
else
    LDFLAGS = -L$(LIB_DIR) -L$(LIB_MX_DIR) -L$(SQLITE_DIR) -lssl -lcrypto -lcjson -lmx -lsqlite3 -lm -Wl,-rpath=$(LIB_DIR):$(LIB_MX_DIR):$(SQLITE_DIR)
endif

SRC_DIRS = $(SRC_DIR) $(SRC_DIR)/database $(SRC_DIR)/database/crud/db_crud $(SRC_DIR)/database/crud/model_crud $(SRC_DIR)/database/migrations $(SRC_DIR)/model_functions

SRCS = $(wildcard $(foreach dir, $(SRC_DIRS), $(dir)/*.c))
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(NAME) $(OBJ_DIR)
