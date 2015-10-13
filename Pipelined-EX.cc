//-*-c++-*-

#include "Pipelined-EX.h"

void Pipelined_EX::execute()
{
  for(;;) {

    wait_until(ready.delayed() == true);
    ready = false;

    //
    // Decode the instruction
    //
    StateID2EX insn = from_decode;

    DLX::execute(insn.opcode,
		 insn.A,
		 insn.B,
		 insn.imm,
		 insn.pc,
		 insn.npc,
		 insn.alu_output,
		 insn.r31_output);

    if (debug) {
      char buffer[512];
      fprintf(stderr, "%s:%d (%s) execute @%08x as %s\n",
	      __FILE__, __LINE__, __FUNCTION__,
	      insn.pc,
	      DLX::print(buffer, insn.opcode, insn.format));
      fprintf(stderr,"\t\trd=%d, imm=%08x, A=%08x, B=%08x\n",
	      insn.rd, insn.imm, insn.A, insn.B);
      fprintf(stderr,"\t\talu_output=%08x, r31_output=%08x\n",
	      insn.alu_output, insn.r31_output);
    }

    to_mem = insn;
    mem_ready = true;
  }

}
