SRC_DIR := src
OBJ_DIR := obj
PIC_OBJ_DIR := obj_pic
BIN_DIR := bin

CC := gcc

SRC_FILES := $(shell find $(SRC_DIR) -type f -name "*.c")

# Exclude these from the library
LIB_SRC_FILES := $(filter-out \
    src/scenes/% \
    src/init/% , \
    $(SRC_FILES))

BIN_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
LIB_OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(PIC_OBJ_DIR)/%.o,$(LIB_SRC_FILES))

CFLAGS := -Wall -g -O2 -pthread -I./src -I./deps/include
PIC_CFLAGS := $(CFLAGS) -fPIC

LDFLAGS := -lSDL2 -lSDL2main -lm -lpthread -lGL -lopenal -l:libGLEW.so -lcjson \
           -L./deps/lib


INCLUDE_DIR := include/gtma

LIB_HDR_FILES := $(patsubst $(SRC_DIR)/%.c,$(SRC_DIR)/%.h,$(LIB_SRC_FILES))
INSTALL_HDR_FILES := $(patsubst $(SRC_DIR)/%,$(INCLUDE_DIR)/%,$(LIB_HDR_FILES))

all: $(BIN_DIR)/libgtma.so

test-demo: $(BIN_DIR)/gtma

install-headers: $(INSTALL_HDR_FILES)

$(BIN_DIR)/gtma: $(BIN_OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

$(BIN_DIR)/libgtma.so: $(LIB_OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $<

$(PIC_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(PIC_CFLAGS) -c -o $@ $<

$(INCLUDE_DIR)/%.h: $(SRC_DIR)/%.h
	@mkdir -p $(dir $@)
	cp $< $@
