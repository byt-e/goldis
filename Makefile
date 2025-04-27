CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./src
LIBS =

SRC_DIR = src
TEST_DIR = tests
BIN_DIR = bin

SRC = $(SRC_DIR)/main.c $(SRC_DIR)/server.c $(SRC_DIR)/handler.c
RESP_SRC = $(SRC_DIR)/resp.c
TEST_SRC = $(TEST_DIR)/test_resp.c

GOLDIS_BIN = $(BIN_DIR)/goldis
TEST_BIN = $(BIN_DIR)/test_resp

.PHONY: all clean test

all: $(GOLDIS_BIN)

$(GOLDIS_BIN): $(RESP_SRC) $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(TEST_BIN): $(RESP_SRC) $(TEST_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(BIN_DIR)/*

