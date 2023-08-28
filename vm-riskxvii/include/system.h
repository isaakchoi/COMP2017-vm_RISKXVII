// Name:   Isaak Choi
// UniKey: icho6322
// SID:    520488399


/* system.h

    Contains system information for the virtual machine, including 
    types, consts, system globals, and virtual routines:

    * Memory
    * Registers
    * Program counter
    * System run globals
    * Virtual routines
    * Memory interface functions

    NOTE
    * Uses virtual routine memory space to store system errors
      and cpu run status information:
      * 0x0850 - System CPU run status in form of <boolean>
      * 0x0854 - System error status in form of <int> error code

*/


// HEADER GUARD ...
#ifndef SYSTEM_H
#define SYSTEM_H


// DEPENDENCIES ...
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "heap_manager.h"


// THIRD PARTY MEMORY LEAK DETECTOR ...
#ifdef DEBUG_DETECT_LEAKS
    #include "leak_detector_c.h"
#endif


// ERROR CODES ...
#define ERR_NO_ERR             ( 0x00) // No error - Success
#define ERR_UNKNOWN_INSTR      (-0x01) // Unknown instruction encountered
#define ERR_ILLEGAL_OPERATION  (-0x02) // Illegal operation encountered
#define ERR_PC_OUT_OF_BOUNDS   (-0x03) // Program counter exceeded allowed bounds
#define ERR_HOST_MALLOC_FAILED (-0x04) // Malloc request on host computer failed 


// SYSTEM ANATOMY CONSTANTS ...

// The size of a byte on the system
#define BYTE_SIZE_BITS      (8)

// How much the program counter is incremented by default each instruction
#define DFLT_PC_INCREMENT   (4)

// The size of a word in bytes
#define WORD_SIZE           (4)

// The size of a word in bits
#define WORD_SIZE_BITS      (4 * BYTE_SIZE_BITS)

// Null address (equivalent of null pointer in virtual machine memory space)
#define NULL_ADDRESS        (0x00000000)


// REGISTER DETAILS ...

// The capacity of a register in bytes
#define REGISTER_SIZE_BYTES (4)

// The capacity of a register in bits
#define REGISTER_SIZE_BITS  (REGISTER_SIZE_BYTES * BYTE_SIZE_BITS)

// The number of registers within the system
#define NUM_REGISTERS       (32)

// The index of the zero register within the registers array
#define ZERO_REGISTER_ADDR  (0)

// The value of the zero register
#define ZERO_REGISTER_VAL   (0x00000000)


// SYSTEM MEMORY ...

// INSTRUCTION MEMORY

// The first address WITHIN the instruction mem section
#define INST_MEM_START (0x0000)

// The final address WITHIN the instruction mem section
#define INST_MEM_END   (0x03ff)

// The number of addresses WITHIN the instruction mem section
#define INST_MEM_SIZE  (INST_MEM_END - INST_MEM_START + 0x01)

// DATA MEMORY

// The first address WITHIN the data mem section
#define DATA_MEM_START (0x0400)

// The final address WITHIN the data mem section
#define DATA_MEM_END   (0x07ff) 

// The number of addresses WITHIN the data mem section
#define DATA_MEM_SIZE  (DATA_MEM_END - DATA_MEM_START + 0x01)

// VIRTUAL MEMORY

// The first address WITHIN the virtual mem section
#define VIRT_MEM_START (0x0800)

// The final address WITHIN the virtual mem section
#define VIRT_MEM_END   (0x08ff)

// The number of addresses WITHIN the virtual mem section
#define VIRT_MEM_SIZE  (VIRT_MEM_END - VIRT_MEM_START + 0x01)

// HEAP BANKS

// The size of each heap bank in bytes
#define HEAP_BANK_SIZE (64) 

// The number of heap banks
#define HEAP_BANK_NUM  (128)

// The size of heap bank mem in bytes
#define HEAP_MEM_SIZE  (HEAP_BANK_SIZE * HEAP_BANK_NUM)

