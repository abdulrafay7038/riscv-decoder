.PHONY: all clean test valgrind debug

CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = run
SRCS = src/main.c src/decoder.c src/memory.c

all: test

$(TARGET):
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

test: $(TARGET)
	@printf "\nRISC-V RV32I Instruction Decoder\n"
	@printf "================================\n\n"
	@./$(TARGET) test/programs/i_type.hex
	@./$(TARGET) test/programs/r_type.hex
	@./$(TARGET) test/programs/branch.hex
	@./$(TARGET) test/programs/mixed.hex

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET) test/programs/r_type.hex

debug: $(TARGET)
	gdb ./$(TARGET)

clean:
	rm -f $(TARGET)