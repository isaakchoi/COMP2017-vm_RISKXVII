// Name:   Isaak Choi
// UniKey: icho6322
// SID:    520488399


/* vm_riskxvii.c

    CONTAINS
    * Method to read input memory image binary file into the VM's memory
    * Main loop - Ties together each step of the CPU cycle with error checking

*/


// INCLUDE HEADER ...
#include "vm_riskxvii.h"


// FUNCTIONS ...

/* Reads in the memory image binary file.

    Data read from the file is saved into the global 'memory' array

    RETURNS 
    0     | On success
    n < 0 | A non-zero error code (n < 0) on failure
    
*/
int read_bin_file(int argc, char** argv) {

    // Ensure correct ammount of arguments given
    if (argc != REQUIRED_ARG_NUM) {
        printf("ERR: Invalid args\n");
        return ERR_INVALID_ARGS;
    }

    // Open specified binary file
    const char* fpath = argv[BIN_SRC_ARG_INDX];
    FILE* fptr;
    fptr = fopen(fpath, "rb"); // rb -> read binary

    // Ensure file opened successfully
    if (fptr == NULL) {
        printf("ERR: Couldn't open file \"%s\"\n", fpath);
        return ERR_COULDNT_OPEN;
    }

    // Ensure input file is correct size
    fseek(fptr, 0L, SEEK_END);
    if (ftell(fptr) != MEM_IMG_BIN_FILE_SIZE) {
        printf("ERR: Invalid file input size: [%ld bytes]\n", ftell(fptr));
        return ERR_INVALID_BIN_SIZE;
    }
    rewind(fptr);

    // Read in instructions
    const long n_ins_bytes_read = fread(memory, INST_MEM_SIZE, 1, fptr);

    // Read in data
    byte* const data_start_ptr = (byte* const)&memory + INST_MEM_SIZE;
    const long n_dat_bytes_read = fread(data_start_ptr, DATA_MEM_SIZE, 1, fptr);

    // Check error in reading
    if ((n_ins_bytes_read != 1) || (n_dat_bytes_read != 1)) {
        printf("ERR: Error reading from file\"%s\"\n", fpath);
        return ERR_READING_FILE;
    }

    // Close binary file
    if (fclose(fptr) == EOF) {
        printf("ERR: Couldn't close file \"%s\"\n", fpath);
        return ERR_COULDNT_CLOSE;
    };

    // Return no error
    return ERR_NO_ERR;
}


// MAIN ...

/* Main - Program entrypoint
    
    * Reads in memory image binary file and sets up vm memory
    * Executes loaded binary
    * Exits with any encountered error codes

    RETURNS
    0     | On success
    n < 0 | On error (Error code where n < 0)

*/
int main(int argc, char** argv) {

    // Variable to store extracted error codes for readability purposes
    int err;

    // Report on memory leaks at program exit
    #ifdef DEBUG_DETECT_LEAKS 
        atexit(report_mem_leak);
    #endif

    // Initialise the vm system
    system_init();
    err = get_system_error_code();
    if (err != ERR_NO_ERR) {
        return err;
    }

    // Read in memory image binary file
    err = read_bin_file(argc, argv);
    if (err != ERR_NO_ERR) {
        return err;
    }

    // Run binary on virtual machine
    do {

        // Get next instruction
        const uint32_t instr = get_instruction();

        // Stepper for debugging
        #ifdef DEBUG_STEP_THROUGH
            fflush(stdout);
            fgetc(stdin);
        #endif
        #ifdef DEBUG_PRINT_PC
            printf("PC    | 0x%08X\n", pc);
        #endif

        // Execute
        exec_instruction(instr);

        // Reset zero register to prevent values being stored there
        registers[ZERO_REGISTER_ADDR] = ZERO_REGISTER_VAL;

        // Check for error
        err = get_system_error_code();
        if (err != ERR_NO_ERR) {
            set_cpu_run_status(false);
            break;
        }

        // Check program counter bounds
        if (pc >= INST_MEM_SIZE || pc < 0) {
            throw_pc_out_of_bounds_err();
            err = get_system_error_code();
            break;
        }

    // Stop if CPU run state is false
    } while (get_cpu_run_status());

    // Deinitialise system - free any malloc'd memory
    system_deinit();

    // Ensure any buffered output is printed
    fflush(stdout);
    
    // Return with any caught errors
    return err;
}