#include "DLX.h"
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <ctype.h>

/*
 * The table below is used to translate bits 31:26 of the instruction
 * into a value suitable for the "opCode" field of a MemWord structure,
 * or into a special value for further decoding.
 */

DLX::OpInfo
DLX::opTable[] =
{
    {OP_SPECIAL, RFMT}, {OP_FPARITH, RFMT}, {OP_J, JFMT}, {OP_JAL, JFMT},
    {OP_BEQZ, IFMT}, {OP_BNEZ, IFMT}, {OP_BFPT, IFMT}, {OP_BFPF, IFMT},
    {OP_ADDI, IFMT}, {OP_ADDUI, IFMT}, {OP_SUBI, IFMT}, {OP_SUBUI, IFMT},
    {OP_ANDI, IFMT}, {OP_ORI, IFMT}, {OP_XORI, IFMT}, {OP_LHI, IFMT},
    {OP_RFE, IFMT}, {OP_TRAP, IFMT}, {OP_JR, IFMT}, {OP_JALR, IFMT},
    {OP_SLLI, IFMT}, {OP_RES, IFMT}, {OP_SRLI, IFMT}, {OP_SRAI, IFMT},
    {OP_SEQI, IFMT}, {OP_SNEI, IFMT}, {OP_SLTI, IFMT}, {OP_SGTI, IFMT},
    {OP_SLEI, IFMT}, {OP_SGEI, IFMT}, {OP_RES, IFMT}, {OP_RES, IFMT},
    {OP_LB, IFMT}, {OP_LH, IFMT}, {OP_RES, IFMT}, {OP_LW, IFMT},
    {OP_LBU, IFMT}, {OP_LHU, IFMT}, {OP_LF, IFMT}, {OP_LD, IFMT},
    {OP_SB, IFMT}, {OP_SH, IFMT}, {OP_RES, IFMT}, {OP_SW, IFMT},
    {OP_RES, IFMT}, {OP_RES, IFMT}, {OP_SF, IFMT}, {OP_SD, IFMT},
    {OP_SEQUI, IFMT}, {OP_SNEUI, IFMT}, {OP_SLTUI, IFMT}, {OP_SGTUI, IFMT},
    {OP_SLEUI, IFMT}, {OP_SGEUI, IFMT}, {OP_RES, IFMT}, {OP_RES, IFMT},
    {OP_RES, IFMT}, {OP_RES, IFMT}, {OP_RES, IFMT}, {OP_RES, IFMT},
    {OP_RES, IFMT}, {OP_RES, IFMT}, {OP_RES, IFMT}, {OP_RES, IFMT}
};

/*
 * the table below is used to convert the "funct" field of SPECIAL
 * instructions into the "opCode" field of a MemWord.
 */

OpcodeEnum
DLX::specialTable[] = {
    OP_NOP, OP_RES, OP_RES, OP_RES, OP_SLL, OP_RES, OP_SRL, OP_SRA,
    OP_RES, OP_RES, OP_RES, OP_RES, OP_TRAP, OP_RES, OP_RES, OP_RES,
    OP_SEQU, OP_SNEU, OP_SLTU, OP_SGTU, OP_SLEU, OP_SGEU, OP_RES, OP_RES,
    OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES,
    OP_ADD, OP_ADDU, OP_SUB, OP_SUBU, OP_AND, OP_OR, OP_XOR, OP_RES,
    OP_SEQ, OP_SNE, OP_SLT, OP_SGT, OP_SLE, OP_SGE, OP_RES, OP_RES,
    OP_MOVI2S, OP_MOVS2I, OP_MOVF, OP_MOVD, OP_MOVFP2I, OP_MOVI2FP,
    OP_RES, OP_RES,
    OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES
};

/*
 * the table below is used to convert the "funct" field of FPARITH
 * instructions into the "opCode" field of a MemWord.
 */
static OpcodeEnum
FParithTable[] = {
  OP_ADDF, OP_SUBF, OP_MULTF, OP_DIVF, OP_ADDD, OP_SUBD, OP_MULTD, OP_DIVD,
  OP_CVTF2D, OP_CVTF2I, OP_CVTD2F, OP_CVTD2I, OP_CVTI2F, OP_CVTI2D, 
  OP_MULT, OP_DIV,
  OP_EQF, OP_NEF, OP_LTF, OP_GTF, OP_LEF, OP_GEF, OP_MULTU, OP_DIVU,
  OP_EQD, OP_NED, OP_LTD, OP_GTD, OP_LED, OP_GED, OP_RES, OP_RES,
  OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES,
  OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES,
  OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES,
  OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES, OP_RES
  };
  
