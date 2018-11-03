CC=clang
CFLAGS=-Wextra -Werror -O3 -c $(INCLUDES)
LDFLAGS=$(LIBS)

INCLUDES=`pkg-config --cflags xcb cairo`
LIBS=`pkg-config --libs xcb cairo` -pthread

BUILD_DIR=$(abspath build)
SRC_DIR=$(abspath src)
SOURCES=$(wildcard $(SRC_DIR)/*.c)
export OBJ_DIR=$(BUILD_DIR)/obj
OBJECTS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))
EXECUTABLE=overlay

all: directories $(OBJECTS) modules $(EXECUTABLE)

directories:
	mkdir $(BUILD_DIR)
	mkdir $(OBJ_DIR)

$(OBJECTS): $(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $< -o $@

modules:
	$(MAKE) -C src/modules

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(wildcard $(OBJ_DIR)/*.o) -o $(BUILD_DIR)/$(EXECUTABLE)

clean:
	rm -R $(BUILD_DIR)
