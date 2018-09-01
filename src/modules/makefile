CC=gcc
CFLAGS=-Wall -c $(INCLUDES)

INCLUDES=`pkg-config --cflags cairo`

SOURCES=$(wildcard *.c)
OBJECTS=$(patsubst %.c,$(OBJ_DIR)/%.o,$(SOURCES))

all: $(OBJECTS)

$(OBJECTS): $(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
