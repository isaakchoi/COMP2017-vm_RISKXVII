// Name:   Isaak Choi
// UniKey: icho6322
// SID:    520488399


/* instructions.h

    Contains basic macros, constants, data types, and function relating to 
    the anatomy, use of, and handling of instructions within the vm.

*/


// HEADER GUARD ...
#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H


// DEPENDENCIES ...
#include <stdint.h>
#include "system.h"
#include "utils.h"


// THIRD PARTY MEMORY LEAK DETECTOR ...
#ifdef DEBUG_DETECT_LEAKS
    #include "leak_detector_c.h"
#endif


// INSTRUCTION ANATOMY INFO AND INSTRUCTION BITMASKS ...

// Instruction size
#define INST_SIZE_BYTES (4)
#define INST_SIZE_BITS  (INST_SIZE_BYTES * BYTE_SIZE_BITS)

// Size in bits of each field of an instruction
#define OPCODE_SIZE_BITS    (7)
#define RD_SIZE_BITS        (5)
#define FUNC3_SIZE_BITS     (3)
#define RS1_SIZE_BITS       (5)
#define RS2_SIZE_BITS       (5)
#define FUNC7_SIZE_BITS     (7)

// Offset in bits from least-significant side for each field of an instruction
#define OPCODE_OFFSET_BITS  (0)
#define RD_OFFSET_BITS      (OPCODE_OFFSET_BITS + OPCODE_SIZE_BITS)
#define FUNC3_OFFSET_BITS   (RD_OFFSET_BITS     + RD_SIZE_BITS)
#define RS1_OFFSET_BITS     (FUNC3_OFFSET_BITS  + FUNC3_SIZE_BITS)
#define RS2_OFFSET_BITS     (RS1_OFFSET_BITS    + RS1_SIZE_BITS)
#define FUNC7_OFFSET_BITS   (RS2_OFFSET_BITS    + RS2_SIZE_BITS)

// Bit masks representing each field of an instruction
#define OPCODE_EXTRACT_MASK (0x0000007F) // 0b00000000000000000000000001111111
#define RD_EXTRACT_MASK     (0x00000F80) // 0b00000000000000000000111110000000
#define FUNC3_EXTRACT_MASK  (0x00007000) // 0b00000000000000000111000000000000
#define RS1_EXTRACT_MASK    (0x000F8000) // 0b00000000000011111000000000000000
#define RS2_EXTRACT_MASK    (0x01F00000) // 0b00000001111100000000000000000000
#define FUNC7_EXTRACT_MASK  (0xFE000000) // 0b11111110000000000000000000000000

// Bit masks used to extract instruction identifiers
#define ID_EXTRACT_MASK_1  (OPCODE_EXTRACT_MASK)                    // opcode
#define ID_EXTRACT_MASK_2  (ID_EXTRACT_MASK_1 | FUNC3_EXTRACT_MASK) // ^ + func3
#define ID_EXTRACT_MASK_3  (ID_EXTRACT_MASK_2 | FUNC7_EXTRACT_MASK) // ^ + func7

// Multipliers to move bit masks from least-significant aligned to original pos
#define FUNC3_OFFSET_MULTIPLIER (0x1000)    // Shifts bits 7 places
#define FUNC7_OFFSET_MULTIPLIER (0x2000000) // Shifts bits 25 places


// INDIVIDUAL INSTRUCTION BIT INFO ...

// add
#define ADD_OPCODE      (0x33) // 0b00110011
#define ADD_FUNC3       (0x00) // 0b00000000
#define ADD_FUNC7       (0x00) // 0b00000000
#define ADD_ID_BITS     (ADD_OPCODE + \
                         ADD_FUNC3 * FUNC3_OFFSET_MULTIPLIER + \
                         ADD_FUNC7 * FUNC7_OFFSET_MULTIPLIER)

// addi
#define ADDI_OPCODE     (0x13) // 0b00010011
#define ADDI_FUNC3      (0x00) // 0b00000000
#define ADDI_ID_BITS    (ADDI_OPCODE + \
                         ADDI_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// sub
#define SUB_OPCODE      (0x33) // 0b00110011
#define SUB_FUNC3       (0x00) // 0b00000000
#define SUB_FUNC7       (0x20) // 0b00100000
#define SUB_ID_BITS     (SUB_OPCODE + \
                         SUB_FUNC3 * FUNC3_OFFSET_MULTIPLIER + \
                         SUB_FUNC7 * FUNC7_OFFSET_MULTIPLIER)

