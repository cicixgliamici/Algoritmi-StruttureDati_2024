CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

TARGET = bakery
TEST_TARGET = test_runner

SRC = main.c strutture_dati.c ordini.c
TEST_SRC = test.c strutture_dati.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_TARGET)

clean:
	rm -f $(TARGET) $(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all test clean run
