CC=gcc
CFLAGS=-Wall -c $(INCLUDES)
LDFLAGS=$(LIBS)

INCLUDES=`pkg-config --cflags xcb` `pkg-config --cflags cairo` `pkg-config --cflags lua53`
LIBS=`pkg-config --libs xcb` `pkg-config --libs cairo` `pkg-config --libs lua53`

BUILD_DIR=build
SRC_DIR=src
SOURCES=$(wildcard $(SRC_DIR)/*.c)
OBJ_DIR=$(BUILD_DIR)/obj
OBJECTS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE=overlay

all: directories $(OBJECTS) $(EXECUTABLE)

directories:
	mkdir build
	mkdir build/obj

$(OBJECTS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $(BUILD_DIR)/$(EXECUTABLE)

clean:
	rm -R build
