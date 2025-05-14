SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(shell find $(SRC_DIR) -type f -name "*.c")
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
LDFLAGS := -lSDL2 -g -lSDL2main -lm -lpthread -lGL -lopenal -l:libGLEW.so -lcjson
CPPFLAGS := -Wall -g -O0 -pthread -I./src

bin/gtma: $(OBJ_FILES)
	gcc $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	gcc $(CPPFLAGS) -c -o $@ $<