// lui
#define LUI_OPCODE      (0x37) // 0b00110111
#define LUI_ID_BITS     (LUI_OPCODE)

// xor
#define XOR_OPCODE      (0x33) // 0b00110011
#define XOR_FUNC3       (0x04) // 0b00000100
#define XOR_FUNC7       (0x00) // 0b00000000
#define XOR_ID_BITS     (XOR_OPCODE + \
                         XOR_FUNC3 * FUNC3_OFFSET_MULTIPLIER + \
                         XOR_FUNC7 * FUNC7_OFFSET_MULTIPLIER)

// xori
#define XORI_OPCODE     (0x13) // 0b00010011
#define XORI_FUNC3      (0x04) // 0b00000100
#define XORI_ID_BITS    (XORI_OPCODE + \
                         XORI_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// or
#define OR_OPCODE       (0x33) // 0b00110011
#define OR_FUNC3        (0x06) // 0b00000110
#define OR_FUNC7        (0x00) // 0b00000000
#define OR_ID_BITS      (OR_OPCODE + \
                         OR_FUNC3 * FUNC3_OFFSET_MULTIPLIER + \
                         OR_FUNC7 * FUNC7_OFFSET_MULTIPLIER)

// ori
#define ORI_OPCODE      (0x13) // 0b00010011
#define ORI_FUNC3       (0x06) // 0b00000110
#define ORI_ID_BITS     (ORI_OPCODE + \
                         ORI_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// and
#define AND_OPCODE      (0x33) // 0b00110011
#define AND_FUNC3       (0x07) // 0b00000111
#define AND_FUNC7       (0x00) // 0b00000000
#define AND_ID_BITS     (AND_OPCODE + \
                         AND_FUNC3 * FUNC3_OFFSET_MULTIPLIER + \
                         AND_FUNC7 * FUNC7_OFFSET_MULTIPLIER)

// andi
#define ANDI_OPCODE     (0x13) // 0b00010011
#define ANDI_FUNC3      (0x07) // 0b00000111
#define ANDI_ID_BITS    (ANDI_OPCODE + \
                         ANDI_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// sll
#define SLL_OPCODE      (0x33) // 0b00110011
#define SLL_FUNC3       (0x01) // 0b00000001
#define SLL_FUNC7       (0x00) // 0b00000000
#define SLL_ID_BITS     (SLL_OPCODE + \
                         SLL_FUNC3 * FUNC3_OFFSET_MULTIPLIER + \
                         SLL_FUNC7 * FUNC7_OFFSET_MULTIPLIER)

// srl
#define SRL_OPCODE      (0x33) // 0b00110011
#define SRL_FUNC3       (0x05) // 0b00000101
#define SRL_FUNC7       (0x00) // 0b00000000
#define SRL_ID_BITS     (SRL_OPCODE + \
                         SRL_FUNC3 * FUNC3_OFFSET_MULTIPLIER + \
                         SRL_FUNC7 * FUNC7_OFFSET_MULTIPLIER)

// sra
#define SRA_OPCODE      (0x33) // 0b00110011
#define SRA_FUNC3       (0x05) // 0b00000101
#define SRA_FUNC7       (0x20) // 0b00100000
#define SRA_ID_BITS     (SRA_OPCODE + \
                         SRA_FUNC3 * FUNC3_OFFSET_MULTIPLIER + \
                         SRA_FUNC7 * FUNC7_OFFSET_MULTIPLIER)

// lb
#define LB_OPCODE       (0x03) // 0b00000011
#define LB_FUNC3        (0x00) // 0b00000000
#define LB_ID_BITS      (LB_OPCODE + \
                         LB_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// lh
#define LH_OPCODE       (0x03) // 0b00000011
#define LH_FUNC3        (0x01) // 0b00000001
#define LH_ID_BITS      (LH_OPCODE + \
                         LH_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// lw
#define LW_OPCODE       (0x03) // 0b00000011
#define LW_FUNC3        (0x02) // 0b00000010
#define LW_ID_BITS      (LW_OPCODE + \
                         LW_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// lbu
#define LBU_OPCODE      (0x03) // 0b00000011
#define LBU_FUNC3       (0x04) // 0b00000100
#define LBU_ID_BITS     (LBU_OPCODE + \
                         LBU_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// lhu
#define LHU_OPCODE      (0x03) // 0b00000011
#define LHU_FUNC3       (0x05) // 0b00000101
#define LHU_ID_BITS     (LHU_OPCODE + \
                         LHU_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// sb
