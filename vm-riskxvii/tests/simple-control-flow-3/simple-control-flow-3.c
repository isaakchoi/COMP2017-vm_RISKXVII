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

// Prints the given string to stdout
void print_string(char *str) {
    while (*str) *((char *)WRITE_CHAR_ADDR) = *(str++);
}

// Gets and returns an int from the user via stdin
int get_int() {
    return *((int*)READ_INT_ADDR);
}

// Main
int main(){

    // Get int from user
    print_string("Enter num from 0 to 15");
    int a = get_int();

    // Switch to find value
    switch (a) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            print_string("Your number was between the given values\n");
            break;
        default:
            print_string("Your number wasn't between the given values\n");
            break;
    }

    // Return
    return 0;
}

