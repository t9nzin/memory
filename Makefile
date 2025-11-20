CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude -Wno-deprecated-declarations -fno-builtin
LDFLAGS =

SRC_DIR = src
TEST_DIR = tests
BUILD_DIR = build

SOURCES = $(SRC_DIR)/allocator.c
TESTS = test_basic test_edge_cases benchmark
LIB = $(BUILD_DIR)/liballocator.a

.PHONY: all clean tests lib

all: $(BUILD_DIR) $(TESTS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

lib: $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $(SOURCES) -o $(BUILD_DIR)/allocator.o
	ar rcs $(LIB) $(BUILD_DIR)/allocator.o

test_basic: $(SOURCES) $(TEST_DIR)/test_basic.c
	$(CC) $(CFLAGS) $(SOURCES) $(TEST_DIR)/test_basic.c -o $(BUILD_DIR)/$@ $(LDFLAGS)

test_edge_cases: $(SOURCES) $(TEST_DIR)/test_edge_cases.c
	$(CC) $(CFLAGS) $(SOURCES) $(TEST_DIR)/test_edge_cases.c -o $(BUILD_DIR)/$@ $(LDFLAGS)

benchmark: $(SOURCES) $(TEST_DIR)/benchmark.c
	$(CC) $(CFLAGS) $(SOURCES) $(TEST_DIR)/benchmark.c -o $(BUILD_DIR)/$@ $(LDFLAGS)

tests: all
	./$(BUILD_DIR)/test_basic
	./$(BUILD_DIR)/test_edge_cases

bench: all
	./$(BUILD_DIR)/benchmark

clean:
	rm -rf $(BUILD_DIR)