#define SB_OPCODE       (0x23) // 0b00100011
#define SB_FUNC3        (0x00) // 0b00000000
#define SB_ID_BITS      (SB_OPCODE + \
                         SB_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// sh
#define SH_OPCODE       (0x23) // 0b00100011
#define SH_FUNC3        (0x01) // 0b00000001
#define SH_ID_BITS      (SH_OPCODE + \
                         SH_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// sw
#define SW_OPCODE       (0x23) // 0b00100011
#define SW_FUNC3        (0x02) // 0b00000010
#define SW_ID_BITS      (SW_OPCODE + \
                         SW_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// slt
#define SLT_OPCODE      (0x33) // 0b00110011
#define SLT_FUNC3       (0x02) // 0b00000010
#define SLT_FUNC7       (0x00) // 0b00000000
#define SLT_ID_BITS     (SLT_OPCODE + \
                         SLT_FUNC3 * FUNC3_OFFSET_MULTIPLIER + \
                         SLT_FUNC7 * FUNC7_OFFSET_MULTIPLIER)

// slti
#define SLTI_OPCODE     (0x13) // 0b00010011
#define SLTI_FUNC3      (0x02) // 0b00000010
#define SLTI_ID_BITS    (SLTI_OPCODE + \
                         SLTI_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// sltu
#define SLTU_OPCODE     (0x33) // 0b00110011
#define SLTU_FUNC3      (0x03) // 0b00000011
#define SLTU_FUNC7      (0x00) // 0b00000000
#define SLTU_ID_BITS    (SLTU_OPCODE + \
                         SLTU_FUNC3 * FUNC3_OFFSET_MULTIPLIER + \
                         SLTU_FUNC7 * FUNC7_OFFSET_MULTIPLIER)

// sltiu
#define SLTIU_OPCODE    (0x13) // 0b00010011
#define SLTIU_FUNC3     (0x03) // 0b00000011
#define SLTIU_ID_BITS   (SLTIU_OPCODE + \
                         SLTIU_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// beq
#define BEQ_OPCODE      (0x63) // 0b01100011
#define BEQ_FUNC3       (0x00) // 0b00000000
#define BEQ_ID_BITS     (BEQ_OPCODE + \
                         BEQ_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// bne 
#define BNE_OPCODE      (0x63) // 0b01100011
#define BNE_FUNC3       (0x01) // 0b00000001
#define BNE_ID_BITS     (BNE_OPCODE + \
                         BNE_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// blt
#define BLT_OPCODE      (0x63) // 0b01100011
#define BLT_FUNC3       (0x04) // 0b00000100
#define BLT_ID_BITS     (BLT_OPCODE + \
                         BLT_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// bltu
#define BLTU_OPCODE     (0x63) // 0b01100011
#define BLTU_FUNC3      (0x06) // 0b00000110
#define BLTU_ID_BITS    (BLTU_OPCODE + \
                         BLTU_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// bge
#define BGE_OPCODE      (0x63) // 0b01100011
#define BGE_FUNC3       (0x05) // 0b00000101
#define BGE_ID_BITS     (BGE_OPCODE + \
                         BGE_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// bgeu
#define BGEU_OPCODE     (0x63) // 0b01100011
#define BGEU_FUNC3      (0x07) // 0b00000111
#define BGEU_ID_BITS    (BGEU_OPCODE + \
                         BGEU_FUNC3 * FUNC3_OFFSET_MULTIPLIER)

// jal
#define JAL_OPCODE      (0x6F) // 0b01101111
#define JAL_ID_BITS     (JAL_OPCODE)

// jalr
#define JALR_OPCODE     (0x67) // 0b01100111
#define JALR_FUNC3      (0x00) // 0b00000000
#define JALR_ID_BITS    (JALR_OPCODE + \
                         JALR_FUNC3 * FUNC3_OFFSET_MULTIPLIER)


// ADDITIONAL INSTRUCTION TYPE ANATOMY INFO FOR INSTRUCTION BIT PARSING ...

// Length in bits of each immediate number after joining for each instr type
#define INST_TYPE_I_IMM_ORI_LEN  (12) // Original bit len of imm in inst type I
#define INST_TYPE_S_IMM_ORI_LEN  (12) // Original bit len of imm in inst type S
#define INST_TYPE_SB_IMM_ORI_LEN (13) // Original bit len of imm in inst type S
#define INST_TYPE_UJ_IMM_ORI_LEN (21) // Original bit len of imm in inst type UJ

