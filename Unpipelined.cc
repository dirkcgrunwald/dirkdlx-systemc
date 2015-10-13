#include <iostream.h>
#include "systemc.h"
#include "DLXLoader.h"
#include "DLXMemory.h"
#include "DLXMachineState.h"
#include "DLXKnob.h"
#include "Unpipelined-RESET.h"
#include "Unpipelined-IF.h"
#include "Unpipelined-ID.h"
#include "Unpipelined-EX.h"
#include "Unpipelined-MEM.h"
#include "Unpipelined-WB.h"

DLXMemory memory;
DLXMachineState machineState;
DLXLoader loader;

Knob<bool> debug("debug", "Enable debugging", true);
Knob<int> cycles("cycles", "Number of cycles to simulate", 40);

int
sc_main(int argc, char **argv)
{
  //
  // Parse the options looking for a cycles
  //
  
  ParseCmdKnobs(argc, argv);

  loader.load("example.img", memory);
  cout << "memory image loaded" << endl;

  //
  // Create the modules, tell them about memory & machine state
  //

  Unpiplined_RESET stage_reset("reset_stage");
  stage_reset.debug = debug;

  Unpiplined_IF stage_if("fetch_stage");
  stage_if.debug = debug;

  Unpiplined_ID stage_id("decode_stage");
  stage_id.debug = debug;

  Unpiplined_EX stage_ex("exec_stage");
  stage_ex.debug = debug;

  Unpiplined_MEM stage_mem("memory_stage");
  stage_mem.debug = debug;

  Unpiplined_WB stage_wb("writeback_stage");
  stage_wb.debug = debug;

  //
  // Create the system clock and reset signal
  //
  sc_clock clk("clock");
  stage_reset.clock(clk);
  stage_if.clock(clk);
  stage_id.clock(clk);
  stage_ex.clock(clk);
  stage_mem.clock(clk);
  stage_wb.clock(clk);

  sc_signal<bool> reset;
  stage_reset.reset(reset);
  stage_if.reset(reset);
  stage_id.reset(reset);
  stage_ex.reset(reset);
  stage_mem.reset(reset);
  stage_wb.reset(reset);

  //
  // Wire the modules together
  //
  sc_signal<StateIF2ID> from_if_2_id;

  stage_if.to_decode(from_if_2_id);
  stage_id.from_fetch(from_if_2_id);
  
  sc_signal<StateID2EX> from_id_2_ex;
  stage_id.to_exec(from_id_2_ex);
  stage_ex.from_decode(from_id_2_ex);

  sc_signal<StateID2EX> from_ex_2_mem;
  stage_ex.to_mem(from_ex_2_mem);
  stage_mem.from_exec(from_ex_2_mem);

  sc_signal<StateID2EX> from_mem_2_wb;
  stage_mem.to_wb(from_mem_2_wb);
  stage_wb.from_mem(from_mem_2_wb);
  
  sc_signal<State2IF> from_wb_2_fetch;
  stage_wb.to_fetch(from_wb_2_fetch);
  stage_if.from_wb(from_wb_2_fetch);

  //
  // Reset module talks only to fetch
  stage_reset.to_fetch(from_wb_2_fetch);

  //
  // Wire the "ready" lines
  //

  sc_signal<bool> fetch_ready;
  sc_signal<bool> decode_ready;
  sc_signal<bool> exec_ready;
  sc_signal<bool> mem_ready;
  sc_signal<bool> wb_ready;

  stage_if.ready(fetch_ready);
  stage_id.ready(decode_ready);
  stage_ex.ready(exec_ready);
  stage_mem.ready(mem_ready);
  stage_wb.ready(wb_ready);

  stage_if.decode_ready(decode_ready);
  stage_id.exec_ready(exec_ready);
  stage_ex.mem_ready(mem_ready);
  stage_mem.wb_ready(wb_ready);
  stage_wb.fetch_ready(fetch_ready);
  stage_reset.fetch_ready(fetch_ready);

  //
  // Set up tracing
  //
  sc_trace_file *tf = sc_create_vcd_trace_file("Pipeline-Trace");
  sc_trace(tf, from_mem_2_wb, "from_wb_2_if");
  sc_trace(tf, from_if_2_id, "from_if_2_id");
  sc_trace(tf, from_id_2_ex, "from_id_2_ex");
  sc_trace(tf, from_ex_2_mem, "from_ex_2_mem");
  sc_trace(tf, from_mem_2_wb, "from_mem_2_wb");

  //
  // Prime the pump
  //
  reset.write(true);

  sc_start(cycles);
  
  sc_close_vcd_trace_file(tf);

  fprintf(stdout, "*******************************************\n");
  fprintf(stdout, "** Simulation ended, memory dump follows **\n");
  fprintf(stdout, "*******************************************\n");
  memory.print(stdout);

  return(0);

}

void sc_trace(sc_trace_file* tf, const State2IF& v, const sc_string& n)
{
  sc_trace(tf, v.pc, n + ".pc");
  sc_trace(tf, v.in_shadow, n + ".in_shadow");
  sc_trace(tf, v.delayed_pc, n + ".delayed_pc");
}

void sc_trace(sc_trace_file* tf, const StateIF2ID& v, const sc_string& n)
{
  sc_trace(tf, v.pc, n + ".pc");
  sc_trace(tf, v.insn, n + ".insn");
  sc_trace(tf, v.delayed_pc, n + ".delayed_pc");
}

void sc_trace(sc_trace_file* tf, const StateID2EX& v, const sc_string& n)
{
  sc_trace(tf, v.insn, n + ".insn");
  sc_trace(tf, v.format, n + ".format");
  sc_trace(tf, v.opcode, n + ".opcode");
  sc_trace(tf, v.pc, n + ".pc");
  sc_trace(tf, v.npc, n + ".npc");
  sc_trace(tf, v.delayed_pc, n + ".delayed_pc");
  sc_trace(tf, v.A, n + ".A");
  sc_trace(tf, v.B, n + ".B");
  sc_trace(tf, v.rd, n + ".rd");
  sc_trace(tf, v.imm, n + ".imm");
  sc_trace(tf, v.alu_output, n + ".alu_output");
  sc_trace(tf, v.r31_output, n + ".r31_output");
  sc_trace(tf, v.lmb, n + ".lmb");
}
