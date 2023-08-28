// Name:   Isaak Choi
// UniKey: icho6322
// SID:    520488399


/* vm_riskxvii.h

    CONTAINS
    * Macros and constants used by the vm_riskxvii.c source file

*/



// HEADER GUARD ...
#ifndef VM_RISKXVII_H
#define VM_RISKXVII_H


// DEPENDENCIES ...
#include <stdint.h>  // For int32_t
#include <stdbool.h> // For bool type
#include <stdio.h>   // For 
#include "instructions.h"
#include "system.h"
#include "utils.h"
#include "heap_manager.h"


// THIRD PARTY MEMORY LEAK DETECTOR ...
#ifdef DEBUG_DETECT_LEAKS
    #include "leak_detector_c.h"
#endif


// ERROR CODES ...
#define ERR_INVALID_ARGS     (-0x11) // Invalid command line arguments given
#define ERR_COULDNT_OPEN     (-0x12) // Couldn't open mem img binary file
#define ERR_COULDNT_CLOSE    (-0x13) // Couldn't close mem img binary fstream
#define ERR_INVALID_BIN_SIZE (-0x14) // Invalid memory image binary size
#define ERR_READING_FILE     (-0x15) // Read from file failed or invalid


// OTHER ...
#define REQUIRED_ARG_NUM (2) // Num of args required when running the program
#define BIN_SRC_ARG_INDX (1) // Index of the binary file path in the args array


// END HEADER GUARD ...
#endif