// Bitmasks to extract imm bit sections from instruction type SB (instr & mask)
#define INST_TYPE_SB_IMM_BIT_11_MASK      (0x00000080)
#define INST_TYPE_SB_IMM_BIT_4_1_MASK     (0x00000F00)
#define INST_TYPE_SB_IMM_BIT_10_5_MASK    (0x7E000000)
#define INST_TYPE_SB_IMM_BIT_12_MASK      (0x80000000)

// The offset of each SB imm bitfield during conversion from [instr -> num]
#define INST_TYPE_SB_IMM_BIT_11_OFFSET    (4)  // Left
#define INST_TYPE_SB_IMM_BIT_4_1_OFFSET   (7)  // Right
#define INST_TYPE_SB_IMM_BIT_10_5_OFFSET  (20) // Right
#define INST_TYPE_SB_IMM_BIT_12_OFFSET    (19) // Right

// Bitmasks to extract imm bit sections from instruction type UJ (instr & mask)
#define INST_TYPE_UJ_IMM_BIT_20_MASK      (0x80000000)
#define INST_TYPE_UJ_IMM_BIT_10_1_MASK    (0x7FE00000)
#define INST_TYPE_UJ_IMM_BIT_11_MASK      (0x00100000)
#define INST_TYPE_UJ_IMM_BIT_19_12_MASK   (0x000FF000)

// The offset of each UJ imm bitfield during conversion from [instr -> num]
#define INST_TYPE_UJ_IMM_BIT_20_OFFSET    (11) // Right
#define INST_TYPE_UJ_IMM_BIT_10_1_OFFSET  (20) // Right
#define INST_TYPE_UJ_IMM_BIT_11_OFFSET    (9)  // Right
#define INST_TYPE_UJ_IMM_BIT_19_12_OFFSET (0)  // Right

// Mask used to extract imm bit from type U instructions
#define INST_TYPE_U_IMM_EXTRACT_MASK      (0xFFFFF000)


// MACROS ...

// Returns whether the given instruction is a match with the given id bits
#define INSTR_MATCH(instr, extract_mask, id_bits) \
            ((instr & extract_mask) == id_bits)


// INSTRUCTION DATA STRUCTS AND IDENTIFIERS ...

// Struct for instruction type R
struct instruction_type_R {
    int32_t opcode;
    int32_t rd;
    int32_t func3;
    int32_t rs1;
    int32_t rs2;
    int32_t func7;
};

// Struct for instruction type I
struct instruction_type_I {
    int32_t opcode;
    int32_t rd;
    int32_t func3;
    int32_t rs1;
    int32_t imm;
};

// Struct for instruction type S
struct instruction_type_S {
    int32_t opcode;
    int32_t func3;
    int32_t rs1;
    int32_t rs2;
    int32_t imm;
};

// Struct for instruction type SB
struct instruction_type_SB {
    int32_t opcode;
    int32_t func3;
    int32_t rs1;
    int32_t rs2;
    int32_t imm;
};

// Struct for instruction type U
struct instruction_type_U {
    int32_t opcode;
    int32_t rd;
    int32_t imm;
};

// Struct for instruction type UJ
struct instruction_type_UJ {
    int32_t opcode;
    int32_t rd;
    int32_t imm;
};

// Joined instruction types for universal use
typedef struct instruction_t instruction_t;
struct instruction_t {
    union {
        struct instruction_type_R type_R;
        struct instruction_type_I type_I;
        struct instruction_type_S type_S;
        struct instruction_type_U type_U;
        struct instruction_type_SB type_SB;
        struct instruction_type_UJ type_UJ;
    };
};


// FUNCTIONS TO EXTRACT BIT FIELDS FROM INSTRUCTIONS ...

/* Returns the opcode bits of the given instruction,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_opcode_bits(const int32_t instr);

/* Returns the bits within the rd field of the function,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_rd_bits(const int32_t instr);

/* Returns the bits within the func3 field of the function,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_func3_bits(const int32_t instr);

/* Returns the bits within the rs1 field of the function,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_rs1_bits(const int32_t instr);

/* Returns the bits within the rs2 field of the function,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_rs2_bits(const int32_t instr);

/* Returns the bits within the func7 field of the function,
   aligned to the least-significant side of the returned uint32
*/
int32_t get_func7_bits(const int32_t instr);


