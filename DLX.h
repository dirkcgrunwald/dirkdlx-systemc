//-*-c++-*-
#ifndef _DLX_hh_
#define _DLX_hh_

#include <stdint.h>

/*
 * The structure below describes the state of an DLX machine.
 */

#define TOTAL_REGS 66
#define NUM_GPRS 64
#define PC_REG (NUM_GPRS)
#define NEXT_PC_REG (NUM_GPRS+1)

/* default values for number of fp units and latencies */
#define MAX_FP_UNITS  20        /* Maximum allowable number of any particular
				   type of floating point unit. */
#define NUM_ADD_UNITS 1         /* Number of fp adder units. */
#define NUM_DIV_UNITS 1         /* Number of fp divider units. */
#define NUM_MUL_UNITS 1         /* Number of fp multiplier units. */
#define FP_ADD_LATENCY 2        /* Latency for an FP add. */
#define FP_DIV_LATENCY 19       /* Latency for an FP divide. */
#define FP_MUL_LATENCY 5        /* Latency for an FP multiply. */

#define FP_ADD 0
#define FP_DIV 1
#define FP_MUL 2
#define FP_SUB 3
#define INT_MUL 4
#define INT_UMUL 5
#define INT_DIV 6
#define INT_UDIV 7

#define FP_INT_OP 0
#define FP_SINGLE_FP_OP 1
#define FP_DOUBLE_FP_OP 2

#define CYC_CNT_RESET 16384     /* How often to reset the cycle count to
				   prevent wrap around problems. */
#define FD_SIZE 32              /* Number of simulated file descriptors */


/*
 * OpCode values for MemWord structs.  These are straight from the MIPS
 * manual except for the following special values:
 *
 * OP_NOT_COMPILED -	means the value of the memory location has changed
 *			so the instruction needs to be recompiled.
 * OP_UNIMP -		means that this instruction is legal, but hasn't
 *			been implemented in the simulator yet.
 * OP_RES -		means that this is a reserved opcode (it isn't
 *			supported by the architecture).
 */
  typedef enum {
    OP_ADD=1, OP_ADDI=2, OP_ADDU=3, OP_ADDUI=4, OP_AND=5, OP_ANDI=6,
    OP_BEQZ=7, OP_BFPF=8, OP_BFPT=9, OP_BNEZ=10, OP_J=11, OP_JAL=12,
    OP_JALR=13, OP_JR=14, OP_LB=15, OP_LBU=16, OP_LD=17, OP_LF=18,
    OP_LH=19, OP_LHI=20, OP_LHU=21, OP_LW=22, OP_MOVD=23, OP_MOVF=24,
    OP_MOVFP2I=25, OP_MOVI2FP=26, OP_MOVI2S=27, OP_MOVS2I=28, OP_OR=29,
    OP_ORI=30, OP_RFE=31, OP_SB=32, OP_SD=33, OP_SEQ=34, OP_SEQI=35,
    OP_SEQU=36, OP_SEQUI=37, OP_SF=38, OP_SGE=39, OP_SGEI=40, OP_SGEU=41,
    OP_SGEUI=42, OP_SGT=43, OP_SGTI=44, OP_SGTU=45, OP_SGTUI=46, OP_SH=47,
    OP_SLE=48, OP_SLEI=49, OP_SLEU=50, OP_SLEUI=51, OP_SLL=52, OP_SLLI=53,
    OP_SLT=54, OP_SLTI=55, OP_SLTU=56, OP_SLTUI=57, OP_SNE=58, OP_SNEI=59,
    OP_SNEU=60, OP_SNEUI=61, OP_SRA=62, OP_SRAI=63, OP_SRL=64, OP_SRLI=65,
    OP_SUB=66, OP_SUBI=67, OP_SUBU=68, OP_SUBUI=69, OP_SW=70, OP_TRAP=71,
    OP_XOR=72, OP_XORI=73, OP_NOP=74, OP_ADDD=80, OP_ADDF=81,
    OP_CVTD2F=82, OP_CVTD2I=83, OP_CVTF2D=84, OP_CVTF2I=85, OP_CVTI2D=86,
    OP_CVTI2F=87, OP_DIV=88, OP_DIVD=89, OP_DIVF=90, OP_DIVU=91,
    OP_EQD=92, OP_EQF=93, OP_GED=94, OP_GEF=95, OP_GTD=96, OP_GTF=97,
    OP_LED=98, OP_LEF=99, OP_LTD=100, OP_LTF=101, OP_MULT=102,
    OP_MULTD=103, OP_MULTF=104, OP_MULTU=105, OP_NED=106, OP_NEF=107,
    OP_SUBD=108, OP_SUBF=109,

    /* special "opcodes", give these values after the above op values */

    OP_NOT_COMPILED=111, OP_UNIMP=112, OP_RES=113, OP_LAST=114,
    OP_SPECIAL=120, OP_FPARITH=121
  } OpcodeEnum;

typedef enum { IFMT=1, JFMT=2, RFMT=3 } InsnFormatEnum;

class DLX {
  typedef struct {
    OpcodeEnum opCode;		/* Translated op code. */
    InsnFormatEnum format;	/* Format type (IFMT or JFMT or RFMT) */
  } OpInfo;

  static OpInfo opTable[];
  static char *operationNames[];
  static OpcodeEnum specialTable[];

public:
  //
  // Initialize the machine state
  //
  DLX() {
    //
    // Empty
    //
  }
    

  //
  // What to do when something is unimplemented
  //
  static void unimplemented(int opcode);

  //
  // Decode an instruction, extracting the format,
  // a "decoded opcode" and the register names or
  // immediate/branch values (depending on format)
  //

  static void decode(uint32_t insn,
		     int& format,
		     OpcodeEnum& opcode,
		     int& rs1,
		     int& rs2,
		     int& rd,
		     int& imm);

  //
  // Function to perform a specifc opcode.  This uses the values
  // contained in the registers specified by the instruction, not the
  // register names. This will call setArchReg/getArchReg to get and
  // set the register values. This does not perform memory actions -
  // those must be done by the pipeline model after the effective
  // address is calculated. All branch operations set the value of
  // "ComputedPC" rather than directly changing the PC.
  //

  static void execute(OpcodeEnum opcode,
		      int A,
		      int B,
		      int imm,
		      int PC,
		      int NPC,
		      int& alu_output,
		      int& r31_output);

  //
  // User must supply buffer for printing
  //
  static char *print(char *, int opcode, int format);

};

#endif
