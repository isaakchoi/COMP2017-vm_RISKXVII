// Isaak Choi
// 520488399
// icho6322

// Constants
#define READ_INT_ADDR   (0x0816)
#define WRITE_UINT_ADDR (0x0808)
#define WRITE_CHAR_ADDR (0x0800)
#define MALLOC_ADDR     (0x0830)
#define FREE_ADDR       (0x0834)
#define READ_CHAR_ADDR  (0x0812)
#define WRITE_INT_ADDR  (0x0804)

// Other functions
void print_string(char *str) {
    while (*str) *((char *)WRITE_CHAR_ADDR) = *(str++);
}

// Main
int main(){

    // Set hard coded values
    int a = 1;
    int b = 0;
    int c = -1;
    int result;

    // beq
    if (a == 1) {
        print_string("beq (1 == 1): true");
    } else {
        print_string("beq (1 == 1): false");
    }
    print_string("\n");
    if (a == 0) {
        print_string("beq (1 == 0): true");
    } else {
        print_string("beq (1 == 0): false");
    }
    print_string("\n");

    // bne
    if (a != 1) {
        print_string("bne (1 != 1): true");
    } else {
        print_string("bne (1 != 1): false");
    }
    print_string("\n");
    if (a != 0) {
        print_string("bne (1 != 0): true");
    } else {
        print_string("bne (1 != 0): false");
    }
    print_string("\n");

    // blt
    if (a < 1) {
        print_string("blt (1 < 1): true");
    } else {
        print_string("blt (1 < 1): false");
    }
    print_string("\n");
    if (a < 2) {
        print_string("blt (1 < 2): true");
    } else {
        print_string("blt (1 < 2): false");
    }
    print_string("\n");
    if (a < 0) {
        print_string("blt (1 < 0): true");
    } else {
        print_string("blt (1 < 0): false");
    }
    print_string("\n");

    // bltu
    unsigned int ua = 1;
    if (ua < 0) {
        print_string("bltu (1 < 0): true");
    } else {
        print_string("bltu (1 < 0): false");
    }
    print_string("\n");
    if (ua < 1) {
        print_string("bltu (1 < 1): true");
    } else {
        print_string("bltu (1 < 1): false");
    }
    print_string("\n");
    if (ua < 2) {
        print_string("bltu (1 < 2): true");
    } else {
        print_string("bltu (1 < 2): false");
    }
    print_string("\n");

    // bge
    if (a >= 0) {
        print_string("bge (1 >= 0): true");
    } else {
        print_string("bge (1 >= 0): false");
    }
    print_string("\n");
    if (a >= 1) {
        print_string("bge (1 >= 1): true");
    } else {
        print_string("bge (1 >= 1): false");
    }
    print_string("\n");
    if (a >= 2) {
        print_string("bge (1 >= 2): true");
    } else {
        print_string("bge (1 >= 2): false");
    }
    print_string("\n");

    // bgeu
    if (ua >= 0) {
        print_string("bgeu (1 >= 0): true");
    } else {
        print_string("bgeu (1 >= 0): false");
    }
    print_string("\n");
    if (ua >= 1) {
        print_string("bgeu (1 >= 1): true");
    } else {
        print_string("bgeu (1 >= 1): false");
    }
    print_string("\n");
    if (ua >= 2) {
        print_string("bgeu (1 >= 2): true");
    } else {
        print_string("bgeu (1 >= 2): false");
    }
    print_string("\n");

    // Return
    return 0;
}

