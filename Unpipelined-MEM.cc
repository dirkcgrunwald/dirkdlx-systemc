#include "Unpipelined-MEM.h"

void
Unpiplined_MEM::mem()
{
  for(;;) {

    wait_until(ready.delayed() == true);
    ready = false;

    StateID2EX insn = from_exec;
    //
    // Perform any necessary memory operations
    //
    bool done = true;
    switch(insn.opcode) {

      /////////////////////////////////////////////////////////////////////////////
      // Load operations
      /////////////////////////////////////////////////////////////////////////////

    case OP_LB:
      insn.lmb = memory.read_byte( insn.alu_output);
      if ( insn.lmb & 0x80 ) {
	insn.lmb |= 0xffffff00;
      }
      break;
      
    case OP_LBU:
      insn.lmb = memory.read_byte( insn.alu_output);
      insn.lmb = insn.lmb & 0xff;
      break;

    case OP_LH:
      insn.lmb = memory.read_half( insn.alu_output);
      if ( insn.lmb & 0x8000 ) {
	insn.lmb |= 0xffff0000;
      }
      break;
      
    case OP_LHU:
      insn.lmb = memory.read_half( insn.alu_output);
      insn.lmb = insn.lmb & 0xffff;
      break;

    case OP_LW:
      insn.lmb = memory.read_word( insn.alu_output);
      break;

    case OP_LF:
    case OP_LD:
      //
      // These should have been caught in execute
      //
      abort();
      break;

      /////////////////////////////////////////////////////////////////////////////
      // Store operations
      /////////////////////////////////////////////////////////////////////////////

    case OP_SB:
      memory.write_byte( insn.alu_output, insn.B);
      break;
    case OP_SH:
      memory.write_half( insn.alu_output, insn.B);
      break;
    case OP_SW:
      memory.write_word( insn.alu_output, insn.B);
      break;

    default:
      done = false;
      done = true;
      break;
    }

    if ( debug && done ) {
      char buffer[512];
      fprintf(stderr, "%s:%d (%s) execute %08x as %s\n",
	      __FILE__, __LINE__, __FUNCTION__,
	      insn.pc,
	      DLX::print(buffer, insn.opcode, insn.format));
      fprintf(stderr,"\t\talu_output=%08x, B=%08x, lmb=%08x\n",
	      insn.alu_output, insn.B, insn.lmb);
    }

    to_wb = insn;
    wb_ready = true;
  }
}	

