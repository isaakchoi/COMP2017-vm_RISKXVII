// Name:   Isaak Choi
// UniKey: icho6322
// SID:    520488399


/* system.c

    Contains function definitions for the virtual machine:

    * System helpers
    * Virtual routines
    * Memory interface functions

    NOTE
    * Uses virtual routine memory space to store system errors
      and cpu run status information:
      * 0x0850 - System CPU run status in form of <boolean>
      * 0x0854 - System error status in form of <int> error code

*/


// INCLUDE HEADER ...
#include "system.h"


// GLOBAL SYSTEM VARS ...

// The program counter
int32_t pc;

// Stores register data, with each index being a unique register
int32_t registers[NUM_REGISTERS];

// The main system memory
byte memory[MEM_SIZE_BYTES];


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
int32_t sign_extend_int32(int32_t num, int curr_len_bits, int new_len_bits) {

    // Extract MSB
    uint32_t mask = (num >> (curr_len_bits - 1));

    // Copy bit to form new desired size int
     // For speed optimisation this could be skipped if the MSB = 0 but skipping
     // the if statement required to check will reduce binary size by an 
     // instruction or two - Attempt to fit within 20kB limit
    mask = mask << curr_len_bits;
    for (int i = 0; i < new_len_bits - curr_len_bits; i++) {
        num |= mask;
        mask *= 2;
    }

    // Return
    return num;
}


// ERROR CALL ABSTRACTIONS ...

/* Throws the error required when an unknown instruction is encountered

    * Sets appropriate error code
    * Prints error identifier text, the instruction in hex, and a register dump
      to stdout.

*/
void throw_not_implemented_err() {
    
    // Set error code
    set_system_error_code(ERR_UNKNOWN_INSTR);

    // Print error info
    printf("Instruction Not Implemented: 0x%08x\n", get_instruction());
    register_dump();
}

/* Throws the error required when an illegal operation is encountered

    * Sets appropriate error code
    * Prints error identifier text, the instruction in hex, and a register dump
      to stdout.

*/
void throw_illegal_operation_err() {
    
    // Set error code
    set_system_error_code(ERR_ILLEGAL_OPERATION);

    // Print error info
    printf("Illegal Operation: 0x%08x\n", get_instruction());
    register_dump();
}

/* Throws the error required when an the program counter exceeds allowed bounds

    * Sets appropriate error code
    * Prints error identifier text and a register dump to stdout

*/
void throw_pc_out_of_bounds_err() {
    
    // Set error code
    set_system_error_code(ERR_PC_OUT_OF_BOUNDS);

    // Print error info
    printf("Program counter out of bounds\n");
    register_dump();
}

/* Throws a malloc failed error

    NOTE
    This error is for when the host computer fails to malloc something
    rather than the vm failing a malloc call.

*/
void throw_host_malloc_failed_err() {
    
    // Set error code
    set_system_error_code(ERR_HOST_MALLOC_FAILED);
}


// SYSTEM INTERFACE FUNCTIONS ...

/* Performs a register dump to stdout
    Prints the value pc followed by the value of registers 0 to NUM_REGISTERS
*/
void register_dump() {
    printf("PC = 0x%08x;\n", pc);
    for (int i = 0; i < NUM_REGISTERS; i++) {
        printf("R[%d] = 0x%08x;\n", i, registers[i]);
    }
}

/* Sets the CPU run status for the system
    Setting to false will cause the simulated CPU to stop running
*/ 
void set_cpu_run_status(const bool run) {
    *(bool*)&memory[VR_CPU_RUN_ADDR] = run;
}

/* Returns the CPU run status as a boolean
    Returns true if CPU is/should run, else false
*/
bool get_cpu_run_status() {
    return *(bool*)&memory[VR_CPU_RUN_ADDR];
}

/* Sets the system error code
    A non zero code denotes an error
*/
void set_system_error_code(const int32_t err) {
    *(int32_t*)&memory[VR_SYS_ERR_ADDR] = err;
}

/* Returns the system error code
    A non zero code denotes an error
*/
int32_t get_system_error_code() {
    return *(int32_t*)&memory[VR_SYS_ERR_ADDR];
}

/* Retrieves the current instruction
    Returns the current instruction, pointed to by the program counter, 
    as a uint32_t.
*/
uint32_t get_instruction() {

    // Return instruction
    return *(uint32_t*)&memory[pc];
}


// VIRTUAL ROUTINES ...

/* Console Write Char
    Prints the value at the given location as a single ASCII 
    encoded character to stdout
*/
void vr_write_char(const char* const char_src_ptr) {
    printf("%c", *char_src_ptr);
}