char *
DLX::operationNames[] = {
    "", "ADD", "ADDI", "ADDU", "ADDUI", "AND", "ANDI", "BEQZ", "BFPF",
    "BFPT", "BNEZ", "J", "JAL", "JALR", "JR", "LB", "LBU", "LD", "LF",
    "LH", "LHI", "LHU", "LW", "MOVD", "MOVF", "MOVFP2I", "MOVI2FP",
    "MOVI2S", "MOVS2I", "OR", "ORI", "RFE", "SB", "SD", "SEQ",  "SEQI",
    "SEQU", "SEQUI",
    "SF", "SGE", "SGEI", "SGEU", "SGEUI", "SGT", "SGTI", "SGTU", "SGTUI",
    "SH", "SLE", "SLEI", "SLEU", "SLEUI", "SLL",
    "SLLI", "SLT", "SLTI", "SLTU", "SLTUI", "SNE", "SNEI", "SNEU", "SNEUI",
    "SRA", "SRAI", "SRL",
    "SRLI", "SUB", "SUBI", "SUBU", "SUBUI", "SW", "TRAP", "XOR", "XORI",
    "NOP", "", "", "","", "",

    /* There must be at least one empty string between the last integer operation
     *	and the first floating point operation.	*/

    "ADDD", "ADDF", "CVTD2F", "CVTD2I", "CVTF2D", "CVTF2I", "CVTI2D",
    "CVTI2F", "DIV", "DIVD", "DIVF", "DIVU", "EQD", "EQF", "GED", "GEF",
    "GTD", "GTF", "LED", "LEF", "LTD", "LTF", "MULT", "MULTD", "MULTF",
    "MULTU", "NED", "NEF", "SUBD", "SUBF", ""
    };


void
DLX::unimplemented(int opcode)
{
  fprintf(stderr,"Opcode %d is unimplemented\n", opcode);
  abort();
}

void
DLX::decode(uint32_t insn,
	    int& format,
	    OpcodeEnum& opcode,
	    int& rs1,
	    int& rs2,
	    int& rd,
	    int& imm)
{
  rs1 = (insn >> 21) & 0x1f;
  rs2 = (insn >> 16) & 0x1f;
  rd = (insn >> 11) & 0x1f;
  int op = (insn >> 26) & 0x3f;
  opcode = opTable[op].opCode;
  format = opTable[op].format;

  switch (format) {
    //
    // Sign extended immediate value
    //
  case IFMT:
    rd = rs2;
    imm = insn & 0xffff;
    if ( imm & 0x8000 ) {
      imm |= 0xffff0000;
    }
    break;
    
  case RFMT:
    //
    // Pull out function
    //
    imm = (insn >> 6) & 0x1f;
    break;

  case JFMT:
    imm = insn & 0x3ffffff;
    if (imm& 0x2000000) {
      imm |= 0xfc000000;
    }
    break;

  default:
    abort();
  }

  if (opcode == OP_SPECIAL) {
    opcode = specialTable[insn & 0x3f];
  }
  else if (opcode == OP_FPARITH) {
    opcode = FParithTable[insn & 0x3f];
  }
}


