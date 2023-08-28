// Isaak Choi
// 520488399
// icho6322

// Constants
#define ONE_OVER_PC_LIMIT "-1024"

// Function to cause pc overflow
int inst_jalr(int rs1){
    int result;
    asm volatile("jalr %[res], %[rs1], 100"
        : [res]"=r"(result)
        : [rs1]"r"(rs1));
    return result;
}

int main(){

    // Invoke pc overflow
    int a = 0;
    inst_jalr(a);

    // Return
    return 0;
}

 