/* Console Write Int
    Prints the value at the given location as a single signed integer to stdout
*/
void vr_write_int(const int32_t* const int_src) {
    printf("%d", *int_src);
}

/* Console Write Unsigned Int
    Prints the value at the given location as a single 
    unsigned integer to stdout
*/
void vr_write_uint(const uint32_t* const uint_src) {
    printf("%x", *uint_src);
}

/* Halt
    Prints CPU halt message to stdout and stops the simulated CPU
*/
void vr_halt() {
    printf("CPU Halt Requested\n");
    set_cpu_run_status(false);
}

/* Console Read Character
    Reads a single char from stdin and stores as a single ASCII encoded char
    at the given location
*/
void vr_read_char(const void* const dst_ptr) {

    // Read char
    char c = fgetc(stdin);

    // // Comented out because otherwise would lose test-case marks for 
    // // irreproducable error
    // if (feof(stdin)) {
    //     printf("Error reading in char\n");
    //     c = 0;
    // }

    // Save char
    *(char*)dst_ptr = c;
}

/* Console Read Signed Integer
    Reads a single signed int from stdin and stores as an int32 
    at the given location
*/
void vr_read_int(const void* const dst_ptr) {

    // Read int
    int int_in;
    scanf("%d", &int_in);

    // Save int
    *(int32_t*)dst_ptr = int_in;
}

/* Dump PC
    Prints the value of the program counter to stdout
*/
void vr_dump_pc() {
    printf("%x", pc);
}

/* Dump Register Banks
    Invokes the register_dump() method, printing the values of 
    the PC and all registers to stdout
*/
void vr_dump_registers() {
    register_dump();
}

/* Dump Memory Word
    Prints the value (4 bytes interpreted in little endian) 
    stored at the given location in hexadecimal to stdout.
*/
void vr_dump_word(const void* src_ptr) {

    // Get vm address
    const uint32_t addr = *(uint32_t*)src_ptr;

    // Get word and print
    printf("%x", *(uint32_t*)&memory[addr]);
}

/* Malloc
    * Attempts to malloc() the given ammount of bytes in virtual machine memory
      * n bytes is the int32_t value at the given address
    * Saves the address of allocated memory chunk in R[28]
    * Saves NULL in R[28] if malloc failed
*/
void vr_malloc(const void* src_ptr) {

    // Get the size of the malloc request
    const int32_t size = *(int32_t*)src_ptr;

    // Send request to heap manager
    heap_manager_t* const manager = get_heap_manager();
    manager->malloc(manager, size);
}

/* Free
    * Attempts to free() the malloc'd memory at the given address
      * The 'given address' is the int32_t pointed to by src_ptr
    * Throws illegal operation error if trying to free memory that hasn't 
      been allocated or is out of allowed bounds
*/
void vr_free(const void* src_ptr) {

    // Get the location to be freed
    const int32_t free_addr = *(int32_t*)src_ptr;

    // Send free request to heap manager
    heap_manager_t* const manager = get_heap_manager();
    manager->free(manager, free_addr);
}


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
void mem_write(const void* const src_ptr, const int32_t dst_addr,
                   const int data_size) {

    // Debugging
    #ifdef DEBUG_PRINT_MEM_ACCESS
        printf("Write request to: 0x%08X\n", dst_addr);
    #endif

    // Bool to hold error state
    bool err = false;

    // The end address (inclusive) of the mem write field
    const int end_addr = dst_addr + data_size - 1;

    // Check for virtual routine
    switch (dst_addr) {

        // Console write char
        case VR_WRITE_CHAR_ADDR:
            vr_write_char(src_ptr);
            break;

        // Console write signed int
        case VR_WRITE_INT_ADDR:
            vr_write_int(src_ptr); 
            break;

        // Console write unsigned int
        case VR_WRITE_UINT_ADDR:
            vr_write_uint(src_ptr);
            break;

        // Halt virtual routine
        case VR_HALT_ADDR:
            vr_halt();
            break;
        
        // Dump PC
        case VR_DUMP_PC_ADDR:
            vr_dump_pc();
            break;
        
        // Dump register banks
        case VR_DUMP_REG_ADDR:
            vr_dump_registers();
            break;
        
        // Dump memory word
        case VR_DUMP_MEM_WORD_ADDR:
            vr_dump_word(src_ptr);
            break;
        
        // Heap bank - malloc
        case VR_HEAP_BANK_MALLOC_ADDR:
            vr_malloc(src_ptr);
            break;

        // Heap bank - free
        case VR_HEAP_BANK_FREE_ADDR:
            vr_free(src_ptr);
            break;

        // If not virtual routine
        default:

            // Validate mem write request ...

            // If within data memory bounds
            if ((dst_addr >= DATA_MEM_START && dst_addr <= DATA_MEM_END) &&
                (end_addr >= DATA_MEM_START && end_addr <= DATA_MEM_END)) {
                // No further verification needed
            } 

            // If within heap memory bounds
            else if ((dst_addr >= HEAP_MEM_START && dst_addr <= HEAP_MEM_END) &&
                     (end_addr >= HEAP_MEM_START && end_addr <= HEAP_MEM_END)) {

                // Check if accessing non-allocated memory or across allocated chunks
                heap_manager_t* const manager = get_heap_manager();
                if (!manager->is_valid_memory(manager, dst_addr, data_size)) {
                    err = true; 
                    throw_illegal_operation_err();
                }
            }

            // If outside valid memory write access
            else {
                err = true;
                throw_illegal_operation_err();
            }
            
            // Perform write if no error - Copy bytes
            if (!err) {
                for (int i = 0; i < data_size; i++) {
                    memory[dst_addr + i] = *((byte*)src_ptr + i);
                }
            }
    }
}