// FUNCTIONS TO UNPACK ENTIRE INSTRUCTIONS INTO ALLOCATED FIELDS ...

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using R type format.
*/
void extract_R(instruction_t* const instr, int32_t raw_instr);

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using I type format.
*/
void extract_I(instruction_t* const instr, int32_t raw_instr);

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using S type format.
*/
void extract_S(instruction_t* const instr, int32_t raw_instr);

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using SB type format.
*/
void extract_SB(instruction_t* const instr, int32_t raw_instr);

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using U type format.
*/
void extract_U(instruction_t* const instr, int32_t raw_instr);

/* Extracts all bit fields from the binary instruction 'raw_instr' and 
   saves them in the instruction struct referenced by the 'instr' pointer.
   Unpacks the 'raw_instr' using UJ type format.
*/
void extract_UJ(instruction_t* const instr, int32_t raw_instr);


// FUNCTIONS TO EXECUTE INDIVIDUAL INSTRUCTION CALLS ...

// ARITHMETIC AND LOGIC OPERATIONS

/* Executes the 'add' instruction
    Uses the given 'instr' data to execute the 'add' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] + R[rs2]
*/
void exec_add(instruction_t* const instr);

/* Executes the 'addi' instruction
    Uses the given 'instr' data to execute the 'addi' instruction.
    Format    | I
    Operation | R[rd] = R[rs1] + imm
*/
void exec_addi(instruction_t* const instr);

/* Executes the 'sub' instruction
    Uses the given 'instr' data to execute the 'sub' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] - R[rs2]
*/
void exec_sub(instruction_t* const instr);

/* Executes the 'lui' instruction
    Uses the given 'instr' data to execute the 'lui' instruction.
    Format    | U
    Operation | R[rd] = {31:12 = imm | 11:0 = 0}
*/
void exec_lui(instruction_t* const instr);

/* Executes the 'xor' instruction
    Uses the given 'instr' data to execute the 'xor' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] ˆ R[rs2]
*/
void exec_xor(instruction_t* const instr);

/* Executes the 'xori' instruction
    Uses the given 'instr' data to execute the 'xori' instruction.
    Format    | I
    Operation | R[rd] = R[rs1] ˆ imm
*/
void exec_xori(instruction_t* const instr);

/* Executes the 'or' instruction
    Uses the given 'instr' data to execute the 'or' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] | R[rs2]
*/
void exec_or(instruction_t* const instr);

/* Executes the 'ori' instruction
    Uses the given 'instr' data to execute the 'ori' instruction.
    Format    | I
    Operation | R[rd] = R[rs1] | imm
*/
void exec_ori(instruction_t* const instr);

/* Executes the 'and' instruction
    Uses the given 'instr' data to execute the 'and' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] & R[rs2]
*/
void exec_and(instruction_t* const instr);

/* Executes the 'andi' instruction
    Uses the given 'instr' data to execute the 'andi' instruction.
    Format    | I
    Operation | R[rd] = R[rs1] & imm
*/
void exec_andi(instruction_t* const instr);

/* Executes the 'sll' instruction
    Uses the given 'instr' data to execute the 'sll' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] « R[rs2]
*/
void exec_sll(instruction_t* const instr);

/* Executes the 'srl' instruction
    Uses the given 'instr' data to execute the 'srl' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] » R[rs2]
*/
void exec_srl(instruction_t* const instr);

/* Executes the 'sra' instruction
    Uses the given 'instr' data to execute the 'sra' instruction.
    Format    | R
    Operation | R[rd] = R[rs1] » R[rs2]
*/
void exec_sra(instruction_t* const instr);

// MEMORY ACCESS OPERATIONS

/* Executes the 'lb' instruction
    Uses the given 'instr' data to execute the 'lb' instruction.
    Format    | I
    Operation | R[rd] = sext(M[R[rs1] + imm])
*/
void exec_lb(instruction_t* const instr);

/* Executes the 'lh' instruction
    Uses the given 'instr' data to execute the 'lh' instruction.
    Format    | I
    Operation | R[rd] = sext(M[R[rs1] + imm])
*/
void exec_lh(instruction_t* const instr);

/* Executes the 'lw' instruction
    Uses the given 'instr' data to execute the 'lw' instruction.
    Format    | I
    Operation | R[rd] = M[R[rs1] + imm]
*/
void exec_lw(instruction_t* const instr);

/* Executes the 'lbu' instruction
    Uses the given 'instr' data to execute the 'lbu' instruction.
    Format    | I
    Operation | R[rd] = M[R[rs1] + imm]
*/
void exec_lbu(instruction_t* const instr);

