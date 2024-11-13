CC = gcc
CFLAGS = `pkg-config --cflags gtk+-3.0` -pthread -I../libs/JsonLib -I../libs/libmx/
LDFLAGS = `pkg-config --libs gtk+-3.0` -lssl -lcrypto ../libs/JsonLib/libcjson.a ../libs/libmx/libmx.a -Wl,-rpath=../libs/JsonLib

SRC_DIR = src
NAME = client
SRCS = $(wildcard $(SRC_DIR)/*.c)

all: $(NAME)

$(NAME): $(SRCS)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(NAME)
