//-*-c++-*-
#ifndef _Pipeline_h_
#define _Pipeline_h_

#include "DLX.h"
#include "DLXMemory.h"
#include "DLXMachineState.h"
#include "DLXLoader.h"

extern DLXMemory memory;
extern DLXMachineState machineState;
extern DLXLoader loader;


//
// Sent to fetch unit - we specify the address of the instruction to fetch.
// For the non-pipelined version, we need to track the instruction state.
//

struct State2IF {
  uint32_t pc;
  bool in_shadow;
  uint32_t delayed_pc;

  inline bool operator == (const State2IF& rhs) {
    return pc == rhs.pc
      && in_shadow == rhs.in_shadow
      && delayed_pc == rhs.delayed_pc;
  }
};
void sc_trace(sc_trace_file*, const State2IF&, const sc_string&);

/////////////////////////////////////////////////////////////////////////////
// Data sent from IF to ID
/////////////////////////////////////////////////////////////////////////////

struct StateIF2ID {
  uint32_t pc;
  uint32_t insn;
  bool in_shadow;
  uint32_t delayed_pc;

  inline bool operator == (const StateIF2ID& rhs) {
    return pc == rhs.pc
      && insn == rhs.insn
      && in_shadow == rhs.in_shadow
      && delayed_pc == rhs.delayed_pc;
  }

};

void sc_trace(sc_trace_file*, const StateIF2ID&, const sc_string&);
//
// Sent from ID -> EX -> MEM -> WB
//
struct StateID2EX {

  uint32_t insn;
  int format;
  OpcodeEnum opcode;

  //
  // Of this instruction...
  //
  int pc;
  int npc;
  bool in_shadow;
  uint32_t delayed_pc;

  uint32_t A;
  uint32_t B;

  int rd;

  int imm;

  //
  // These are really only used in the EX -> MEM stages
  //
  int alu_output;
  int r31_output;
  //
  // And in the MEM -> WB stages
  //
  uint32_t lmb;

  inline bool operator == (const StateID2EX& rhs) {
    return insn == rhs.insn
      && format == rhs.format
      && opcode == rhs.opcode
      && pc == rhs.pc
      && npc == rhs.npc
      && in_shadow == rhs.in_shadow
      && delayed_pc == rhs.delayed_pc
      && A == rhs.A
      && B == rhs.B
      && rd == rhs.rd
      && imm == rhs.imm
      && alu_output == rhs.alu_output
      && r31_output == rhs.r31_output
      && lmb == rhs.lmb;
  }


};
void sc_trace(sc_trace_file*, const StateID2EX&, const sc_string&);

#endif
