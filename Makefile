CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
LDFLAGS = -lraylib
TARGET = gol
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean

