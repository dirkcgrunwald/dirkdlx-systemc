//-*-c++-*-
#ifndef _Pipelined_WB_h_
#define _Pipelined_WB_h_

#include "systemc.h"
#include "Pipelined.h"
#include "DLXMemory.h"
#include "DLX.h"
#include "DLXMachineState.h"

struct Pipelined_WB : public sc_module {

  //
  // Define ports
  //
  sc_in<StateID2EX> from_mem;
  sc_out<State2IF> to_fetch;
  sc_out<bool> fetch_ready;

  sc_in<bool> reset;
  sc_in_clk clock;
  sc_inout<bool> ready;

  bool debug;

  SC_CTOR(Pipelined_WB) {
    //
    // User needs to set up memory/machine state links by hand
    //
    debug = false;

    SC_CTHREAD(wb, clock.pos());
    watching(reset.delayed() == true);
  }

  void wb();
};

#endif
