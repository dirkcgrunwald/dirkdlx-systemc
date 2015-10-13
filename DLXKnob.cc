#include <iostream.h>
#include <stdlib.h>
#include "DLXKnob.h"

//
// Knobs, or runtime options
//

KnobTableType _GlobalKnobTable;
static const char* WHITE_SPACE = " \n\r\t";

void WriteKnobs(FILE *file, KnobTableType& _table)
{

  //
  // output each item to a strstream and then
  // write that to the file
  //
  for (KnobTableType::iterator i = _table.begin();
       i != _table.end(); i++) {
    KnobBase *knob = (i -> second);
    string name = i -> first;
    ostrstream str;
    str << name << " -> ";
    knob -> getKnob(str);
    str << endl;
    char *chars = str.str();
    int len = strlen(chars);
    fwrite(chars, len, sizeof(char), file);
  }
}

bool
ReadKnob(char *name, char *value, KnobTableType& _table)
{
  KnobTableType::iterator found = _table.find(name);
  if ( found != _table.end() ) {
    istrstream istr(value);
    _table[name] -> setKnob(istr);
    return true;
  } else {
    return false;
  }
}

void ReadKnobs(FILE *file, KnobTableType& _table)
{
  while (!feof(file)) {
    char buffer[1024];
    buffer[0] = 0;
    fgets(buffer, sizeof(buffer), file);
    buffer[sizeof(buffer)-1] = 0;
    int len=strlen(buffer);
    if (len == 0 ) break;
    //
    // remove newline
    //
    buffer[len-1]=0;
    //
    // We want to find var -> value, but value can continue up
    // to a #. We strip all spaces.
    //
    int spn = strspn(buffer, WHITE_SPACE);
    char *varStart = buffer + spn;
    char *needle = strstr(varStart, "->");
    if (needle != NULL ) {
      *needle = 0;
      needle += 2; // Skip ->

      char *varEnd = varStart + strlen(varStart) - 1;
      //
      // Strip white space backwards
      //
      while (varEnd > varStart && strspn(varEnd, WHITE_SPACE) > 0 ) {
	*varEnd = 0;
	varEnd--;
      }

      char *valStart = needle + strspn(needle, WHITE_SPACE);
      char *valEnd = strstr(valStart, "#");
      if (valEnd != NULL) {
	*valEnd = 0; // Remove valEnd from buffer
	valEnd--;
      } else {
	valEnd = valStart + strlen(valStart) - 1; // end
      }
      //
      // Strip white space backwards
      //
      while (valEnd > valStart && strspn(valEnd, WHITE_SPACE) > 0 ) {
	*valEnd = 0;
	valEnd--;
      }
      cout << "name is *" << varStart << "*, value = *" << valStart << "*\n";
      ReadKnob(varStart, valStart);
    }
  }
}

void ParseCmdKnobs(int argc, char **argv, KnobTableType& _table)
{
  int i = 1;
  while (i < argc) {
    char *arg = argv[i];
    if ( arg ) {
      if (arg[0] == '-') {
	char tempArg[1024];
	int k=1;
	if ( arg[1] == '-' ) {
	  k = 2;
	}
	strncpy(tempArg, &arg[k], sizeof(tempArg));
	char *varStart = tempArg;
	char *needle = strstr(varStart,"=");
	if ( needle != NULL ) {
	  *needle = 0;
	  needle += 1; // skip = 

	  char *varEnd = varStart + strlen(varStart) - 1;
	  //
	  // Strip white space backwards
	  //
	  while (varEnd > varStart && strspn(varEnd, WHITE_SPACE) > 0 ) {
	    *varEnd = 0;
	    varEnd--;
	  }

	  char *valStart = needle + strspn(needle, WHITE_SPACE);
	  char *valEnd = strstr(valStart, "#");
	  if (valEnd != NULL) {
	    *valEnd = 0; // Remove valEnd from buffer
	    valEnd--;
	  } else {
	    valEnd = valStart + strlen(valStart) - 1; // end
	  }
	  //
	  // Strip white space backwards
	  //
	  while (valEnd > valStart && strspn(valEnd, WHITE_SPACE) > 0 ) {
	    *valEnd = 0;
	    valEnd--;
	  }
	  cout << "name is *" << varStart << "*, value = *" << valStart << "*\n";
	  ReadKnob(varStart, valStart);
	  i ++;
	} else {
	  //
	  // "needle" is null - this means there is no '=' in the
	  // argument and we'll assume the value is in the next arg
	  //
	  cout << "name is *" << tempArg << "*, value = *" << argv[i+1] << "*\n";
	  ReadKnob(tempArg, argv[i+1]);
	  i += 2;
	}
      }
    } else {
      i++;
    }
  }
}

