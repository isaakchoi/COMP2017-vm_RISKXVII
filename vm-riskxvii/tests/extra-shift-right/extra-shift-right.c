// Isaak Choi
// 520488399
// icho6322
// USYD CODE CITATION ACKNOWLEDGEMENT
// This file contains acknowledgements for ideas and/or code

// Constants
#define READ_INT_ADDR     (0x0816)
#define WRITE_UINT_ADDR   (0x0808)
#define WRITE_CHAR_ADDR   (0x0800)


/*
    USYD CODE CITATION ACKNOWLEDGEMENT
    I declare that the majority of the following code has been taken
    from the provided testcases of the assignent spec (from the compiler
    toolchain git repo).
*/


// Hex output write VR location
int volatile *const ConsoleWriteHex = (int *)WRITE_UINT_ADDR;

void print_string(char *str) {
    while (*str) *((char *)WRITE_CHAR_ADDR) = *(str++);
}

/*  End of copied code  */

int get_int() {
    return *((int*)READ_INT_ADDR);
}

int srl(int num, int shift_ammount) {
    int result;
    asm volatile("srl %[res], %[num], %[nbits]"
        : [res]"=r"(result)
        : [nbits]"r"(shift_ammount), [num]"r"(num));
    return result;
}


int main(){

    // Get num to shift
    print_string("Enter num to shift right: ");
    int a = get_int();
    print_string("\n");
    print_string("Enter ammount to shift right by: ");
    int b = get_int();
    print_string("\n");

    // Perform shift
    int c = srl(a, b);

    // Print output
    print_string("n1 shifted right by n2 places: ");
    *(int*)WRITE_UINT_ADDR = c;
    print_string("\n");

    // Return
    return 0;
}

