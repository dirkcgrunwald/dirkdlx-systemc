//-*-c++-*-
#ifndef _DLX_Knob_
#define _DLX_Knob_

#include <map>
#include <iomanip.h>
#include <strstream.h>
#include <stdio.h>
#include <string>

class KnobBase;
typedef map<string, KnobBase*> KnobTableType;
extern KnobTableType _GlobalKnobTable;

class KnobBase {
  protected:
  char *_desc;

  KnobBase(string name, char *desc, KnobTableType& _table) {
    _desc = desc;
    _table[name] = this;
  }

  virtual ~KnobBase() {
    // Empty
  }

  public:  
  virtual void getKnob(ostrstream& str) = 0;
  virtual void setKnob(istrstream& str) = 0;
};

template <class T>
class Knob : public KnobBase {
private:
  T _value;
  public:

  Knob(string name, char *desc, T value,
       KnobTableType& _table = _GlobalKnobTable)
  : KnobBase(name, desc, _table)
  {
    _value = value;
  }

  ~Knob() {
    // Empty
  }

  const T& value() {
    return _value;
  }

  const Knob<T>& operator=(const Knob<T>& knob) 
  {
    _value = knob.value();
    return *this;
  }

  const Knob<T>& operator=(const T& value) 
  {
    _value = value;
    return *this;
  }

  //
  // Convert a Knob<T> to a T
  //
  operator const T () const {
    return _value;
  }

  void getKnob(ostrstream& str)
  {
    str << _value;
  }

  void setKnob(istrstream& str)
  {
    str >> _value;
  }

};

//
// Functions that manipulate the global knob table
//
extern void WriteKnobs(FILE *file,
		       KnobTableType& _table = _GlobalKnobTable);

extern bool ReadKnob(char *name, char *value,
		     KnobTableType& _table = _GlobalKnobTable);

extern void ReadKnobs(FILE *file,
		      KnobTableType& _table = _GlobalKnobTable);

extern void ParseCmdKnobs(int argc, char **argv,
			  KnobTableType& _table = _GlobalKnobTable);
#endif
