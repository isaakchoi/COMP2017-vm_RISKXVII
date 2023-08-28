// Name:   Isaak Choi
// UniKey: icho6322
// SID:    520488399
// USYD CODE CITATION ACKNOWLEDGEMENT
// This file contains acknowledgements of code


/* utils.h

    Contains helper / utility functions for use with other libraries.

*/


// HEADER GUARD ...
#ifndef UTILS_H
#define UTILS_H


// DEPENDENCIES ...
#include <stdint.h>


// GENERAL HELPER MACROS ...

// USYD CODE CITATION ACKNOWLEDGEMENT
// I declare that the majority of the following code has been taken
// from the website titled: 
// "Is there a printf converter to print in binary format?" 
// and it is not my own work. 
//  
// Original URL
// * https://stackoverflow.com/questions/111928/is-there-a-printf-
//   converter-to-print-in-binary-format
// Last access March, 2023
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
((byte) & 0x80 ? '1' : '0'), \
((byte) & 0x40 ? '1' : '0'), \
((byte) & 0x20 ? '1' : '0'), \
((byte) & 0x10 ? '1' : '0'), \
((byte) & 0x08 ? '1' : '0'), \
((byte) & 0x04 ? '1' : '0'), \
((byte) & 0x02 ? '1' : '0'), \
((byte) & 0x01 ? '1' : '0') 
// END OF COPIED CODE


// END HEADER GUARD ...
#endif