//-*-c++-*-
#ifndef _Pipelined_ID_h_
#define _Pipelined_ID_h_

#include "systemc.h"
#include "DLX.h"
#include "DLXMemory.h"
#include "DLXMachineState.h"
#include "Pipelined.h"

struct Pipelined_ID : public sc_module {

  //
  // Define ports
  //
  sc_in<StateIF2ID> from_fetch;
  sc_out<StateID2EX> to_exec;
  sc_out<bool> exec_ready;

  sc_in<bool> reset;
  sc_in_clk clock;
  sc_inout<bool> ready;

  bool debug;

  SC_CTOR(Pipelined_ID) {
    //
    // User needs to set up memory/machine state links by hand
    //
    debug = false;

    SC_CTHREAD(decode, clock.pos());
    watching(reset.delayed() == true);
  }

  void decode();

};

#endif
