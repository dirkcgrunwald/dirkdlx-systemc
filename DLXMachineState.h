//-*-c++-*-
#ifndef _DLXMachineState_hh_
#define _DLXMachineState_hh_

#include <stdint.h>

class DLXMachineState {
protected:
  //
  // Architected machine state
  //
  static const int ARCH_REGS = 32;
  uint32_t regs[ARCH_REGS];
  uint32_t PC;   
  uint32_t NPC;

public:
  //
  // Initialize the machine state
  //
  DLXMachineState()
  {
    int num_regs = (sizeof(regs) / sizeof(regs[0]));
    for (int i = 0 ; i < num_regs ; i++) {
      regs[i] = 0;
    }
  }

  void reset(uint32_t PC_, uint32_t SP_);

  //
  // Define interfaces to the architected registers - these should be
  // used rather than the raw register numbers since we may later
  // implement register renaming
  //
  uint32_t getArchReg(int i) {
    return regs[i];
  }

  void setArchReg(int i, uint32_t v) {
    regs[i] = v;
  }

  uint32_t getPC() {
    return PC;
  }

  void setPC(uint32_t value) {
    PC = value;
  }
};

#endif
