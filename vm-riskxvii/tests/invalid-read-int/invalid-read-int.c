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

// Other functions
void print_string(char *str) {
    while (*str) *((char *)WRITE_CHAR_ADDR) = *(str++);
}

// Get int using virtual routine
volatile int get_int() {
    return *((int*)READ_INT_ADDR);
}

// Main
int main(){

    // Get invalid int
    for (int i = 0; i < 5; i++) {
        print_string("Enster invalid int: ");
        volatile int n = get_int();
        print_string("\n");
        print_string("Read result: ");
        *(int*)WRITE_INT_ADDR = n;
        print_string("\n");
    }

    // Return
    return 0;
}

