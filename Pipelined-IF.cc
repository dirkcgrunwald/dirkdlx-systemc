//-*-c++-*-
#include "Pipelined-IF.h"

void Pipelined_IF::fetch()
{
  for(;;) {

    wait_until(ready.delayed() == true);
    ready = false;

    State2IF request = from_wb;

    StateIF2ID state;
    state.pc = request.pc;
    state.insn = memory.read_word(state.pc);
    state.in_shadow = request.in_shadow;
    state.delayed_pc = request.delayed_pc;

    if (debug) {
      fprintf(stderr, "%s:%d (%s) fetch %08x from %08x\n",
	      __FILE__, __LINE__, __FUNCTION__,
	      state.insn, state.pc);
      fprintf(stderr,"\t\tin_shadow=%d, delayed_pc=%08x\n",
	      state.in_shadow, state.delayed_pc);
    }

    to_decode = state;
    decode_ready = true;

  }
}

