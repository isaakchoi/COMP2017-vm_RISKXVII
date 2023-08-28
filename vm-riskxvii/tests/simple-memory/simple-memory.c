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

// Memory operation functions
// Not very readable, but forces compiler to perform 
// all load and store operations within its assembly
void inst_sb(char b) {
    return;
}

void inst_sh(short h) {
    return;
}

void inst_sw(int w) {
    return;
}

void inst_lb(signed char b) {
    return;
}

void inst_lhu(unsigned short h) {
    return;
}

int main(){

    // Get value in form of char from user
    print_string("Enter char to be used as byte: ");
    char c = *(char*)READ_CHAR_ADDR;
    print_string("\n");
    print_string("Given char: ");
    *(int*)WRITE_CHAR_ADDR = c;
    print_string("\n");

    // Some other data to store
    short h = 0xFFFF;
    int w = 0xFFFFFFFF;
    unsigned short unsigned_h = 0xFFFF;
    signed char signed_b = 0xFF;

    // Store byte
    print_string("Storing char as byte");
    inst_sb(c);
    print_string("\n");

    // Store half
    print_string("Storing 0xFFFF as half");
    inst_sh(h);
    print_string("\n");

    // Store word
    print_string("Storing 0xFFFFFFFF as word");
    inst_sw(w);
    print_string("\n");

    // Store unsigned half
    print_string("Storing 0xFFFF as unsigned half");
    inst_lhu(unsigned_h);
    print_string("\n");

    // Store signed byte
    print_string("Storing 0xFF as signed byte");
    inst_lb(signed_b);
    print_string("\n");

    // Return
    return 0;
}

