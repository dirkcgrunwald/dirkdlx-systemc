//-*-c++-*-

#include "Pipelined-ID.h"

void Pipelined_ID::decode()
{
  for(;;) {
    wait_until(ready.delayed() == true);
    ready = false;

    StateIF2ID fetched = from_fetch;
    //
    // Decode the instruction
    //
    StateID2EX exState;
    //
    // Copy the standard fields
    //
    exState.pc = fetched.pc;
    exState.insn = fetched.insn;
    exState.in_shadow = fetched.in_shadow;
    exState.delayed_pc = fetched.delayed_pc;
    exState.alu_output = 0;
    exState.r31_output = 0;

    int rs1;
    int rs2;
    DLX::decode(fetched.insn,
		exState.format,
		exState.opcode,
		rs1,
		rs2,
		exState.rd,
		exState.imm);
    //
    // Access the register values
    //
    exState.A = machineState.getArchReg(rs1);
    exState.B = machineState.getArchReg(rs2);
    //
    // Pass along PC, compute NPC
    //
    exState.pc = fetched.pc;
    exState.npc = fetched.pc + 4;

    if (debug) {
      char buffer[512];
      fprintf(stderr, "%s:%d (%s) decode %08x as %s\n",
	      __FILE__, __LINE__, __FUNCTION__,
	      fetched.insn,
	      DLX::print(buffer, exState.opcode, exState.format));
      fprintf(stderr,"\t\trs1=%d, rs2=%d, rd=%d, imm=%08x, A=%08x, B=%08x\n",
	      rs1, rs2, exState.rd, exState.imm, exState.A, exState.B);
    }

    to_exec = exState;
    exec_ready = true;
  }
}