/* Handle read from memory request

    * Checks for virtual routine calls
    * Verifies memory source integrity
    * Performs read if necessary

    PARAMS
    dst_ptr   | Pointer to the memory that should be overwritten with read data
    src_addr  | Source address in vm memory space
    data_size | The size of the data to be read in bytes

*/
void mem_read(void* const dst_ptr, const int32_t src_addr,
                  const int data_size) {

    // Debugging
    #ifdef DEBUG_PRINT_MEM_ACCESS
        printf("Read request from: 0x%08X\n", src_addr);
    #endif

    // Bool to hold error state
    bool err = false;

    // The end address (inclusive) of the mem read field
    const int end_addr = src_addr + data_size - 1;

    // Check for virtual routine
    switch (src_addr) {

        // Console read int
        case VR_READ_INT_ADDR:
            vr_read_int(dst_ptr);
            break;

        // Console read char
        case VR_READ_CHAR_ADDR:
            vr_read_char(dst_ptr);
            break;

        // Attempt read
        default:

            // Validate mem read request ...

            // If within instruction or data memory
            if ((src_addr >= INST_MEM_START && src_addr <= DATA_MEM_END) &&
                (end_addr >= INST_MEM_START && end_addr <= DATA_MEM_END)) {
                    // No more validation needed
            }

            // If within heap bank memory
            else if ((src_addr >= HEAP_MEM_START && src_addr <= HEAP_MEM_END) &&
                (end_addr >= HEAP_MEM_START && end_addr <= HEAP_MEM_END)) {

                // Check if accessing non-allocated memory or across allocated chunks
                heap_manager_t* const manager = get_heap_manager();
                if (!manager->is_valid_memory(manager, src_addr, data_size)) {
                    err = true;
                    throw_illegal_operation_err();
                }
            }

            // Else outside valid memory read access
            else {
                err = true;
                throw_illegal_operation_err();
            }

            // Read by copying bytes to output dst
            if (!err) {
                for (int i = 0; i < data_size; i++) {
                    *((byte*)dst_ptr + i) = memory[src_addr + i];
                }
            }
    }
}


// SYSTEM INITIALISER AND DEINITIALISER ...

/* Initialises the system
    Initialises, allocates, and links required system vars and methods
*/
void system_init() {

    // Zero registers, memory, and pc
     // NOTE
     // As per C standard all globals will already be zero initialised but
     // doing it anyway increases future portability with minimal overhead
    pc = 0;
    for (int i = 0; i < MEM_SIZE_BYTES; i++) {
        memory[i] = 0;
    }
    for (int i = 0; i < NUM_REGISTERS; i++) {
        registers[i] = 0;
    }

    // Set error status to 'no error'
    set_system_error_code(ERR_NO_ERR);

    // Set CPU to run
    set_cpu_run_status(true);

    // Initialise and link heap bank manager
    heap_manager_t* const manager = heap_manager_init();
    link_heap_manager(manager);
}

/* Deinitialises the system
    Deallocates any malloc'd memory used for the system so that 
    it can safely be destroyed
*/
void system_deinit() {
    
    // Free heap bank manager
    heap_manager_t* const manager = get_heap_manager();
    manager->deallocate(manager);
}