#include "DLXMachineState.h"
#include <stdio.h>
#include <stdlib.h>
#include <values.h>
#include <ctype.h>

void
DLXMachineState::reset(uint32_t PC_, uint32_t SP_)
{
  PC = PC_;
  //
  // SP is r29
  // FP is r30 (but would be over-written on first procedure entry
  //
  setArchReg(29, SP_);
  setArchReg(30, SP_);
}
