#include <assert.h>
#include "DLXMemory.h"

DLXMemory::DLXMemory(bool debug = false)
{
  debug_ = debug;
  low_ = 0;
  high_ = 0xffffffff;
  unsigned long long bytes = high_ - low_ + 1;
  pages = (bytes + pagesize-1) / pagesize;
  assert(pages > 0);
  storage = new uint8_t*[pages];
  for(int i = 0; i < pages; i++) {
    storage[i] = NULL;
  }
}

//
// Internal interfaces
//
void
DLXMemory::checkbyte(DLX_addr_t addr)
{
  int page = addr >> pagesize_shift;
  if ( storage[page] == NULL ) {
    if(debug_) {
      cerr << "Allocate page for at " << addr << endl;
    }
    uint8_t *p = new uint8_t[pagesize];
    memset(p, 0, pagesize);
    storage[page] = p;
  }
}

inline uint8_t DLXMemory::getbyte(DLX_addr_t addr) {
  assert(addr >= 0);
  int page = addr >> pagesize_shift;
  int offset = addr & pagesize_mask;
  uint8_t value = storage[page][offset];
  return value;
}

inline void  DLXMemory::setbyte(DLX_addr_t addr, uint8_t value) {
  int page = addr >> pagesize_shift;
  int offset = addr & pagesize_mask;
  storage[page][offset] = value;
}

/////////////////////////////////////////////////////////////////////////////
// Read interfaces
/////////////////////////////////////////////////////////////////////////////
uint8_t DLXMemory::read_byte(DLX_addr_t addr)
{
  DLX_addr_t base = addr - low_;
  checkbyte(base);
  uint8_t value = getbyte(base);
  if ( debug_ ) {
    cerr << "[Physmem] Read " << hex << (int) value
	 << " from " << hex << addr << dec << endl;
  }
  return value;
}

uint16_t DLXMemory::read_half(DLX_addr_t addr)
{
  DLX_addr_t base = addr - low_;
  checkbyte(base);
  checkbyte(base+1);
  uint16_t value = getbyte(base)
    | ( getbyte(base+1) << 8 );

  if ( debug_ ) {
    cerr << "[Physmem] Read " << hex << value
	 << " from " << hex << addr << dec << endl;
  }
  return value;
}

uint32_t DLXMemory::read_word(DLX_addr_t addr)
{
  DLX_addr_t base = addr - low_;
  checkbyte(base);
  checkbyte(base+3);
  uint32_t value = getbyte(base)
    | ( getbyte(base+1) << 8 )
    | ( getbyte(base+2) << 16 )
    | ( getbyte(base+3) << 24 )
    ;

  if ( debug_ ) {
    cerr << "[Physmem] Read " << hex << value
	 << " from " << hex << addr << dec << endl;
  }
  return value;
}

uint64_t DLXMemory::read_qword(DLX_addr_t addr)
{
  DLX_addr_t base = addr - low_;
  checkbyte(base);
  checkbyte(base+7);
  uint64_t value = getbyte(base)
    | ( ( (uint64_t) getbyte(base+1) ) << 8 )
    | ( ( (uint64_t) getbyte(base+2) ) << 16 )
    | ( ( (uint64_t) getbyte(base+3) ) << 24 )
    | ( ( (uint64_t) getbyte(base+4) ) << 32 )
    | ( ( (uint64_t) getbyte(base+5) ) << 40 )
    | ( ( (uint64_t) getbyte(base+6) ) << 48 )
    | ( ( (uint64_t) getbyte(base+7) ) << 56 )
    ;

  if ( debug_ ) {
    cerr << "[Physmem] Read " << hex << value
	 << " from " << hex << addr << dec << endl;
  }
  return value;
}

/////////////////////////////////////////////////////////////////////////////
// Write interfaces
/////////////////////////////////////////////////////////////////////////////

void DLXMemory::write_byte(DLX_addr_t addr, uint8_t value)
{
  DLX_addr_t base = addr - low_;
  checkbyte(base);
  setbyte(base, value & 0xff);
  if ( debug_ ) {
    cerr << "[Physmem] Write " << hex << (int) value << dec
	 << " to " << hex << addr << dec << endl;
  }
}

void DLXMemory::write_half(DLX_addr_t addr, uint16_t value)
{
  DLX_addr_t base = addr - low_;
  checkbyte(base);
  checkbyte(base+1);
  setbyte(base, value & 0xff);
  setbyte(base+1, (value >> 8) & 0xff );

  if ( debug_ ) {
    cerr << "[Physmem] Write " << hex << value << " to " << hex << addr << dec << endl;
  }
}

void DLXMemory::write_word(DLX_addr_t addr, uint32_t value)
{
  DLX_addr_t base = addr - low_;
  checkbyte(base);
  checkbyte(base+3);
  setbyte(base, value & 0xff );
  setbyte(base+1, (value >> 8) & 0xff );
  setbyte(base+2, (value >> 16) & 0xff );
  setbyte(base+3, (value >> 24) & 0xff );

  if ( debug_ ) {
    cerr << "[Physmem] Write " << hex << value << " to " << hex << addr << dec << endl;
  }
}

void DLXMemory::write_qword(DLX_addr_t addr, uint64_t value)
{
  DLX_addr_t base = addr - low_;
  checkbyte(base);
  checkbyte(base+3);
  setbyte(base, value & 0xff);
  setbyte(base+1, (value >> 8) & 0xff );
  setbyte(base+2, (value >> 16) & 0xff );
  setbyte(base+3, (value >> 24) & 0xff );
  setbyte(base+4, (value >> 32) & 0xff );
  setbyte(base+5, (value >> 40) & 0xff );
  setbyte(base+6, (value >> 48) & 0xff );
  setbyte(base+7, (value >> 56) & 0xff );

  if ( debug_ ) {
    cerr << "[Physmem] Write " << hex << value << " to " << hex << addr << dec << endl;
  }
}

void
DLXMemory::printpage(FILE *file, DLX_addr_t base, uint8_t *page)
{
  uint32_t *p = (uint32_t*) page;
  uint32_t size = pagesize / sizeof(uint32_t);
  uint32_t lastlabel = pagesize+10;
  char *sep = "";
  int cols = 0;
  for (uint32_t i = 0 ; i < size; i++) {
    if (p[i] != 0) {

      if ( i != lastlabel+1 || cols > 4) {
	fprintf(file, sep);
	sep = "\n";
	fprintf(file, "%08x : ", base + i * sizeof(uint32_t));
	cols = 0;
	lastlabel = i;
      }

      fprintf(file, "\t%08x", p[i]);
      cols++;
      lastlabel = i;
    }
  }
  fprintf(file, sep);
}

void
DLXMemory::print(FILE* file, DLX_addr_t from, DLX_addr_t to)
{
  //
  // Shift to page, then walk pages
  //
  uint64_t from_page = ((uint64_t) from >> pagesize_shift);
  uint64_t to_page = (((uint64_t) to + pagesize-1) >> pagesize_shift);

  for (uint64_t i = from_page; i <= to_page; i++) {
    if (storage[i]) {
      printpage(file, i * pagesize, storage[i]);
    }
  }
}

