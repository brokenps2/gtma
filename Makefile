SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDES := -Iinclude/
LDFLAGS := -L/usr/lib -g -l:libglfw.so -lm -lGL -lopenal -l:libGLEW.so -l:libconfig++.so
CPPFLAGS := -g -O2 -Wall

bin/program: $(OBJ_FILES)
	gcc $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc $(CPPFLAGS) -c -o $@ $<