void
DLX::execute(OpcodeEnum opcode,
	     int A,
	     int B,
	     int imm,
	     int PC,
	     int NPC,
	     int& alu_output,
	     int& r31_output
	     )
{
  switch(opcode) {
  case OP_ADD:
    //
    // No check for overflow
    //
    alu_output = A + B; break;

  case OP_ADDI:
    alu_output = A + imm; break;

  case OP_ADDU:
    alu_output = (uint32_t) A + ((uint32_t) B & 0xffff);
    break;

  case OP_ADDUI:
    alu_output = (uint32_t) A + ((uint32_t) imm & 0xffff);
    break;

  case OP_AND:
    alu_output = A & B; break;
  case OP_ANDI:
    alu_output = A & imm; break;

  case OP_BEQZ:
    if ( A == 0 ) {
      alu_output = NPC + imm;
    } else {
      alu_output = NPC;
    }
    break;

  case OP_BFPF:
  case OP_BFPT:
    unimplemented(opcode);
    break;
    
  case OP_BNEZ:
    if ( A != 0 ) {
      alu_output = NPC + imm;
    } else {
      alu_output = NPC;
    }
    break;

  case OP_JAL:
    r31_output = NPC + 4;
    alu_output = PC + 4 + imm;
    break;

  case OP_J:
    alu_output = PC + 4 + imm;
    break;

  case OP_JALR:
    r31_output = NPC + 4;
    alu_output = PC + 4 + A;
    break;

  case OP_JR:
    alu_output = PC + 4 + A;
    break;

    //
    // Load operations only compute the effective address
    //
  case OP_LB:
  case OP_LBU:
  case OP_LD:
  case OP_LF:
  case OP_LH:
  case OP_LHU:
  case OP_LW:
    alu_output = A + imm;
    break;

  case OP_LHI:
    alu_output = imm << 16; 
    break;

  case OP_MOVD:
  case OP_MOVF:
  case OP_MOVFP2I:
  case OP_MOVI2FP:
  case OP_MOVI2S:
  case OP_MOVS2I:
    unimplemented(opcode);
    break;
    

  case OP_OR:
    alu_output = A | B; break;
  case OP_ORI:
    alu_output = A | imm; break;
    
  case OP_RFE:
    unimplemented(opcode);
    break;

    //
    // Store, like load, ony computes the effective address
    //
  case OP_SB:
  case OP_SD:
    alu_output = A + imm;
    break;
    
  case OP_SEQ:
    if (A == B ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SEQI:
    if (A == imm ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SEQU:
    if ((uint32_t) A == ((uint32_t) B & 0xffff)) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }

  case OP_SEQUI:
    if ((uint32_t) A == ((uint32_t) imm & 0xffff)) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }

    // More store
  case OP_SF:
    alu_output = A + imm;
    break;

  case OP_SGE:
    if ( A >= B ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SGEI:
    if ( A >= imm ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SGEU:
    if ((uint32_t) A >= ((uint32_t) B & 0xffff) ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SGEUI:
    if ((uint32_t) A >= ((uint32_t) imm & 0xffff) ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SGT:
    if ( A > B ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SGTI:
    if ( A > imm ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SGTU:
    if ((uint32_t) A > ((uint32_t) B & 0xffff) ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SGTUI:
    if ((uint32_t) A > ((uint32_t) imm & 0xffff) ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

    //
    // Store
    //
  case OP_SH:
     alu_output = A + imm;
    break;

  case OP_SLE:
    if ( A <= B ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SLEI:
    if ( A <= imm ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SLEU:
    if ((uint32_t) A <= ((uint32_t) B & 0xffff)) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SLEUI:
    if ((uint32_t) A <= ((uint32_t) imm & 0xffff) ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SLL:
    alu_output = A << (B & 0x1f);
    break;

  case OP_SLLI:
    alu_output = A << (imm & 0x1f);
    break;

  case OP_SLT:
    if ( A < B ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SLTI:
    if ( A < imm ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SLTU:
    if ((uint32_t) A < ((uint32_t) B & 0xffff) ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SLTUI:
    if ((uint32_t) A < ((uint32_t) imm & 0xffff) ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SNE:
    if ( A != B ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SNEI:
    if ( A != imm ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SNEU:
    if ((uint32_t) A != ((uint32_t) B & 0xffff) ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SNEUI:
    if ((uint32_t) A != ((uint32_t) imm & 0xffff) ) {
      alu_output = 1;
    } else {
      alu_output = 0;
    }
    break;

  case OP_SRA:
    alu_output = A >> (B & 0x1f);
    break;

  case OP_SRAI:
    alu_output = A >> (imm & 0x1f);
    break;

  case OP_SRL:
    {
      uint32_t tmp = A;
      tmp >>= (B & 0x1f);
      alu_output = tmp;
    }
    break;

  case OP_SRLI:
    {
      uint32_t tmp = A;
      tmp >>= (imm & 0x1f);
      alu_output = tmp;
    }
    break;


    //
    // Again, no over/underflow
    //
  case OP_SUB:
    alu_output = A = B;
    break;
  case OP_SUBI:
    alu_output = A - imm;
    break;
    
  case OP_SUBU:
    alu_output = A - ((uint32_t) B & 0xffff);
    break;
  case OP_SUBUI:
    alu_output = A - ((uint32_t) imm & 0xffff);
    break;
    
    //
    // Store word
    //
  case OP_SW:
    alu_output = A + imm;
    break;

  case OP_TRAP:
    unimplemented(opcode);
    break;
   
  case OP_XOR:
    alu_output = A ^ B;
    break;

  case OP_XORI:
    alu_output = A ^ imm;
    break;

  case OP_NOP:
    break;

    //
    // Unimplemented floating point operations
    //
  default:
    unimplemented(opcode);
    break;
  }
}

char *
DLX::print(char *buffer, int opcode, int format)
{
  int names = sizeof(operationNames) / sizeof(char*);
  char *fmt;
  switch (format) {
  case IFMT: fmt = "IFMT"; break;
  case JFMT: fmt = "JFMT"; break;
  case RFMT: fmt = "RFMT"; break;
  default:
    fmt = "<UNKNOWN FMT>"; break;
  }

  if (opcode >= 0 && opcode <= names ) {
    sprintf(buffer, "OP=%s, FMT=%s",
	    operationNames[opcode],
	    fmt);
  } else {
    sprintf(buffer, "OP=%s, FMT=%s",
	    "UNKNOWN", fmt);
  }
  return buffer;
}
