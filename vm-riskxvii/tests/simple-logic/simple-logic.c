// Isaak Choi
// 520488399
// icho6322

// Constants
#define READ_INT_ADDR       (0x0816)
#define WRITE_UINT_ADDR     (0x0808)
#define WRITE_CHAR_ADDR     (0x0800)
#define MALLOC_ADDR         (0x0830)
#define FREE_ADDR           (0x0834)
#define WRITE_INT_ADDR      (0x0804)

// Hex output write VR location
int volatile *const ConsoleWriteHex = (int*)WRITE_UINT_ADDR;

// Get the address of the malloced memory
int get_malloc_addr(){
    int result;
    asm volatile("addi %[res], t3, 0"
        : [res]"=r"(result));
    return result;
}

// Other functions
void print_string(char *str) {
    while (*str) *((char *)WRITE_CHAR_ADDR) = *(str++);
}

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

    // Some constants
    int alloc_size_bytes = 123;

    // Malloc
    *(int*)MALLOC_ADDR = alloc_size_bytes;

    // Get number to operate on 
    print_string("Enter number to do FANCY LOGIC on: ");
    int a = *(int*)READ_INT_ADDR;
    print_string("\n");
    print_string("Enter another number: ");
    int b = *(int*)READ_INT_ADDR;
    print_string("\n");

    // Do some logic
    int c;

    // xor
    print_string("a ^ b: ");
    c = a ^ b;
    *(int*)WRITE_UINT_ADDR = c;
    print_string("\n");

    // or
    print_string("a | b: ");
    c = a | b;
    *(int*)WRITE_UINT_ADDR = c;
    print_string("\n");

    // and
    print_string("a & b: ");
    c = a & b;
    *(int*)WRITE_UINT_ADDR = c;
    print_string("\n");

    // ori
    print_string("a | 0b01010101: ");
    c = a | 0b01010101;
    *(int*)WRITE_UINT_ADDR = c;
    print_string("\n");

    // andi
    print_string("a & 0b01010101: ");
    c = a & 0b01010101;
    *(int*)WRITE_UINT_ADDR = c;
    print_string("\n");

    // xori
    print_string("a ^ 0b01010101: ");
    c = a ^ 0b01010101;
    *(int*)WRITE_UINT_ADDR = c;
    print_string("\n");

    // Return
    return 0;
}

