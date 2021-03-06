//-*-c++-*-
#ifndef _Pipelined_MEM_h_
#define _Pipelined_MEM_h_

#include "systemc.h"
#include "DLX.h"
#include "DLXMemory.h"
#include "DLXMachineState.h"
#include "Pipelined.h"

struct Pipelined_MEM : public sc_module {

  //
  // Define ports
  //
  sc_in<StateID2EX> from_exec;
  sc_out<StateID2EX> to_wb;
  sc_out<bool> wb_ready;

  sc_in<bool> reset;
  sc_in_clk clock;
  sc_inout<bool> ready;

  bool debug;

  SC_CTOR(Pipelined_MEM) {
    //
    // User needs to set up memory/machine state links by hand
    //
    debug = false;

    SC_CTHREAD(mem, clock.pos());
    watching(reset.delayed() == true);
  }

  void mem();
};

#endif