// The starting address of heap banks
#define HEAP_MEM_START (0xb700)

// The end address (inclusive) of heap banks
#define HEAP_MEM_END   (HEAP_MEM_START + HEAP_MEM_SIZE - 0x01)

// OTHER MEMORY RELATED CONSTS

// The location at which memory indexing starts
#define MEM_START_ADDR (0x0000)

// The total size of the memory in bytes
#define MEM_SIZE_BYTES (HEAP_MEM_END + 0x01)

// The size of the input memory image binary file in bytes
#define MEM_IMG_BIN_FILE_SIZE (INST_MEM_SIZE + DATA_MEM_SIZE)

// The lower bound (inclusive) of readable memory
#define MEM_READABLE_LOWER_BOUND (0x0000)


// UNIVERSAL VIRTUAL ROUTINE CONSTANTS ...
#define VR_WRITE_CHAR_ADDR       (0x0800) // Console Write Character
#define VR_WRITE_INT_ADDR        (0x0804) // Console Write Signed Integer
#define VR_WRITE_UINT_ADDR       (0x0808) // Console Write Unsigned Integer
#define VR_HALT_ADDR             (0x080C) // Halt
#define VR_READ_CHAR_ADDR        (0x0812) // Console Read Character
#define VR_READ_INT_ADDR         (0x0816) // Console Read Signed Integer
#define VR_DUMP_PC_ADDR          (0x0820) // Dump PC
#define VR_DUMP_REG_ADDR         (0x0824) // Dump Register Banks
#define VR_DUMP_MEM_WORD_ADDR    (0x0828) // Dump Memory Word
#define VR_HEAP_BANK_MALLOC_ADDR (0x0830) // Heap Bank - Malloc
#define VR_HEAP_BANK_FREE_ADDR   (0x0834) // Heap Bank - Free


// CUSTOM VIRTUAL ROUTINE CONSTANTS ...
#define VR_CPU_RUN_ADDR          (0x0850) // Stores whether the vm CPU runs
#define VR_SYS_ERR_ADDR          (0x0854) // Stores the vm system error code
#define VR_HEAP_MANAGER_ADDR     (0x0858) // Stores a ptr to the heap manager
#define VR_HEAP_MANAGER_RESERVED (0x085C) // Reserved for manager addr overflow


// TYPEDEFS FOR READABILITY AND MAINTAINABILITY ...
typedef unsigned char byte;


// GLOBAL SYSTEM VARS ...

// The program counter
extern int32_t pc;

// Stores register data, with each index being a unique register
extern int32_t registers[NUM_REGISTERS];

// The main system memory
extern byte memory[MEM_SIZE_BYTES];


// SYSTEM SUB-OPERATION FUNCTIONS ...

/* Used to sign extend a two's compliment int of len < WORD_SIZE
    
    E.G., from 12 bits to 32 bits length.

    Works by copying the MSB of the original int to each bit leading to 
    the most-significant side of the new int.

    MAX new_bit_len = sizeof(int32_t) * 8
    MIN new_bit_len = 2 
    new_bit_len must be >= curr_bit_len

    WARNING - Doesn't check bounds of parameters - Do not exceed given bounds
*/
extern int32_t sign_extend_int32(int32_t num, int curr_len_bits, 
                                        int new_len_bits);


// ERROR CALL ABSTRACTIONS ...

/* Throws the error required when an unknown instruction is encountered

    * Sets appropriate error code
    * Prints error identifier text, the instruction in hex, and a register dump
      to stdout.

*/
extern void throw_not_implemented_err();

/* Throws the error required when an illegal operation is encountered

    * Sets appropriate error code
    * Prints error identifier text, the instruction in hex, and a register dump
      to stdout.

*/
extern void throw_illegal_operation_err();

/* Throws the error required when an the program counter exceeds allowed bounds

    * Sets appropriate error code
    * Prints error identifier text and a register dump to stdout

*/
extern void throw_pc_out_of_bounds_err();

