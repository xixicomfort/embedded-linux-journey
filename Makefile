CC = gcc
CFLAGS = -Wall -Wextra -O2 -g

SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)
TARGET = main

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.Phony: clean
clean:
	rm -f *.o $(TARGET)
	@echo "Clean All Objects and executable"

.PHONY: print
print:
	@echo "SRC = $(SRC)"
	@echo "OBJ = $(OBJ)"

