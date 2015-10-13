//-*-c++-*-

#include "Pipelined-RESET.h"
void Pipelined_RESET::resetmethod()
{
  //
  // We got here because someone signaled 'advance'
  //
  if (debug) {
    cerr << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " "
	 << " RESET " << endl;
  }

  if ( reset == true ) {
    //
    // Prepare the signal to start execution at
    // the loader start
    //

    machineState.reset(loader.startloc(), 0xffffffff);

    State2IF start_if;
    start_if.pc = loader.startloc();
    start_if.in_shadow = false;
    start_if.delayed_pc = 0;
    to_fetch = start_if;
    fetch_ready = true;

    reset = false;
  }
}

