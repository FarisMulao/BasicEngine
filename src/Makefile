# Variables
CC = gcc
CFLAGS = -Wall -Iexternal/cglm/include -I/usr/include/SDL2 -I/usr/include
LDFLAGS = -lSDL2 -lGL -lGLEW -lm
TARGET = test

# Source files
SRC = test.c shaders.h

# Object files
OBJ = test.o

# Rules
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