/* Throws a malloc failed error

    NOTE
    This error is for when the host computer fails to malloc something
    rather than the vm failing a malloc call.

*/
extern void throw_host_malloc_failed_err();


// SYSTEM INTERFACE FUNCTIONS ...

/* Performs a register dump to stdout
    Prints the value pc followed by the value of registers 0 to NUM_REGISTERS
*/
extern void register_dump();

/* Sets the CPU run status for the system
    Setting to false will cause the simulated CPU to stop running
*/ 
extern void set_cpu_run_status(const bool run);

/* Returns the CPU run status as a boolean
    Returns true if CPU is/should run, else false
*/
extern bool get_cpu_run_status();

/* Sets the system error code
    A non zero code denotes an error
*/
extern void set_system_error_code(const int32_t err);

/* Returns the system error code
    A non zero code denotes an error
*/
extern int32_t get_system_error_code();

/* Retrieves the current instruction
    Returns the current instruction, pointed to by the program counter, 
    as a uint32_t.
*/
extern uint32_t get_instruction();


// VIRTUAL ROUTINES ...

/* Console Write Char
    Prints the value at the given location as a single ASCII 
    encoded character to stdout
*/
void vr_write_char(const char* const char_src_ptr);

/* Console Write Int
    Prints the value at the given location as a single signed integer to stdout
*/
void vr_write_int(const int32_t* const int_src);

/* Console Write Unsigned Int
    Prints the value at the given location as a single 
    unsigned integer to stdout
*/
void vr_write_uint(const uint32_t* const uint_src);

/* Halt
    Prints CPU halt message to stdout and stops the simulated CPU
*/
void vr_halt();

/* Console Read Character
    Reads a single char from stdin and stores as a single ASCII encoded char
    at the given location
*/
void vr_read_char(const void* const dst_ptr);

/* Console Read Signed Integer
    Reads a single signed int from stdin and stores as an int32 
    at the given location
*/
void vr_read_int(const void* const dst_ptr);

/* Dump PC
    Prints the value of the program counter to stdout
*/
void vr_dump_pc();

/* Dump Register Banks
    Invokes the register_dump() method, printing the values of 
    the PC and all registers to stdout
*/
void vr_dump_registers();

/* Dump Memory Word
    Prints the value (4 bytes interpreted in little endian) 
    stored at the given location in hexadecimal to stdout.
*/
void vr_dump_word(const void* src_ptr);

/* Malloc
    * Attempts to malloc() the given ammount of bytes in virtual machine memory
    * Saves the address of allocated memory chunk in R[28]
    * Saves NULL in R[28] if malloc failed
*/
void vr_malloc(const void* src_ptr);

/* Free
    * Attempts to free() the malloc'd memory at the given address
    * Throws illegal operation error if trying to free memory that hasn't 
      been allocated or is out of allowed bounds
*/
void vr_free(const void* src_ptr);


// MEMORY INTERFACE FUNCTIONS ...

/* Handle write to memory request

    * Checks for virtual routine calls
    * Verifies memory destination integrity
    * Performs write if necessary

    PARAMS
    src_ptr   | Pointer to the data source to be copied
    dst_addr  | Destination address in vm memory space
    data_size | The size of the data to be copied (in bytes)

*/
extern void mem_write(const void* const src_ptr, const int32_t dst_addr, const int data_size);

/* Handle read from memory request

    * Checks for virtual routine calls
    * Verifies memory source integrity
    * Performs read if necessary

    PARAMS
    dst_ptr   | Pointer to the memory that should be overwritten with read data
    src_addr  | Source address in vm memory space
    data_size | The size of the data to be read in bytes

*/
extern void mem_read(void* const dst_ptr, const int32_t src_addr, const int data_size);


// SYSTEM INITIALISER ...

/* Initialises the system
    Initialises, allocates, and links required system vars and methods
*/
extern void system_init();

/* Deinitialises the system
    Deallocates any malloc'd memory used for the system so that 
    it can safely be destroyed
*/
extern void system_deinit();


// END HEADER GUARD ...
#endif
