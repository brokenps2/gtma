TARGET = bin/program

SRCS = $(wildcard src/*.cpp)

INCLUDES = -Iinclude/

LIBDIRS = -L/usr/lib

LIBS = -l:libSDL2main.a -l:libSDL2.so -lm -lGL -l:libGLEW.so -l:libconfig++.so

CFLAGS = -O2 -Wall -ggdb -g3

CC = g++

all:
	$(CC) $(CFLAGS) $(SRCS) $(INCLUDES) $(LIBDIRS) $(LIBS) -o $(TARGET)

