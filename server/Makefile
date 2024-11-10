CC = gcc
CFLAGS = -Wall -I../libs/libmx/
SRC_DIR = src
OBJ_DIR = obj
NAME = server

LIB_DIR = ../libs/JsonLib
LIB_MX_DIR = ../libs/libmx
LDFLAGS = -L$(LIB_DIR) -lssl -lcrypto $(LIB_DIR)/libcjson.so.1.7.18 $(LIB_MX_DIR)/libmx.a
RPATH_FLAGS = -Wl,-rpath=$(LIB_DIR)

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

KEY_DIR = .
PRIVATE_KEY = $(KEY_DIR)/private_key.pem
PUBLIC_KEY = $(KEY_DIR)/public_key.pem

all: keys $(NAME)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) $(RPATH_FLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
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
