SRC_DIR := src
OBJ_DIR := obj
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))
INCLUDES := -Iinclude/
LDFLAGS := -L/usr/lib -l:libglfw.so -lm -lGL -l:libGLEW.so -l:libconfig++.so -lstdc++ -shared-libgcc
#compiler opt set to 0 temporarily so that I can get faster compile times
CPPFLAGS := -O0 -Wall -xc++

bin/program: $(OBJ_FILES)
	gcc $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	gcc $(CPPFLAGS) -c -o $@ $<
