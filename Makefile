.PHONY: all clean test valgrind debug

CC = gcc
CFLAGS = -Wall -Wextra -g

SRCS1 = src/main.c src/decoder.c src/memory.c
SRCS2 = src/decoder.c test/test_decoder.c

all:
	@printf "\nRISC-V RV32I Instruction Decoder\n"
	@printf "================================\n\n"
	@$(CC) $(CFLAGS) $(SRCS1) -o run
	@./run test/programs/i_type.hex
	@./run test/programs/r_type.hex
	@./run test/programs/branch.hex
	@./run test/programs/mixed.hex

test:
	@$(CC) $(CFLAGS) $(SRCS2) -o test_decoder
	@./test_decoder

valgrind: run
	valgrind --leak-check=full ./run test/programs/r_type.hex
	valgrind --leak-check=full ./run test/programs/i_type.hex
	valgrind --leak-check=full ./run test/programs/branch.hex
	valgrind --leak-check=full ./run test/programs/mixed.hex

debug: run
	@gdb --args ./run test/programs/mixed.hex
	
clean:
	@rm -f run