## Name:   Isaak Choi
## UniKey: icho6322
## SID:    520488399

## USYD CODE CITATION ACKNOWLEDGEMENT - NOT ENTIRELY MY OWN WORK
# Adapted and extended from: 
# "How to get Makefile to put obj files in separate directory?"
# https://stackoverflow.com/questions/59180047/how-to-get-makefile-to-put-obj-files-in-separate-directory
# Last access March 18, 2023


## Set phony make commands
.PHONY: clean build all git small tests run_tests


## Compilation settings
CC = gcc
STD = -std=c11
SHARED_FLAGS = $(STD) -Wvla -Os -g -Wall -flto -fstrict-aliasing -fno-asynchronous-unwind-tables -fno-unwind-tables # -z norelro -Werror
COMPILE_FLAGS = -I$(INCLUDE_DIR) -c $(SHARED_FLAGS)
LINK_FLAGS = $(SHARED_FLAGS)
ASAN_FLAGS = #-fsanitize=address
# DEBUG += -D DEBUG_DETECT_LEAKS      # Check and generate a report on any memory leaks at run time. Output 'leak_info.txt'.
# DEBUG += -D DEBUG_PRINT_MEM_ACCESS  # Print summary of each vm memory access request
# DEBUG += -D DEBUG_PRINT_INSTRUCTION # Each time an instruction is executed print the instruction and associated info
# DEBUG += -D DEBUG_PRINT_PC          # Print the program counter each time a virtual instruction is executed
# DEBUG += -D DEBUG_STEP_THROUGH      # Manualy step (simulated) instruction by instruction during test runs
# DEBUG += -D DEBUG_CHECK_LIST_RANGE  # Error checks the range in linked-list manipulation requests


## Setup paths
INCLUDE_DIR = ./include/
SRC_DIR = ./src
OBJ_DIR = ./obj
TEST_DIR = ./tests

