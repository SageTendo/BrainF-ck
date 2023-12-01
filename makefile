CC = cc
CFLAGS = -Wall -Wextra -g

# Add your source files here
SRC = FrainBuck.c stack.c

# Name of the executable
EXEC = FrainBuck

all: $(EXEC)

$(EXEC): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

# usage: make run ARGS="tests/hello.bf"
run: $(EXEC)
	./$(EXEC) $(ARGS)

.PHONY: clean

clean:
	rm -f $(EXEC)

