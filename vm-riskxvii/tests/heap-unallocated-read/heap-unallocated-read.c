// Isaak Choi
// 520488399
// icho6322

// Constants
#define READ_INT_ADDR       (0x0816)
#define WRITE_UINT_ADDR     (0x0808)
#define WRITE_CHAR_ADDR     (0x0800)
#define MALLOC_ADDR         (0x0830)
#define FREE_ADDR           (0x0834)

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

    // Get malloc location
    int malloc_location = get_malloc_addr();

    // // Write to allocated mem
    // for (int i = 0; i < 128; i++) {
    //     *(int*)(malloc_location + i) = i;
    // }

    // Write read and print char
    *(int*)malloc_location = 'X';
    char c = *(int*)malloc_location;
    *(int*)WRITE_CHAR_ADDR = c;

    // Free
    *(int*)FREE_ADDR = malloc_location;

    // Read from unallocated mem
    int some_random_offset = 53;
    int err = *(int*)(malloc_location + some_random_offset);

    // Return
    return 0;
}

