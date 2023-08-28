// Isaak Choi
// 520488399
// icho6322

// Constants
#define ONE_OVER_PC_LIMIT "-1024"

// Function to cause pc overflow
int jump_under_pc_limit(){
    int result;
    asm volatile("jal %[res], " ONE_OVER_PC_LIMIT
        : [res]"=r"(result));
    return result;
}

int main(){

    // Invoke pc overflow
    jump_under_pc_limit();

    // Return
    return 0;
}

 