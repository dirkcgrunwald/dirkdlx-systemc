//-*-c++-*-
#ifndef _Unpiplined_IF_h_
#define _Unpiplined_IF_h_

#include "systemc.h"
#include "Unpipelined.h"
#include "DLXMemory.h"
#include "DLXMachineState.h"

struct Unpiplined_IF : public sc_module {

  //
  // Define ports
  //
  sc_out<StateIF2ID> to_decode;
  sc_out<bool> decode_ready;

  sc_in<State2IF> from_wb;
  sc_in<bool> reset;
  sc_in_clk clock;
  sc_inout<bool> ready;

  bool debug;
  bool wasDelayedBranch;

  SC_CTOR(Unpiplined_IF) {
    debug = false;

    SC_CTHREAD(fetch, clock.pos());
    watching(reset.delayed() == true);
  }

  void fetch();
};

#endif
