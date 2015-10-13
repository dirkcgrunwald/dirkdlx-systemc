#include "Pipelined-WB.h"

void Pipelined_WB::wb()
{
  for(;;) {
    wait_until(ready.delayed() == true);
    ready = false;

    StateID2EX insn = from_mem;

    //
    // Set up default fetch path
    //
    State2IF fetchState;
    if ( insn.in_shadow ) {
      //
      // If this instruction is in the shadow of a delayed
      // branch, the next PC to fetch is the delayed PC.
      //
      fetchState.pc = insn.delayed_pc;
    } else {
      fetchState.pc = insn.pc + 4;
    }
    fetchState.in_shadow = false;
    fetchState.delayed_pc = 0;

    switch(insn.format) {

    case RFMT: {
      if (debug) {
	char buffer[512];
	fprintf(stderr, "%s:%d (%s) execute @%08x as %s\n",
		__FILE__, __LINE__, __FUNCTION__,
		insn.pc,
		DLX::print(buffer, insn.opcode, insn.format));
	fprintf(stderr,"\t\twrite %08x to R%d\n",
		insn.alu_output, insn.rd);
      }
      machineState.setArchReg(insn.rd, insn.alu_output);

      to_fetch = fetchState;
      fetch_ready = true;

    }
    break;
    
    case IFMT: {
      switch(insn.opcode) {
      case OP_LB:
      case OP_LBU:
      case OP_LH:
      case OP_LHU:
      case OP_LW:
      case OP_LF:
      case OP_LD:
	//
	// Write the memory value into the 'rd' register
	//
	if (debug) {
	  char buffer[512];
	  fprintf(stderr, "%s:%d (%s) writeback %08x as %s\n",
		  __FILE__, __LINE__, __FUNCTION__,
		  insn.insn,
		  DLX::print(buffer, insn.opcode, insn.format));
	  fprintf(stderr,"\t\twrite %08x to R%d\n",
		  insn.lmb, insn.rd);
	}
	machineState.setArchReg(insn.rd, insn.lmb);

	to_fetch = fetchState;
	fetch_ready = true;
	break;

	/////////////////////////////////////////////////////////////////////////////
	// Store -- nothing happens
	/////////////////////////////////////////////////////////////////////////////
      case OP_SB:
      case OP_SH:
      case OP_SW:
	to_fetch = fetchState;
	fetch_ready = true;
	break;

      case OP_BEQZ:
      case OP_BNEZ:
      case OP_BFPT:
      case OP_BFPF:
	//
	// Conditional branch - outcome already computed, destination
	// address is in alu_output (either pc+4 or destination)
	//
	if (debug) {
	  char buffer[512];
	  fprintf(stderr, "%s:%d (%s) conditional branch at %08x as %s\n",
		  __FILE__, __LINE__, __FUNCTION__,
		  insn.insn,
		  DLX::print(buffer, insn.opcode, insn.format));
	  fprintf(stderr,"\t\tbranch to %08x\n",
		  insn.alu_output);
	}

	fetchState.pc = insn.pc + 4;
	fetchState.in_shadow = true;
	fetchState.delayed_pc = insn.alu_output;
	to_fetch = fetchState;
	fetch_ready = true;

	break;

      default:
	if (debug) {
	  char buffer[512];
	  fprintf(stderr, "%s:%d (%s) execute %08x as %s\n",
		  __FILE__, __LINE__, __FUNCTION__,
		  insn.insn,
		  DLX::print(buffer, insn.opcode, insn.format));
	  fprintf(stderr,"\t\twrite %08x to R%d\n",
		  insn.alu_output, insn.rd);
	}
	machineState.setArchReg(insn.rd, insn.alu_output);

	to_fetch = fetchState;
	fetch_ready = true;
      }
    }
    break;

    case JFMT: {
      if (debug) {
	cerr << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " "
	     << " jump " << insn.alu_output << " to R" << insn.rd << endl;
      }
    }
    break;
    } // switch
      
    //
    // Already set R0 to insure that it remains as zero
    //
    machineState.setArchReg(0, 0);
    
    fprintf(stderr,"\n-----------------------------------------------------------------------------\n");

  }
}
