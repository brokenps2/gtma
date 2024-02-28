TARGET = bin/program

SRCS = $(wildcard src/*.cpp)

OBJS := $(patsubst %.c, %.o, $(wildcard src/*.c))

INCLUDES = -Iinclude/

LIBDIRS = -L/usr/lib

LIBS = -l:libglfw.so -lm -lGL -l:libGLEW.so -l:libconfig++.so

CFLAGS = -O2 -Wall -ggdb -g3

CC = g++

all:
	$(CC) $(CFLAGS) $(SRCS) $(INCLUDES) $(LIBDIRS) $(LIBS) -o $(TARGET)