/* Executes the 'lhu' instruction
    Uses the given 'instr' data to execute the 'lhu' instruction.
    Format    | I
    Operation | R[rd] = M[R[rs1] + imm]
*/
void exec_lhu(instruction_t* const instr);

/* Executes the 'sb' instruction
    Uses the given 'instr' data to execute the 'sb' instruction.
    Format    | S
    Operation | M[R[rs1] + imm] = R[rs2]
*/
void exec_sb(instruction_t* const instr);

/* Executes the 'sh' instruction
    Uses the given 'instr' data to execute the 'sh' instruction.
    Format    | S
    Operation | M[R[rs1] + imm] = R[rs2]
*/
void exec_sh(instruction_t* const instr);

/* Executes the 'sw' instruction
    Uses the given 'instr' data to execute the 'sw' instruction.
    Format    | S
    Operation | M[R[rs1] + imm] = R[rs2]
*/
void exec_sw(instruction_t* const instr);

// PROGRAM FLOW OPERATIONS

/* Executes the 'slt' instruction
    Uses the given 'instr' data to execute the 'slt' instruction.
    Format    | R
    Operation | R[rd] = (R[rs1] < R[rs2]) ? 1 : 0
*/
void exec_slt(instruction_t* const instr);

/* Executes the 'slti' instruction
    Uses the given 'instr' data to execute the 'slti' instruction.
    Format    | I
    Operation | R[rd] = (R[rs1] < imm) ? 1 : 0
*/
void exec_slti(instruction_t* const instr);

/* Executes the 'sltu' instruction
    Uses the given 'instr' data to execute the 'sltu' instruction.
    Format    | R
    Operation | R[rd] = (R[rs1] < R[rs2]) ? 1 : 0
*/
void exec_sltu(instruction_t* const instr);

/* Executes the 'sltiu' instruction
    Uses the given 'instr' data to execute the 'sltiu' instruction.
    Format    | I
    Operation | R[rd] = (R[rs1] < imm) ? 1 : 0
*/
void exec_sltiu(instruction_t* const instr);

/* Executes the 'beq' instruction
    Uses the given 'instr' data to execute the 'beq' instruction.
    Format    | SB
    Operation | if(R[rs1] == R[rs2]) then PC = PC + (imm « 1)
*/
void exec_beq(instruction_t* const instr);

/* Executes the 'bne' instruction
    Uses the given 'instr' data to execute the 'bne' instruction.
    Format    | SB
    Operation | if(R[rs1] != R[rs2]) then PC = PC + (imm « 1)
*/
void exec_bne(instruction_t* const instr);

/* Executes the 'blt' instruction
    Uses the given 'instr' data to execute the 'blt' instruction.
    Format    | SB
    Operation | if(R[rs1] < R[rs2]) then PC = PC + (imm « 1)
*/
void exec_blt(instruction_t* const instr);

/* Executes the 'bltu' instruction
    Uses the given 'instr' data to execute the 'bltu' instruction.
    Format    | SB
    Operation | if(R[rs1] < R[rs2]) then PC = PC + (imm « 1)
*/
void exec_bltu(instruction_t* const instr);

/* Executes the 'bge' instruction
    Uses the given 'instr' data to execute the 'bge' instruction.
    Format    | SB
    Operation | if(R[rs1] >= R[rs2]) then PC = PC + (imm « 1)
*/
void exec_bge(instruction_t* const instr);

/* Executes the 'bgeu' instruction
    Uses the given 'instr' data to execute the 'bgeu' instruction.
    Format    | SB
    Operation | if(R[rs1] >= R[rs2]) then PC = PC + (imm « 1)
*/
void exec_bgeu(instruction_t* const instr);

/* Executes the 'jal' instruction
    Uses the given 'instr' data to execute the 'jal' instruction.
    Format    | UJ
    Operation | R[rd] = PC + 4; PC = PC + (imm « 1)
*/
void exec_jal(instruction_t* const instr);

/* Executes the 'jalr' instruction
    Uses the given 'instr' data to execute the 'jalr' instruction.
    Format    | I
    Operation | R[rd] = PC + 4; PC = R[rs1] + imm
*/
void exec_jalr(instruction_t* const instr);


// INSTRUCTION PARSER AND EXECUTOR ...

/* Determines which instruction was given and executes accordingly

    Prints appropriate error message on error

    RETURNS 
    0  | On success
    -1 | On error

*/
extern void exec_instruction(const int32_t instr);


// END HEADER GUARD ...
#endif
