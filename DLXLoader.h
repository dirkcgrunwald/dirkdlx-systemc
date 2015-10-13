//-*-c++-*-
#ifndef _DLXLoader_h_
#define _DLXLoader_h_

#include <stdint.h>

class DLXMemory;

//
// Load a memory image into a memory.
//
class DLXLoader
{
  uint32_t _startloc;
  uint32_t _endloc;
  uint32_t _text_start;
  uint32_t _text_size;
  uint32_t _data_start;
  uint32_t _data_size;

public:
  DLXLoader();
  bool load(char *memoryImageFile, DLXMemory&, bool debug = false);

  uint32_t startloc() { return _startloc; }
  uint32_t endloc() { return _endloc; }
  uint32_t text_start() { return _text_start; }
  uint32_t text_size() { return _text_size; }
  uint32_t data_start() { return _data_start; }
  uint32_t data_size() { return _data_size; }
  
};

#endif
