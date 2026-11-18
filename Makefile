CC = gcc

TARGET = keyoverlay-karlson.exe
SRC = main.c

CFLAGS = -O3 -flto -march=native -Wall -Wextra -std=c11
LIBS = -lraylib -lopengl32 -lgdi32 -lwinmm

all: $(TARGET)

$(TARGET): $(SRC) config.h
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

.PHONY: all clean