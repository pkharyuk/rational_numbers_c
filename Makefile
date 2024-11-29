SHELL = bash
CC = gcc
LIBS = -lm

SRC_DIR = src
UTEST_DIR = tests
BUILD_DIR = build

SAMPLE_N = 7
SAMPLE_INPUT_FILE = ./sample_input.txt

TARGET_SRC_FILE = ./demo.c
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
#SRC_FILES := $(shell ls $(SRC_DIR)/*.{c,h} 2>/dev/null)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

CFLAGS = -Wall --pedantic -I./$(SRC_DIR)
UTCFLAGS = -g

UTEST_FILES = $(wildcard $(UTEST_DIR)/*.c)
UTEST_OBJ_FILES = $(patsubst $(UTEST_DIR)/%.c,$(BUILD_DIR)/%.o,$(UTEST_FILES))

TARGET = demo
UT_TARGET = unit_test

compile: $(TARGET)

run_demo: $(TARGET)
	@echo "1) Randomly sampled input: "
	./$(TARGET) -n $(SAMPLE_N)
	@echo -e "\n2) Input from file: "
	./$(TARGET) -i $(SAMPLE_INPUT_FILE)
	@echo -e "\n"

unit_tests: $(UT_TARGET)
	./$(UT_TARGET)

$(TARGET): $(OBJ_FILES) $(TARGET_SRC_FILE)
	echo "$(SRC_FILES)"
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

$(UT_TARGET): $(UTEST_OBJ_FILES) $(OBJ_FILES)
	$(CC) $(CFLAGS) $(UTCFLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(LIBS) -c $< -o $@

$(BUILD_DIR)/%.o: $(UTEST_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(UTCFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

memcheck: $(TARGET)
	valgrind --leak-check=full --undef-value-errors=no ./$(TARGET) -n $(SAMPLE_N)
	valgrind --leak-check=full --undef-value-errors=no ./$(TARGET) -i $(SAMPLE_INPUT_FILE)
	#valgrind --leak-check=full --track-origins=yes ./$(TARGET) -i ./sample_input.txt

clean:
	rm -rf $(BUILD_DIR)/*
	rm $(TARGET) $(UT_TARGET)

.PHONY: clean compile memcheck unit_test
