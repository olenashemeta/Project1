CC = gcc
CFLAGS = -Wall -I../libs/libmx/ -I../libs/Sqlite3Lib/ -I./src -I./src/database -I./src/database/crud/db_crud -I./src/database/crud/model_crud -I./src/database/migrations -I./src/model_functions
SRC_DIR = src
OBJ_DIR = obj
NAME = server

LIB_DIR = ../libs/JsonLib
LIB_MX_DIR = ../libs/libmx
SQLITE_DIR = ../libs/Sqlite3Lib
LDFLAGS = -L$(LIB_DIR) -L$(LIB_MX_DIR) -L$(SQLITE_DIR) -lssl -lcrypto -lcjson -lmx -lsqlite3 -lm -Wl,-rpath=$(LIB_DIR):$(LIB_MX_DIR):$(SQLITE_DIR)

SRC_DIRS = $(SRC_DIR) $(SRC_DIR)/database $(SRC_DIR)/database/crud/db_crud $(SRC_DIR)/database/crud/model_crud $(SRC_DIR)/database/migrations $(SRC_DIR)/model_functions

SRCS = $(wildcard $(foreach dir, $(SRC_DIRS), $(dir)/*.c))
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

KEY_DIR = .
PRIVATE_KEY = $(KEY_DIR)/private_key.pem
PUBLIC_KEY = $(KEY_DIR)/public_key.pem

all: keys $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

keys: $(PRIVATE_KEY) $(PUBLIC_KEY)

$(PRIVATE_KEY):
	openssl genpkey -algorithm RSA -out $(PRIVATE_KEY)

$(PUBLIC_KEY): $(PRIVATE_KEY)
	openssl rsa -in $(PRIVATE_KEY) -pubout -out $(PUBLIC_KEY)

clean:
	rm -rf $(NAME) $(OBJ_DIR) $(PRIVATE_KEY) $(PUBLIC_KEY)
