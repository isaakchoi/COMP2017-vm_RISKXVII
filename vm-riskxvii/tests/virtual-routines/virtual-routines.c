// Isaak Choi
// 520488399
// icho6322

// Constants
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


////////////////////////////////////////////////////////////


// Get the address of the malloced memory
int get_malloc_addr(){
    int result;
    asm volatile("addi %[res], t3, 0"
        : [res]"=r"(result));
    return result;
}

// Print string to srdout
void print_string(char *str) {
    while (*str) *((char *)VR_WRITE_CHAR_ADDR) = *(str++);
}


////////////////////////////////////////////////////////////


// Read int - DONE
int get_int() {
    return *((int*)VR_READ_INT_ADDR);
}

// Read char - DONE
char get_char() {
    return *((int*)VR_READ_CHAR_ADDR);
}

// Write char - DONE
void write_char(char c) {
    *(int*)VR_WRITE_CHAR_ADDR = c; 
}

// Write int - DONE
void write_int(int n) {
    *(int*)VR_WRITE_INT_ADDR = n;
}

// Write uint - DONE
void write_uint(unsigned int n) {
    *(int*)VR_WRITE_UINT_ADDR = n;
}

// Halt - DONE
void halt() {
    *(int*)VR_HALT_ADDR = 1;
}

// Dump pc - DONE
void dump_pc() {
    *(int*)VR_DUMP_PC_ADDR = 1;
}

// Dump registers - DONE
void dump_reg() {
    *(int*)VR_DUMP_REG_ADDR = 1;
}

// Dump mem word - DONE
void dump_word(int addr) {
    *(int*)VR_DUMP_MEM_WORD_ADDR = addr;
}

// malloc - DONE
void vr_malloc(int size) {
    *(int*)VR_HEAP_BANK_MALLOC_ADDR = size;
}

// free - DONE
void vr_free(int addr) {
    *(int*)VR_HEAP_BANK_FREE_ADDR = addr;
}


// Main
int main(){


    print_string("Enter single char: ");
    char c = get_char();

    print_string("\nEnter single int: ");
    int n = get_int();

    print_string("\nGiven char: ");
    write_char(c);

    print_string("\nGiven int: ");
    write_int(n);

    print_string("\nGiven int as uint: ");
    write_uint(n);

    print_string("\nDumping pc:\n");
    dump_pc();

    print_string("\nDumping registers:\n");
    dump_reg();

    print_string("\nDumping memory word at address of given int:\n");
    dump_word((int)&n);

    print_string("\nmalloc'ing 128 individual banks ...\n");
    vr_malloc(1);
    int first_malloc_addr = get_malloc_addr();
    for (int i = 0; i < 127; i++) {
        vr_malloc(1);
    }
    print_string("DONE\n");

    print_string("free'ing all malloc banks ...\n");
    for (int i = 0; i < 128; i++) {
        #define BANK_SIZE_BYTES (64)
        vr_free(first_malloc_addr);
        first_malloc_addr += BANK_SIZE_BYTES;
    }
    print_string("DONE\n");

    print_string("HALTING ...\n");
    halt();
    print_string("This printed after halt >:(\n");

    // Return
    return 0;
}

