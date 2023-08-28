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

int sra(int num, int shift_ammount){
    int result;
    asm volatile("sra %[res], %[num], %[nbits]"
        : [res]"=r"(result)
        : [nbits]"r"(shift_ammount), [num]"r"(num));
    return result;
}

void print_string(char *str) {
    while (*str) *((char *)WRITE_CHAR_ADDR) = *(str++);
}

/*  End of copied code  */

int get_int() {
    return *((int*)READ_INT_ADDR);
}

int sll(int num, int shift_ammount) {
    return num << shift_ammount;
}

int srl(int num, int shift_ammount) {
    return num >> shift_ammount;
}


int main(){

    // Get input 

    print_string("Enter num to shift: ");
    int num_in = get_int();
    print_string("\n");

    print_string("Enter ammount to shift: ");
    int shift_ammount = get_int();
    print_string("\n");

    print_string("Enter shift ammount less than 0: ");
    int shift_under_ammount = get_int();
    print_string("\n");

    print_string("Enter shift ammount greater than 32: ");
    int over_shift_ammount = get_int();
    print_string("\n");


    // Perform default shifts

    print_string("\nShift using given normal ammount ...\n");

    print_string("Shift left: ");
    *ConsoleWriteHex = sll(num_in, shift_ammount);
    print_string("\n");

    print_string("Shift right: ");
    *ConsoleWriteHex = srl(num_in, shift_ammount);
    print_string("\n");

    print_string("Shift right wrap: ");
    *ConsoleWriteHex = sra(num_in, shift_ammount);
    print_string("\n\n");


    // Perform under shift

    print_string("\nShift using given < 0 ammount ...\n");

    print_string("Shift left negative: ");
    *ConsoleWriteHex = sll(num_in, shift_under_ammount);
    print_string("\n");
    
    print_string("Shift right negative: ");
    *ConsoleWriteHex = srl(num_in, shift_under_ammount);
    print_string("\n");

    print_string("Shift right wrap negative: ");
    *ConsoleWriteHex = sra(num_in, shift_under_ammount);
    print_string("\n\n");


    // Perform over shift

    print_string("\nShift using given over bounds ammount ...\n");

    print_string("Shift left over bounds: ");
    *ConsoleWriteHex = sll(num_in, over_shift_ammount);
    print_string("\n");

    print_string("Shift right over bounds: ");
    *ConsoleWriteHex = srl(num_in, over_shift_ammount);
    print_string("\n");

    print_string("Shift right wrap over bounds: ");
    *ConsoleWriteHex = sra(num_in, over_shift_ammount);
    print_string("\n\n");


    // Return
    return 0;
}

