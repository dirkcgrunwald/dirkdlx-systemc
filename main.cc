#include "DLXLoader.h"
#include "DLXMemory.h"

int
main(int, char **)
{
  DLXLoader loader;
  DLXMemory memory;
  loader.load("example.img", memory);

  cout << "Done!\n";
}
