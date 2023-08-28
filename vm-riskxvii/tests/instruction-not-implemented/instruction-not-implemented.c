// Isaak Choi
// 520488399
// icho6322


/*

The following code produces a binary that includes an instruction
that hasn't been implemented. Thus, it can be used to test for
this error case.

*/



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
// Not readable, but forces compiler to perform operations

void inst_sb(char b) {
    return;
}

void inst_sh(short h) {
    return;
}

void inst_sw(int w) {
    return;
}

int inst_lb() {
    int result;
    asm volatile("addi %[res], t3, 0"
        : [res]"=r"(result));
    return result;
}

int inst_lh() {
    int result;
    asm volatile("addi %[res], t3, 0"
        : [res]"=r"(result));
    return result;
}

int inst_lw() {
    int result;
    asm volatile("addi %[res], t3, 0"
        : [res]"=r"(result));
    return result;
}

int inst_lbu() {

}

int inst_lhu() {

}

int main(){

    // // Get value in form of char from user
    // print_string("Enter char to be used as byte: ");
    // char c = *(char*)READ_CHAR_ADDR;
    // print_string("\n");
    // print_string("Given char: ");
    // *(int*)WRITE_CHAR_ADDR = c;
    // print_string("\n");

    char c = 'A';

    // Constants or bitshifting - Prevent slli and srli
    int shift_n_word = 24;
    int shift_n_half = 8;

    // Store byte
    inst_sb(c);

    // Store half
    inst_sh(c << shift_n_half);

    // Store word
    inst_sw(c << shift_n_word);

    // Return
    return 0;
}

