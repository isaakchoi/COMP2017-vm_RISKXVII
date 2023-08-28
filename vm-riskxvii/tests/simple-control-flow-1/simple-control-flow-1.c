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

// Invokes slt instruction
int inst_slt(int a, int b) {
    int result;
    asm volatile("slt %[res], %[a], %[rs2]"
        : [res]"=r"(result)
        : [rs2]"r"(b), [a]"r"(a));
    return result;
}

// Invokes slti instruction
int inst_slti(int a) {
    int result;
    asm volatile("slti %[res], %[a], 0"
        : [res]"=r"(result)
        : [a]"r"(a));
    return result;
}

// Invokes sltu instruction
int inst_sltu(int a, int b) {
    int result;
    asm volatile("sltu %[res], %[a], %[rs2]"
        : [res]"=r"(result)
        : [rs2]"r"(b), [a]"r"(a));
    return result;
}

// Invokes sltiu instruction
int inst_sltiu(int a) {
    int result;
    asm volatile("sltiu %[res], %[a], 1"
        : [res]"=r"(result)
        : [a]"r"(a));
    return result;
}

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

    // slt
    result = inst_slt(a, 0);
    print_string("slt (1 < 0): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");
    result = inst_slt(b, 0);
    print_string("slt (0 < 0): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");
    result = inst_slt(c, 0);
    print_string("slt (-1 < 0): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");

    // slti
    result = inst_slti(a);
    print_string("slti (1 < 0): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");
    result = inst_slti(b);
    print_string("slti (0 < 0): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");
    result = inst_slti(c);
    print_string("slti (-1 < 0): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");

    // sltu
    result = inst_sltu(a, 0);
    print_string("sltu (1 < 0): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");
    result = inst_sltu(0, 0);
    print_string("sltu (0 < 0): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");
    result = inst_sltu(0, 1);
    print_string("sltu (0 < 1): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");

    // sltiu
    result = inst_sltiu(0);
    print_string("sltiu (0 < 1): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");
    result = inst_sltiu(1);
    print_string("sltiu (1 < 1): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");
    result = inst_sltiu(2);
    print_string("sltiu (2 < 1): ");
    *(int*)WRITE_INT_ADDR = result;
    print_string("\n");

    // Return
    return 0;
}

