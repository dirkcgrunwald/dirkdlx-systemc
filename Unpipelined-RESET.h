//-*-c++-*-
#ifndef _Unpiplined_RESET_h_
#define _Unpiplined_RESET_h_

#include "systemc.h"
#include "Unpipelined.h"
#include "DLXMemory.h"
#include "DLXMachineState.h"

struct Unpiplined_RESET : public sc_module {

  //
  // Define ports
  //
  sc_in_clk clock;
  sc_inout<bool> reset;

  sc_out<State2IF> to_fetch;
  sc_out<bool> fetch_ready;
  bool debug;

  SC_CTOR(Unpiplined_RESET) {
    SC_METHOD(resetmethod);
    sensitive << reset;
    debug = false;
  }

  void resetmethod();
};

#endif
