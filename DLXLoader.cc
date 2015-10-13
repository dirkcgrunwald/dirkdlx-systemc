#include "DLXLoader.h"
#include "DLXMemory.h"

DLXLoader::DLXLoader()
{
  //
  // Empty
  //
}

bool
DLXLoader::load(char* imageFile, DLXMemory& ram, bool debug)
{
  FILE *fin = fopen(imageFile, "r");
  if ( ! fin ) {
    cerr << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " "
	 << "Unable to open image file " << imageFile << endl;
    return false;
  }

  char buffer[1024];
  fgets(buffer, sizeof(buffer), fin);
  buffer[sizeof(buffer)-1] = 0;

  uint32_t a, b, c, d, e, f;
  int cnt = sscanf(buffer, "start:%08x %08x %08x %08x %08x %08x\n",
		   &a, &b, &c, &d, &e, &f);
  if ( cnt != 6 ) {
    cerr << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " "
	 << "DLXLoaderUnable to read 6 starting fields" << endl;
    return false;
  }
  
  _startloc = a;
  _endloc = b;
  _text_start = c;
  _text_size = d;
  _data_start = e;
  _data_size = f;

  DLX_addr_t addr = 0;
  uint32_t contents = 0;

  while (!feof(fin)) {
    char *foo = fgets(buffer, sizeof(buffer), fin);
    if ( foo == NULL ) {
      break;
    }
    buffer[sizeof(buffer)-1] = 0;
    if (strlen(buffer) == 0 ) break;

    //
    // Check for new starting address or continuation address
    //
    if ( sscanf(buffer, "%08x:%08x\n", &a, &b) == 2 ) {
      addr = a;
      contents = b;
    } else if (sscanf(buffer, ":%08x\n", &b) == 1) {
      contents = b;
    } else {
      cerr << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " "
	   << "Unable to parse nonsense from load image" << endl;
      cerr << "line is " << buffer << endl;
      return false;
    }
    if ( debug ) {
      cout << "Load " << hex << addr << ": " << contents << dec << endl;
    }
    ram.write_word(addr, contents);
    addr += 4;
  }

  return true;
}
