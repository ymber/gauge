VPATH=src src/modules

CC=clang
INCLUDES=`pkg-config --cflags xcb cairo`
LIBS=`pkg-config --libs xcb cairo` -pthread -lcjson
CFLAGS=-Wextra -Werror -c $(INCLUDES)

BUILD_DIR=$(abspath build)
SOURCES=$(notdir $(foreach dir,$(VPATH),$(wildcard $(dir)/*.c)))
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))
EXECUTABLE=overlay

DBG_BUILD_DIR=$(BUILD_DIR)/debug
DBG_OBJECTS=$(addprefix $(DBG_BUILD_DIR)/obj/, $(OBJECTS))
DBG_EXECUTABLE=$(DBG_BUILD_DIR)/$(EXECUTABLE)
DBG_CFLAGS=-g -O0

REL_BUILD_DIR=$(BUILD_DIR)/release
REL_OBJECTS=$(addprefix $(REL_BUILD_DIR)/obj/, $(OBJECTS))
REL_EXECUTABLE=$(REL_BUILD_DIR)/$(EXECUTABLE)
REL_CFLAGS=-O3

debug: directories $(DBG_OBJECTS) $(DBG_EXECUTABLE)
release: directories $(REL_OBJECTS) $(REL_EXECUTABLE)

directories:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(DBG_BUILD_DIR)
	mkdir -p $(DBG_BUILD_DIR)/obj
	mkdir -p $(REL_BUILD_DIR)
	mkdir -p $(REL_BUILD_DIR)/obj

$(DBG_OBJECTS): $(DBG_BUILD_DIR)/obj/%.o: %.c
	$(CC) $(CFLAGS) $(DBG_CFLAGS) $< -o $@

$(DBG_EXECUTABLE): $(DBG_OBJECTS)
	$(CC) $(wildcard $(DBG_BUILD_DIR)/obj/*.o) -o $(DBG_EXECUTABLE) $(LIBS)

$(REL_OBJECTS): $(REL_BUILD_DIR)/obj/%.o: %.c
	$(CC) $(CFLAGS) $(REL_CFLAGS) $< -o $@

$(REL_EXECUTABLE): $(REL_OBJECTS)
	$(CC) $(wildcard $(REL_BUILD_DIR)/obj/*.o) -o $(REL_EXECUTABLE) $(LIBS)

clean:
	rm -Rf $(BUILD_DIR)
