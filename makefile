CC=gcc
CFLAGS=-Wall -c $(INCLUDES)
LDFLAGS=$(LIBS)

INCLUDES=`pkg-config --cflags xcb` `pkg-config --cflags cairo`
LIBS=`pkg-config --libs xcb` `pkg-config --libs cairo`

BUILD_DIR=$(abspath build)
SRC_DIR=$(abspath src)
SOURCES=$(wildcard $(SRC_DIR)/*.c)
export OBJ_DIR=$(BUILD_DIR)/obj
OBJECTS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE=overlay

all: directories $(OBJECTS) $(EXECUTABLE)

directories:
	mkdir -p build/obj

$(OBJECTS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@
	$(MAKE) -C src/modules

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(wildcard $(OBJ_DIR)/*.o) -o $(BUILD_DIR)/$(EXECUTABLE)

clean:
	rm -R build