## File lists
CFILES = $(wildcard $(SRC_DIR)/*.c)
OBJS   = $(CFILES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

## Name of the produced binary
BIN_OUT_NAME = vm_riskxvii


## Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(COMPILE_FLAGS) $(ASAN_FLAGS) $(DEBUG) -o $@ -c $<

$(BIN_OUT_NAME): $(OBJS)
	@echo --------------------------------------------------
	@echo Linking object files into main executable ...
	$(CC) $(LINK_FLAGS) $(ASAN_FLAGS) -o $@ $(OBJS)
	@echo "Binary size (Bytes): `wc -c $(BIN_OUT_NAME)`"
	@echo DONE
	make small

## Makes the produced binary smaller
small:
	@echo --------------------------------------------------
	@echo Minimising binary size post compilation ...
	strip -R .comment $(BIN_OUT_NAME)
	strip -R .note.gnu.build-id $(BIN_OUT_NAME)
	strip -R .eh_frame_hdr $(BIN_OUT_NAME)
	strip -R .eh_frame $(BIN_OUT_NAME)
	@echo "New binary size (Bytes): `wc -c $(BIN_OUT_NAME)`"
	@echo DONE

## Remove output object files and binary
clean:
	@echo --------------------------------------------------
	@echo Removing uneeded files ...
	rm -f $(OBJS) $(BIN_OUT_NAME) *~
	@echo DONE

## Make a git commit
git:
	make clean
	@echo --------------------------------------------------
	@echo Adding and pushing all changes to git repo ...
	git add .
	git commit -m "Committed automatically from makefile"
	git push
	@echo DONE

## Make test files
tests:
	make
	@echo --------------------------------------------------
	@echo Test binaries already compiled.

## Run tests
run_tests:
	make
	@echo --------------------------------------------------
	@echo Running tests ...
	
	@echo
	@echo diff [all-shifts]:
	@./$(BIN_OUT_NAME) $(TEST_DIR)/all-shifts/all-shifts.mi < $(TEST_DIR)/all-shifts/all-shifts.in | diff $(TEST_DIR)/all-shifts/all-shifts.out -
	
	@echo
	@echo diff [heap-unallocated-read]:
	@./$(BIN_OUT_NAME) $(TEST_DIR)/heap-unallocated-read/heap-unallocated-read.mi < $(TEST_DIR)/heap-unallocated-read/heap-unallocated-read.in | diff $(TEST_DIR)/heap-unallocated-read/heap-unallocated-read.out -
	
	@echo
	@echo diff [heap-unallocated-write]:
	@./$(BIN_OUT_NAME) $(TEST_DIR)/heap-unallocated-write/heap-unallocated-write.mi < $(TEST_DIR)/heap-unallocated-write/heap-unallocated-write.in | diff $(TEST_DIR)/heap-unallocated-write/heap-unallocated-write.out -
	
	@echo
	@echo diff [file-to-large]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/file-to-large/file-to-large.mi < $(TEST_DIR)/file-to-large/file-to-large.in | diff $(TEST_DIR)/file-to-large/file-to-large.out -
	
	@echo
	@echo diff [file-too-small]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/file-too-small/file-too-small.mi < $(TEST_DIR)/file-too-small/file-too-small.in | diff $(TEST_DIR)/file-too-small/file-too-small.out -

	@echo
	@echo diff [simple-math]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/simple-math/simple-math.mi < $(TEST_DIR)/simple-math/simple-math.in | diff $(TEST_DIR)/simple-math/simple-math.out -

	@echo
	@echo diff [simple-logic]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/simple-logic/simple-logic.mi < $(TEST_DIR)/simple-logic/simple-logic.in | diff $(TEST_DIR)/simple-logic/simple-logic.out -

	@echo
	@echo diff [extra-shift-right]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/extra-shift-right/extra-shift-right.mi < $(TEST_DIR)/extra-shift-right/extra-shift-right.in | diff $(TEST_DIR)/extra-shift-right/extra-shift-right.out -

	@echo
	@echo diff [simple-memory]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/simple-memory/simple-memory.mi < $(TEST_DIR)/simple-memory/simple-memory.in | diff $(TEST_DIR)/simple-memory/simple-memory.out -

	@echo
	@echo diff [instruction-not-implemented]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/instruction-not-implemented/instruction-not-implemented.mi < $(TEST_DIR)/instruction-not-implemented/instruction-not-implemented.in | diff $(TEST_DIR)/instruction-not-implemented/instruction-not-implemented.out -
	
	@echo
	@echo diff [pc-overflow]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/pc-overflow/pc-overflow.mi < $(TEST_DIR)/pc-overflow/pc-overflow.in | diff $(TEST_DIR)/pc-overflow/pc-overflow.out -

	@echo
	@echo diff [simple-control-flow-1]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/simple-control-flow-1/simple-control-flow-1.mi < $(TEST_DIR)/simple-control-flow-1/simple-control-flow-1.in | diff $(TEST_DIR)/simple-control-flow-1/simple-control-flow-1.out -

	@echo
	@echo diff [simple-control-flow-2]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/simple-control-flow-2/simple-control-flow-2.mi < $(TEST_DIR)/simple-control-flow-2/simple-control-flow-2.in | diff $(TEST_DIR)/simple-control-flow-2/simple-control-flow-2.out -

	@echo
	@echo diff [err-file-nonexistent]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/err-file-nonexistent/err-file-nonexistent.mi < $(TEST_DIR)/err-file-nonexistent/err-file-nonexistent.in | diff $(TEST_DIR)/err-file-nonexistent/err-file-nonexistent.out -

	@echo
	@echo diff [pc-below-zero]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/pc-below-zero/pc-below-zero.mi < $(TEST_DIR)/pc-below-zero/pc-below-zero.in | diff $(TEST_DIR)/pc-below-zero/pc-below-zero.out -

	@echo
	@echo diff [simple-shift]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/simple-shift/simple-shift.mi < $(TEST_DIR)/simple-shift/simple-shift.in | diff $(TEST_DIR)/simple-shift/simple-shift.out -

	@echo
	@echo diff [simple-control-flow-3]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/simple-control-flow-3/simple-control-flow-3.mi < $(TEST_DIR)/simple-control-flow-3/simple-control-flow-3.in | diff $(TEST_DIR)/simple-control-flow-3/simple-control-flow-3.out -

	@echo
	@echo diff [jalr]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/jalr/jalr.mi < $(TEST_DIR)/jalr/jalr.in | diff $(TEST_DIR)/jalr/jalr.out -

	@echo
	@echo diff [virtual-routines]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/virtual-routines/virtual-routines.mi < $(TEST_DIR)/virtual-routines/virtual-routines.in | diff $(TEST_DIR)/virtual-routines/virtual-routines.out -

	@echo
	@echo diff [invalid-read-int]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/invalid-read-int/invalid-read-int.mi < $(TEST_DIR)/invalid-read-int/invalid-read-int.in | diff $(TEST_DIR)/invalid-read-int/invalid-read-int.out -

	@echo
	@echo diff [malloc-out-of-space]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/malloc-out-of-space/malloc-out-of-space.mi < $(TEST_DIR)/malloc-out-of-space/malloc-out-of-space.in | diff $(TEST_DIR)/malloc-out-of-space/malloc-out-of-space.out -

	@echo
	@echo diff [malloc-multi-bank-alloc]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/malloc-multi-bank-alloc/malloc-multi-bank-alloc.mi < $(TEST_DIR)/malloc-multi-bank-alloc/malloc-multi-bank-alloc.in | diff $(TEST_DIR)/malloc-multi-bank-alloc/malloc-multi-bank-alloc.out -

	@echo
	@echo diff [malloc-negative-size]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/malloc-negative-size/malloc-negative-size.mi < $(TEST_DIR)/malloc-negative-size/malloc-negative-size.in | diff $(TEST_DIR)/malloc-negative-size/malloc-negative-size.out -

	@echo
	@echo diff [malloc-too-big]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/malloc-too-big/malloc-too-big.mi < $(TEST_DIR)/malloc-too-big/malloc-too-big.in | diff $(TEST_DIR)/malloc-too-big/malloc-too-big.out -

	@echo
	@echo diff [free-not-starting-bank-addr]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/free-not-starting-bank-addr/free-not-starting-bank-addr.mi < $(TEST_DIR)/free-not-starting-bank-addr/free-not-starting-bank-addr.in | diff $(TEST_DIR)/free-not-starting-bank-addr/free-not-starting-bank-addr.out -

	@echo
	@echo diff [free-unallocated]:
	@-./$(BIN_OUT_NAME) $(TEST_DIR)/free-unallocated/free-unallocated.mi < $(TEST_DIR)/free-unallocated/free-unallocated.in | diff $(TEST_DIR)/free-unallocated/free-unallocated.out -

#	@echo
#	@echo diff [REPLACE]:
#	@-./$(BIN_OUT_NAME) $(TEST_DIR)/REPLACE/REPLACE.mi < $(TEST_DIR)/REPLACE/REPLACE.in | diff $(TEST_DIR)/REPLACE/REPLACE.out -


