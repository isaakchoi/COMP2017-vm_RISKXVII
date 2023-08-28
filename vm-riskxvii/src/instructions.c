// Name:   Isaak Choi
// UniKey: icho6322
// SID:    520488399


/* instructions.c

    Contains basic macros, constants, data types, and function relating to 
    the anatomy, use of, and handling of instructions within the vm.

*/


// INCLUDE HEADER ...
#include "instructions.h"


// FUNCTIONS TO EXTRACT BIT FIELDS FROM INSTRUCTIONS ...

/* Returns the opcode bits of the given instruction,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_opcode_bits(const int32_t instr) {
    return ((instr & OPCODE_EXTRACT_MASK) >> OPCODE_OFFSET_BITS);
}

/* Returns the bits within the rd field of the function,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_rd_bits(const int32_t instr) {
    return ((instr & RD_EXTRACT_MASK) >> RD_OFFSET_BITS);
}

/* Returns the bits within the func3 field of the function,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_func3_bits(const int32_t instr) {
    return ((instr & FUNC3_EXTRACT_MASK) >> FUNC3_OFFSET_BITS);
}

/* Returns the bits within the rs1 field of the function,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_rs1_bits(const int32_t instr) {
    return ((instr & RS1_EXTRACT_MASK) >> RS1_OFFSET_BITS);
}

/* Returns the bits within the rs2 field of the function,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_rs2_bits(const int32_t instr) {
    return ((instr & RS2_EXTRACT_MASK) >> RS2_OFFSET_BITS);
}

/* Returns the bits within the func7 field of the function,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_func7_bits(const int32_t instr) {
    return ((instr & FUNC7_EXTRACT_MASK) >> FUNC7_OFFSET_BITS);
}


// FUNCTIONS TO UNPACK ENTIRE INSTRUCTIONS INTO ALLOCATED FIELDS ...

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using R type format.
*/
void extract_R(instruction_t* const instr, int32_t raw_instr) {

    // Unpack and save simple data fields
    instr->type_R.opcode = get_opcode_bits(raw_instr);
    instr->type_R.rd = get_rd_bits(raw_instr);
    instr->type_R.func3 = get_func3_bits(raw_instr);
    instr->type_R.rs1 = get_rs1_bits(raw_instr);
    instr->type_R.rs2 = get_rs2_bits(raw_instr);
    instr->type_R.func7 = get_func7_bits(raw_instr);
}

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using I type format.
*/
void extract_I(instruction_t* const instr, int32_t raw_instr) {

    // Unpack and save simple data fields
    instr->type_I.opcode = get_opcode_bits(raw_instr);
    instr->type_I.rd = get_rd_bits(raw_instr);
    instr->type_I.func3 = get_func3_bits(raw_instr);
    instr->type_I.rs1 = get_rs1_bits(raw_instr);
     
    // Extract imm bitfield and shift to least-significant side of int32_t
      // Shift using parallel default field masks
    int32_t imm = (raw_instr >> RS2_OFFSET_BITS);

    // Convert from length of 12 bits to length of 32 bits
    imm = sign_extend_int32(imm, INST_TYPE_I_IMM_ORI_LEN, REGISTER_SIZE_BITS);

    // Save to imm field
    instr->type_I.imm = imm;
}

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using S type format.
*/
void extract_S(instruction_t* const instr, int32_t raw_instr) {

    // Unpack and save simple data fields
    instr->type_S.opcode = get_opcode_bits(raw_instr);
    instr->type_S.func3 = get_func3_bits(raw_instr);
    instr->type_S.rs1 = get_rs1_bits(raw_instr);
    instr->type_S.rs2 = get_rs2_bits(raw_instr);

    // Extract and shift imm bitfields into place on least-significant side
      // Construct extract mask and shift using parallel default field masks
    const int32_t imm_field_1 = (
        (raw_instr & FUNC7_EXTRACT_MASK) >> (FUNC7_OFFSET_BITS - RD_SIZE_BITS));

    const int32_t imm_field_2 = ((raw_instr & RD_EXTRACT_MASK) >> RD_OFFSET_BITS);

    // Convert from 12 bit int to 32 bit int and save
    instr->type_S.imm = sign_extend_int32((imm_field_1 | imm_field_2),
        INST_TYPE_S_IMM_ORI_LEN, REGISTER_SIZE_BITS);
}

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using SB type format.
*/
void extract_SB(instruction_t* const instr, int32_t raw_instr) {
    
    // Unpack and save simple data fields
    instr->type_SB.opcode = get_opcode_bits(raw_instr);
    instr->type_SB.func3 = get_func3_bits(raw_instr);
    instr->type_SB.rs1 = get_rs1_bits(raw_instr);
    instr->type_SB.rs2 = get_rs2_bits(raw_instr);

    // Extract imm bitfields
    int32_t imm = (
        ((raw_instr & INST_TYPE_SB_IMM_BIT_11_MASK)   // Extract [11] bit
            << INST_TYPE_SB_IMM_BIT_11_OFFSET) |      // Shift into place
        ((raw_instr & INST_TYPE_SB_IMM_BIT_4_1_MASK)  // Extract [4:1] bits
            >> INST_TYPE_SB_IMM_BIT_4_1_OFFSET) |     // Shift into place
        ((raw_instr & INST_TYPE_SB_IMM_BIT_10_5_MASK) // Extract [10:5] bits
            >> INST_TYPE_SB_IMM_BIT_10_5_OFFSET) |    // Shift into place
        ((raw_instr & INST_TYPE_SB_IMM_BIT_12_MASK)   // Extract [12] bit
            >> INST_TYPE_SB_IMM_BIT_12_OFFSET)
        );

    // Extend from 12 to 32 bit
    imm = sign_extend_int32(imm, INST_TYPE_SB_IMM_ORI_LEN, REGISTER_SIZE_BITS);

    // Save
    instr->type_SB.imm = imm;
}

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using U type format.
*/
void extract_U(instruction_t* const instr, int32_t raw_instr) {

    // Unpack and save simple data fields
    instr->type_U.opcode = get_opcode_bits(raw_instr);
    instr->type_U.rd = get_rd_bits(raw_instr);

    // Extract and save - No shift required
    instr->type_U.imm = raw_instr & INST_TYPE_U_IMM_EXTRACT_MASK;
}

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using UJ type format.
*/
void extract_UJ(instruction_t* const instr, int32_t raw_instr) {

    // Unpack and save simple data fields
    instr->type_UJ.opcode = get_opcode_bits(raw_instr);
    instr->type_UJ.rd = get_rd_bits(raw_instr);

    // Extract, shift and combine imm bits
    int32_t imm = (
        ((raw_instr & INST_TYPE_UJ_IMM_BIT_20_MASK)    // Extract [20] bit
            >> INST_TYPE_UJ_IMM_BIT_20_OFFSET) |       // Shift into place
        ((raw_instr & INST_TYPE_UJ_IMM_BIT_10_1_MASK)  // Extract [10:1] bits
            >> INST_TYPE_UJ_IMM_BIT_10_1_OFFSET) |     // Shift into place
        ((raw_instr & INST_TYPE_UJ_IMM_BIT_11_MASK)    // Extract [11] bit
            >> INST_TYPE_UJ_IMM_BIT_11_OFFSET) |       // Shift into place
        ((raw_instr & INST_TYPE_UJ_IMM_BIT_19_12_MASK) // Extract [19:12] bit
            >> INST_TYPE_UJ_IMM_BIT_19_12_OFFSET)
        ); 

    // Extend to 32 bit two's compliment int
    imm = sign_extend_int32(imm, INST_TYPE_UJ_IMM_ORI_LEN, REGISTER_SIZE_BITS);

    // Save
    instr->type_UJ.imm = imm;
}


