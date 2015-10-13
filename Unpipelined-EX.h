//-*-c++-*-
#ifndef _Unpiplined_EX_h_
#define _Unpiplined_EX_h_

#include "systemc.h"
#include "Unpipelined.h"
#include "DLXMemory.h"
#include "DLX.h"
#include "DLXMachineState.h"

struct Unpiplined_EX : public sc_module {

  //
  // Define ports
  //
  sc_in<StateID2EX> from_decode;
  sc_out<StateID2EX> to_mem;
  sc_out<bool> mem_ready;

  sc_in<bool> reset;
  sc_in_clk clock;
  sc_inout<bool> ready;

  bool debug;

  SC_CTOR(Unpiplined_EX) {
    debug = false;
    SC_CTHREAD(execute, clock.pos());
    watching(reset.delayed() == true);
  }

  void execute();
};

#endif
