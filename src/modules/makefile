CC=clang
CFLAGS=-Wextra -Werror -Wno-unused-parameter -O3 -c $(INCLUDES)

INCLUDES=`pkg-config --cflags cairo`

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(OBJECTS)

$(OBJECTS): $(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
