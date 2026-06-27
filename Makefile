.PHONY: all clean test valgrind debug

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGET = run
TEST_TARGET = test_decoder

SRCS = src/main.c src/decoder.c src/memory.c
TEST_SRCS = src/decoder.c test/test_decoder.c

$(TARGET): $(SRCS)
	@$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

all: $(TARGET)
	@printf "\nRISC-V RV32I Instruction Decoder\n"
	@printf "================================\n\n"
	@./$(TARGET) test/programs/i_type.hex
	@./$(TARGET) test/programs/r_type.hex
	@./$(TARGET) test/programs/branch.hex
	@./$(TARGET) test/programs/mixed.hex

test:
	@$(CC) $(CFLAGS) $(TEST_SRCS) -o $(TEST_TARGET)
	@./$(TEST_TARGET)

valgrind: $(TARGET)
	@valgrind --leak-check=full ./$(TARGET) test/programs/r_type.hex
	@valgrind --leak-check=full ./$(TARGET) test/programs/i_type.hex
	@valgrind --leak-check=full ./$(TARGET) test/programs/branch.hex
	@valgrind --leak-check=full ./$(TARGET) test/programs/mixed.hex

debug: $(TARGET)
	@gdb --args ./$(TARGET) test/programs/mixed.hex

clean:
	@rm -f $(TARGET) $(TEST_TARGET)

help:
	@echo "Available targets:"
	@echo "  make           - Compile and create an executable"
	@echo "  make           - Decode all hex files in test/programs"
	@echo "  make test      - Run unit tests"
	@echo "  make valgrind  - Run memory leak checks"
	@echo "  make debug     - Start GDB"
	@echo "  make clean     - Remove generated executables"