// FUNCTIONS TO EXECUTE INDIVIDUAL INSTRUCTION CALLS ...

// ARITHMETIC AND LOGIC OPERATIONS

/* Executes the 'add' instruction
    Uses the given 'instr' data to execute the 'add' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] + R[rs2]
*/
void exec_add(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("ADD   | R[0x%x]",
            instr->type_R.rd);
        printf(" = R[0x%x](0x%x) + R[0x%x](0x%x) = 0x%x\n",
            instr->type_R.rs1,
            registers[instr->type_R.rs1],
            instr->type_R.rs2,
            registers[instr->type_R.rs2],
            registers[instr->type_R.rs1] + registers[instr->type_R.rs2]);
    #endif

    // Execute instruction
    registers[instr->type_R.rd] = (
        registers[instr->type_R.rs1] + registers[instr->type_R.rs2]);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'addi' instruction
    Uses the given 'instr' data to execute the 'addi' instruction.
    Format    | I
    Operation | R[rd] = R[rs1] + imm
*/
void exec_addi(instruction_t* const instr) {
    
    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("ADDI  | R[0x%x] = ", 
            instr->type_I.rd);
        printf("R[0x%x](0x%x) + imm(0x%x) = 0x%x\n",
            instr->type_I.rs1, 
            registers[instr->type_I.rs1], 
            instr->type_I.imm,
            registers[instr->type_I.rs1] + instr->type_I.imm);
    #endif

    // Execute instruction
    registers[instr->type_I.rd] = (
        registers[instr->type_I.rs1] + instr->type_I.imm);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'sub' instruction
    Uses the given 'instr' data to execute the 'sub' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] - R[rs2]
*/
void exec_sub(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SUB\n");
    #endif

    // Execute instruction
    registers[instr->type_R.rd] = (
        registers[instr->type_R.rs1] - registers[instr->type_R.rs2]);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'lui' instruction
    Uses the given 'instr' data to execute the 'lui' instruction.
    Format    | U
    Operation | R[rd] = {31:12 = imm | 11:0 = 0}
*/
void exec_lui(instruction_t* const instr) {

    // NOTE
    // * instr->type_U.imm is already set to {31:12 = imm | 11:0 = 0} in
    //   instruction decoding step

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("LUI\n");
    #endif

    // Execute instruction
    registers[instr->type_U.rd] = instr->type_U.imm;

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'xor' instruction
    Uses the given 'instr' data to execute the 'xor' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] ˆ R[rs2]
*/
void exec_xor(instruction_t* const instr) {
    
    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("XOR\n");
    #endif

    // Execute instruction
    registers[instr->type_R.rd] = (
        registers[instr->type_R.rs1] ^ registers[instr->type_R.rs2]);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'xori' instruction
    Uses the given 'instr' data to execute the 'xori' instruction.
    Format    | I
    Operation | R[rd] = R[rs1] ˆ imm
*/
void exec_xori(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("XORI\n");
    #endif

    // Execute instruction
    registers[instr->type_I.rd] = (
        registers[instr->type_I.rs1] ^ instr->type_I.imm);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'or' instruction
    Uses the given 'instr' data to execute the 'or' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] | R[rs2]
*/
void exec_or(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("OR\n");
    #endif

    // Execute instruction
    registers[instr->type_R.rd] = (
        registers[instr->type_R.rs1] | registers[instr->type_R.rs2]); 

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'ori' instruction
    Uses the given 'instr' data to execute the 'ori' instruction.
    Format    | I
    Operation | R[rd] = R[rs1] | imm
*/
void exec_ori(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("ORI\n");
    #endif

    // Execute instruction
    registers[instr->type_I.rd] = (
        registers[instr->type_I.rs1] | instr->type_I.imm); 

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'and' instruction
    Uses the given 'instr' data to execute the 'and' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] & R[rs2]
*/
void exec_and(instruction_t* const instr) {
    
    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("AND\n");
    #endif

    // Execute instruction
    registers[instr->type_R.rd] = (
        registers[instr->type_R.rs1] & registers[instr->type_R.rs2]); 

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'andi' instruction
    Uses the given 'instr' data to execute the 'andi' instruction.
    Format    | I
    Operation | R[rd] = R[rs1] & imm
*/
void exec_andi(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("ANDI\n");
    #endif

    // Execute instruction
    registers[instr->type_I.rd] = (
        registers[instr->type_I.rs1] & instr->type_I.imm);    

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'sll' instruction
    Uses the given 'instr' data to execute the 'sll' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] « R[rs2]
*/
void exec_sll(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SLL\n");
    #endif

    // Get shift ammount
    const int32_t shift = registers[instr->type_R.rs2];

    // Set to null if invalid shift ammount - Negative or n(shift) > n(bits)
    if ((shift < 0) || (shift > REGISTER_SIZE_BITS)) {
        registers[instr->type_R.rd] = ZERO_REGISTER_VAL;
    }

    // Execute instruction if no error
    else {
        registers[instr->type_R.rd] = (
            (uint32_t)registers[instr->type_R.rs1] << shift);
    }

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'srl' instruction
    Uses the given 'instr' data to execute the 'srl' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] » R[rs2]
*/
void exec_srl(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SRL\n");
    #endif

    // Get shift ammount
    const int32_t shift = registers[instr->type_R.rs2];

    // Set to null if invalid shift ammount - Negative or n(shift) > n(bits)
    if ((shift < 0) || (shift > REGISTER_SIZE_BITS)) {
        registers[instr->type_R.rd] = ZERO_REGISTER_VAL;
    }

    // Execute instruction if no error
    else {

        // Cast to uint32_t to prevent implementation defined sign extension
        registers[instr->type_R.rd] = (
            (uint32_t)registers[instr->type_R.rs1] >> shift);
    }

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'sra' instruction
    Uses the given 'instr' data to execute the 'sra' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] » R[rs2]
*/
void exec_sra(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SRA\n");
    #endif

    // Get shift ammount
    int32_t shift = registers[instr->type_R.rs2];

    // Set to null if invalid shift ammount - Negative
    if (shift < 0) {
        registers[instr->type_R.rd] = ZERO_REGISTER_VAL;
    }

    // Execute instruction if no error
    else {

        // Crop shift ammount
        shift %= REGISTER_SIZE_BITS;

        // Find and combine bits post shift and bits removed by shift (wrapped bits)
        registers[instr->type_R.rd] = (

            // Shifted section - Cast to uint32_t to prevent sign extension
            ((uint32_t)registers[instr->type_R.rs1] >> shift) |

            // Wrapped bits
            (registers[instr->type_R.rs1] << (REGISTER_SIZE_BITS - shift)));

    }

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

// MEMORY ACCESS OPERATIONS

/* Executes the 'lb' instruction
    Uses the given 'instr' data to execute the 'lb' instruction.
    Format    | I
    Operation | R[rd] = sext(M[R[rs1] + imm])
*/
void exec_lb(instruction_t* const instr) {
    
    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("LB\n");
    #endif

    // Get byte from memory
    signed char raw_byte;
    const int32_t src_addr = registers[instr->type_I.rs1] + instr->type_I.imm;
    mem_read(&raw_byte, src_addr, 1);

    // Convert to size of WORD_SIZE_BITS by sign extension
    const int32_t extended_byte = raw_byte;

    // Save in specified register
    *(int32_t*)&registers[instr->type_I.rd] = extended_byte;

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'lh' instruction
    Uses the given 'instr' data to execute the 'lh' instruction.
    Format    | I
    Operation | R[rd] = sext(M[R[rs1] + imm])
*/
void exec_lh(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("LH\n");
    #endif

    // Get half from memory
    int16_t raw_half;
    const int32_t src_addr = registers[instr->type_I.rs1] + instr->type_I.imm;
    mem_read(&raw_half, src_addr, WORD_SIZE / 2);

    // Convert to size of WORD_SIZE_BITS by sign extension
    const int32_t extended_half = raw_half;

    // Save in specified register
    *(int32_t*)&registers[instr->type_I.rd] = extended_half;

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'lw' instruction
    Uses the given 'instr' data to execute the 'lw' instruction.
    Format    | I
    Operation | R[rd] = M[R[rs1] + imm]
*/
void exec_lw(instruction_t* const instr) { 

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("LW    | R[0x%x] = M[R[0x%x](0x%x) + imm(0x%x)] = 0x%x\n",
        instr->type_I.rd,
        instr->type_I.rs1,
        registers[instr->type_I.rs1],
        instr->type_I.imm,
        memory[registers[instr->type_I.rs1] + instr->type_I.imm]);
    #endif

    // Execute instruction
    int32_t* const dst_ptr = &registers[instr->type_I.rd];
    const int32_t src_addr = registers[instr->type_I.rs1] + instr->type_I.imm;
    mem_read(dst_ptr, src_addr, WORD_SIZE);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'lbu' instruction
    Uses the given 'instr' data to execute the 'lbu' instruction.
    Format    | I
    Operation | R[rd] = M[R[rs1] + imm]
*/
void exec_lbu(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
    int32_t rs1_val = registers[instr->type_I.rs1];
    #endif

    // Get byte from memory
    unsigned char raw_byte;
    const int32_t src_addr = registers[instr->type_I.rs1] + instr->type_I.imm;
    mem_read(&raw_byte, src_addr, 1);

    // Convert to size of WORD_SIZE_BITS by zero extension
    const uint32_t extended_byte = raw_byte;

    // Save in specified register
    *(uint32_t*)&registers[instr->type_I.rd] = extended_byte;

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("LBU   | R[0x%x] = M[R[0x%x](0x%x) + imm(0x%x)] = 0x%x\n",
            instr->type_I.rd,
            instr->type_I.rs1,
            rs1_val,
            instr->type_I.imm,
            registers[instr->type_I.rd]);
    #endif

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'lhu' instruction
    Uses the given 'instr' data to execute the 'lhu' instruction.
    Format    | I
    Operation | R[rd] = M[R[rs1] + imm]
*/
void exec_lhu(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("LHU\n");
    #endif

    // Get half from memory
    uint16_t raw_half;
    const int32_t src_addr = registers[instr->type_I.rs1] + instr->type_I.imm;
    mem_read(&raw_half, src_addr, WORD_SIZE / 2);

    // Convert to size of WORD_SIZE_BITS by zero extension
    const uint32_t extended_half = raw_half;

    // Save in specified register
    *(uint32_t*)&registers[instr->type_I.rd] = extended_half;

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'sb' instruction
    Uses the given 'instr' data to execute the 'sb' instruction.
    Format    | S
    Operation | M[R[rs1] + imm] = R[rs2]
*/
void exec_sb(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SB\n");
    #endif

    // Execute instruction
    const byte* const src_ptr = (byte*)&registers[instr->type_S.rs2];
    const int32_t dst_addr = registers[instr->type_S.rs1] + instr->type_S.imm;
    mem_write(src_ptr, dst_addr, 1);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'sh' instruction
    Uses the given 'instr' data to execute the 'sh' instruction.
    Format    | S
    Operation | M[R[rs1] + imm] = R[rs2]
*/
void exec_sh(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SH\n");
    #endif

    // Execute instruction
    const int16_t* const src_ptr = (int16_t*)&registers[instr->type_S.rs2];
    const int32_t dst_addr = registers[instr->type_S.rs1] + instr->type_S.imm;
    mem_write(src_ptr, dst_addr, WORD_SIZE / 2);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'sw' instruction
    Uses the given 'instr' data to execute the 'sw' instruction.
    Format    | S
    Operation | M[R[rs1] + imm] = R[rs2]
*/
void exec_sw(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SW    | M[R[0x%x](0x%x) + imm(0x%x)] = R[0x%x](0x%08x)\n",
            instr->type_S.rs1,
            registers[instr->type_S.rs1],
            instr->type_S.imm,
            instr->type_S.rs2,
            registers[instr->type_S.rs2]);
    #endif

    // Execute instruction
    const int32_t* const src_ptr = (int32_t*)&registers[instr->type_S.rs2];
    const int32_t dst_addr = registers[instr->type_S.rs1] + instr->type_S.imm;
    mem_write(src_ptr, dst_addr, WORD_SIZE);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

// PROGRAM FLOW OPERATIONS

/* Executes the 'slt' instruction
    Uses the given 'instr' data to execute the 'slt' instruction.
    Format    | R
    Operation | R[rd] = (R[rs1] < R[rs2]) ? 1 : 0
*/
void exec_slt(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SLT\n");
    #endif

    // Execute instruction
    registers[instr->type_R.rd] = (
        (registers[instr->type_R.rs1] < registers[instr->type_R.rs2]) ? 1 : 0);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'slti' instruction
    Uses the given 'instr' data to execute the 'slti' instruction.
    Format    | I
    Operation | R[rd] = (R[rs1] < imm) ? 1 : 0
*/
void exec_slti(instruction_t* const instr) {
    
    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SLTI\n");
    #endif

    // Execute instruction
    registers[instr->type_I.rd] = (
        (registers[instr->type_I.rs1] < instr->type_I.imm) ? 1 : 0);

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'sltu' instruction
    Uses the given 'instr' data to execute the 'sltu' instruction.
    Format    | R
    Operation | R[rd] = (R[rs1] < R[rs2]) ? 1 : 0
*/
void exec_sltu(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SLTU\n");
    #endif

    // Cast to uint32 to treat as an unsigned comparison
    const uint32_t rs1 = *(uint32_t*)&registers[instr->type_R.rs1];
    const uint32_t rs2 = *(uint32_t*)&registers[instr->type_R.rs2];

    // Execute instruction
    registers[instr->type_R.rd] = (rs1 < rs2) ? 1 : 0;

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'sltiu' instruction
    Uses the given 'instr' data to execute the 'sltiu' instruction.
    Format    | I
    Operation | R[rd] = (R[rs1] < imm) ? 1 : 0
*/
void exec_sltiu(instruction_t* const instr) {

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("SLTIU\n");
    #endif

    // Cast to uint32 to treat as an unsigned comparison
    const uint32_t rs1 = *(uint32_t*)&registers[instr->type_I.rs1];
    const uint32_t rs2 = *(uint32_t*)&instr->type_I.imm;

    // Execute instruction
    registers[instr->type_I.rd] = (rs1 < rs2) ? 1 : 0;

    // Increment program counter
    pc += DFLT_PC_INCREMENT;
}

/* Executes the 'beq' instruction
    Uses the given 'instr' data to execute the 'beq' instruction.
    Format    | SB
    Operation | if(R[rs1] == R[rs2]) then PC = PC + (imm « 1)
*/
void exec_beq(instruction_t* const instr) {

    // NOTE
    // * From spec: 'pc = pc + (instr->type_SB.imm << 1);'
    //   However, Bitshift '<< 1' already done during decoding

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("BEQ   |\n");
    #endif

    // Execute instruction
    if (registers[instr->type_SB.rs1] == registers[instr->type_SB.rs2]) {
        pc = pc + instr->type_SB.imm;
    }

    // Increment program counter if not branched
    else {
        pc += DFLT_PC_INCREMENT;
    }
}

/* Executes the 'bne' instruction
    Uses the given 'instr' data to execute the 'bne' instruction.
    Format    | SB
    Operation | if(R[rs1] != R[rs2]) then PC = PC + (imm « 1)
*/
void exec_bne(instruction_t* const instr) {

    // NOTE
    // * From spec: 'pc = pc + (instr->type_SB.imm << 1);'
    //   However, Bitshift '<< 1' already done during decoding

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("BNE\n");
    #endif

    // Execute instruction
    if (registers[instr->type_SB.rs1] != registers[instr->type_SB.rs2]) {
        pc = pc + instr->type_SB.imm;
    }

    // Increment program counter if not branched
    else {
        pc += DFLT_PC_INCREMENT;
    }
}

/* Executes the 'blt' instruction
    Uses the given 'instr' data to execute the 'blt' instruction.
    Format    | SB
    Operation | if(R[rs1] < R[rs2]) then PC = PC + (imm « 1)
*/
void exec_blt(instruction_t* const instr) {

    // NOTE
    // * From spec: 'pc = pc + (instr->type_SB.imm << 1);'
    //   However, Bitshift '<< 1' already done during decoding

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("BLT\n");
    #endif

    // Execute instruction
    if (registers[instr->type_SB.rs1] < registers[instr->type_SB.rs2]) {
        pc = pc + instr->type_SB.imm;
    }    
    
    // Increment program counter if not branched
    else {
        pc += DFLT_PC_INCREMENT;
    }
}

/* Executes the 'bltu' instruction
    Uses the given 'instr' data to execute the 'bltu' instruction.
    Format    | SB
    Operation | if(R[rs1] < R[rs2]) then PC = PC + (imm « 1)
*/
void exec_bltu(instruction_t* const instr) {

    // NOTE
    // * From spec: 'pc = pc + (instr->type_SB.imm << 1);'
    //   However, Bitshift '<< 1' already done during decoding

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("BLTU\n");
    #endif

    // Cast to uint32 to treat as unsigned
    const uint32_t rs1 = *(uint32_t*)&registers[instr->type_SB.rs1];
    const uint32_t rs2 = *(uint32_t*)&registers[instr->type_SB.rs2];

    // Execute instruction
    if (rs1 < rs2) {
        pc = pc + instr->type_SB.imm;
    }

    // Increment program counter if not branched
    else {
        pc += DFLT_PC_INCREMENT;
    }
}

/* Executes the 'bge' instruction
    Uses the given 'instr' data to execute the 'bge' instruction.
    Format    | SB
    Operation | if(R[rs1] >= R[rs2]) then PC = PC + (imm « 1)
*/
void exec_bge(instruction_t* const instr) {

    // NOTE
    // * From spec: 'pc = pc + (instr->type_SB.imm << 1);'
    //   However, Bitshift '<< 1' already done during decoding

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("BGE\n");
    #endif

    // Execute instruction
    if (registers[instr->type_SB.rs1] >= registers[instr->type_SB.rs2]) {
        pc = pc + instr->type_SB.imm;
    }

    // Increment program counter if not branched
    else {
        pc += DFLT_PC_INCREMENT;
    }
}

/* Executes the 'bgeu' instruction
    Uses the given 'instr' data to execute the 'bgeu' instruction.
    Format    | SB
    Operation | if(R[rs1] >= R[rs2]) then PC = PC + (imm « 1)
*/
void exec_bgeu(instruction_t* const instr) {

    // NOTE
    // * From spec: 'pc = pc + (instr->type_SB.imm << 1);'
    //   However, Bitshift '<< 1' already done during decoding

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("BGEU\n");
    #endif

    // Cast to uint32 to treat as unsigned
    const uint32_t r1 = *(uint32_t*)&registers[instr->type_SB.rs1];
    const uint32_t r2 = *(uint32_t*)&registers[instr->type_SB.rs2];

    // Execute instruction
    if (r1 >= r2) {
        pc = pc + instr->type_SB.imm;
    } 
    
    // Increment program counter if not branched
    else {
        pc += DFLT_PC_INCREMENT;
    }
}

/* Executes the 'jal' instruction
    Uses the given 'instr' data to execute the 'jal' instruction.
    Format    | UJ
    Operation | R[rd] = PC + 4; PC = PC + (imm « 1)
*/
void exec_jal(instruction_t* const instr) {

    // NOTE
    // * From spec: 'pc = pc + (instr->type_UJ.imm << 1);'
    //   However, Bitshift '<< 1' already done during decoding
    // * '- DFLT_PC_INCREMENT' counteracts latter default pc increment
    //   to prevent over jump

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("JAL   | R[0x%x] = pc(0x%x) + 0x4 = 0x%x\n",
            instr->type_UJ.rd,
            pc,
            pc + DFLT_PC_INCREMENT);
        printf("      | pc = pc(0x%x) + imm(0x%x) = 0x%x\n",
            pc,
            instr->type_UJ.imm,
            pc + instr->type_UJ.imm);
    #endif

    // Save next pc into rd
    registers[instr->type_UJ.rd] = pc + DFLT_PC_INCREMENT;

    // Jump
    pc = pc + instr->type_UJ.imm;
}

/* Executes the 'jalr' instruction
    Uses the given 'instr' data to execute the 'jalr' instruction.
    Format    | I
    Operation | R[rd] = PC + 4; PC = R[rs1] + imm
*/
void exec_jalr(instruction_t* const instr) {

    // NOTE 
    // * '- DFLT_PC_INCREMENT' counteracts latter default pc increment
    //   to prevent over jump

    // Debugging output
    #ifdef DEBUG_PRINT_INSTRUCTION
        printf("JALR  |\n");
    #endif

    // Save next pc into rd
    registers[instr->type_I.rd] = pc + DFLT_PC_INCREMENT;

    // Jump
    pc = registers[instr->type_I.rs1] + instr->type_I.imm;
}


// INSTRUCTION PARSER AND EXECUTOR ...

/* Determines which instruction was given and executes accordingly

    Prints appropriate error message on error

    RETURNS 
    0  | On success
    -1 | On error

*/
void exec_instruction(const int32_t instr) {
    
    // Declare struct to store unpacked instruction data
    instruction_t parsed_instr;

    // add
    if (INSTR_MATCH(instr, ID_EXTRACT_MASK_3, ADD_ID_BITS)) {
        extract_R(&parsed_instr, instr);
        exec_add(&parsed_instr);
    } 

    // addi
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, ADDI_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_addi(&parsed_instr);
    } 
    
    // sub
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_3, SUB_ID_BITS)) {
        extract_R(&parsed_instr, instr);
        exec_sub(&parsed_instr);
    }

    // lui
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_1, LUI_ID_BITS)) {
        extract_U(&parsed_instr, instr);
        exec_lui(&parsed_instr);
    }

    // xor
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_3, XOR_ID_BITS)) {
        extract_R(&parsed_instr, instr);
        exec_xor(&parsed_instr);
    }

    // xori
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, XORI_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_xori(&parsed_instr);
    }

    // or
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_3, OR_ID_BITS)) {
        extract_R(&parsed_instr, instr);
        exec_or(&parsed_instr);
    }

    // ori
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, ORI_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_ori(&parsed_instr);
    }

    // and
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_3, AND_ID_BITS)) {
        extract_R(&parsed_instr, instr);
        exec_and(&parsed_instr);
    }

    // andi
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, ANDI_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_andi(&parsed_instr);
    }

    // sll
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_3, SLL_ID_BITS)) {
        extract_R(&parsed_instr, instr);
        exec_sll(&parsed_instr);
    }

    // srl
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_3, SRL_ID_BITS)) {
        extract_R(&parsed_instr, instr);
        exec_srl(&parsed_instr);
    }

    // sra
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_3, SRA_ID_BITS)) {
        extract_R(&parsed_instr, instr);
        exec_sra(&parsed_instr);
    }

    // lb
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, LB_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_lb(&parsed_instr);
    }

    // lh
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, LH_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_lh(&parsed_instr);
    }

    // lw
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, LW_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_lw(&parsed_instr);
    }

    // lbu
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, LBU_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_lbu(&parsed_instr);
    }

    // lhu
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, LHU_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_lhu(&parsed_instr);
    }

    // sb
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, SB_ID_BITS)) {
        extract_S(&parsed_instr, instr);
        exec_sb(&parsed_instr);
    }

    // sh
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, SH_ID_BITS)) {
        extract_S(&parsed_instr, instr);
        exec_sh(&parsed_instr);
    }

    // sw
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, SW_ID_BITS)) {
        extract_S(&parsed_instr, instr);
        exec_sw(&parsed_instr);
    }

    // slt
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_3, SLT_ID_BITS)) {
        extract_R(&parsed_instr, instr);
        exec_slt(&parsed_instr);
    }

    // slti
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, SLTI_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_slti(&parsed_instr);
    }

    // sltu 
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_3, SLTU_ID_BITS)) {
        extract_R(&parsed_instr, instr);
        exec_sltu(&parsed_instr);
    }

    // sltiu
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, SLTIU_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_sltiu(&parsed_instr);
    }

    // beq
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, BEQ_ID_BITS)) {
        extract_SB(&parsed_instr, instr);
        exec_beq(&parsed_instr);
    }

    // bne
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, BNE_ID_BITS)) {
        extract_SB(&parsed_instr, instr);
        exec_bne(&parsed_instr);
    }
    
    // blt
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, BLT_ID_BITS)) {
        extract_SB(&parsed_instr, instr);
        exec_blt(&parsed_instr);
    }

    // bltu
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, BLTU_ID_BITS)) {
        extract_SB(&parsed_instr, instr);
        exec_bltu(&parsed_instr);
    }
    
    // bge
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, BGE_ID_BITS)) {
        extract_SB(&parsed_instr, instr);
        exec_bge(&parsed_instr);
    }

    // bgeu
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, BGEU_ID_BITS)) {
        extract_SB(&parsed_instr, instr);
        exec_bgeu(&parsed_instr);
    }
    
    // jal
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_1, JAL_ID_BITS)) {
        extract_UJ(&parsed_instr, instr);
        exec_jal(&parsed_instr);
    }

    // jalr
    else if (INSTR_MATCH(instr, ID_EXTRACT_MASK_2, JALR_ID_BITS)) {
        extract_I(&parsed_instr, instr);
        exec_jalr(&parsed_instr);
    }

    // Error - Unknown instruction given
    else {
        throw_not_implemented_err();
    }
}
