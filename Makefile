SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDES := -Iinclude/
LDFLAGS := -L/usr/lib -l:libglfw.so -lm -lGL -l:libGLEW.so -l:libconfig++.so
CPPFLAGS := -O2 -Wall

bin/program: $(OBJ_FILES)
	g++ $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	g++ $(CPPFLAGS) -c -o $@ $<
