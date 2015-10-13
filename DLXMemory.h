//-*-c++-*-
#ifndef _DLXMemory_h_
#define _DLXMemory_h_

#include <stdint.h>
#include <stdio.h>
#include <iostream.h>
#include <memory.h>
#include <assert.h>

//
// An abstraction for memory for the DLX system. This interface has no
// timing of performance components -- that needs to be provided by
// external timing logic written in e.g. SystemC
//
// This unit simply lets you access a 32-bit address space using byte,
// half-word or word access methods. The space is sparsely allocated,
// letting ussupport a large address space on a small machine.
//

typedef unsigned long DLX_addr_t;

class DLXMemory {
public:
  DLXMemory(bool debug = false);

  /////////////////////////////////////////////////////////////////////////////
  // Read interfaces
  /////////////////////////////////////////////////////////////////////////////
  uint8_t read_byte(DLX_addr_t addr);
  uint16_t read_half(DLX_addr_t addr);
  uint32_t read_word(DLX_addr_t addr);
  uint64_t read_qword(DLX_addr_t addr);

  /////////////////////////////////////////////////////////////////////////////
  // Write interfaces
  /////////////////////////////////////////////////////////////////////////////

  void write_byte(DLX_addr_t addr, uint8_t value);
  void write_half(DLX_addr_t addr, uint16_t value);
  void write_word(DLX_addr_t addr, uint32_t value);
  void write_qword(DLX_addr_t addr, uint64_t value);

  /////////////////////////////////////////////////////////////////////////////
  // Print interface
  /////////////////////////////////////////////////////////////////////////////
  
  void print(FILE*, DLX_addr_t from = 0, DLX_addr_t to = 0xffffffff);

private:

  //
  // Internal interfaces
  //
  void checkbyte(DLX_addr_t addr);
  uint8_t getbyte(DLX_addr_t addr);
  void setbyte(DLX_addr_t addr, uint8_t value);

  void printpage(FILE *file, DLX_addr_t base, uint8_t *page);

  //
  // Implement a sparse array of memory so this can run on a laptop.
  // The page map structure will need 2^(32-15) * 4 == 512K of space.
  //
  
  static const int pagesize_shift = 14;
  static const int pagesize_mask = ~(-1 << pagesize_shift);
  static const int pagesize = (1 << pagesize_shift);
  bool debug_;
  unsigned long long low_;
  unsigned long long high_;
  unsigned long bytes;
  int pages;
  uint8_t **storage;
public:

};

#endif
