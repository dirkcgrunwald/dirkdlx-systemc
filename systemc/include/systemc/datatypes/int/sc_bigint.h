/*****************************************************************************

  The following code is released by Synopsys, Inc., Copyright (c) 1996-2001,
  Synopsys, Inc., CoWare, Inc., Frontier Design, Inc. and others.
  All Rights reserved.

  The contents of this file are subject to the restrictions and limitations
  set forth in the SystemC Open Source License Version 2.2 (the "License");
  You may not use this file except in compliance with such restrictions and
  limitations. You may obtain instructions on how to receive a copy of the
  License at http://www.systemc.org/. Software distributed by Contributors
  under the License is distributed on an "AS IS" basis, WITHOUT WARRANTY OF
  ANY KIND, either express or implied. See the License for the specific
  language governing rights and limitations under the License.

 *****************************************************************************/

/*****************************************************************************

  sc_bigint.h -- Template version of sc_signed. This class enables
                 compile-time bit widths for sc_signed numbers.

  Original Author: Ali Dasdan, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date: Gene Bushayev, Synopsys, Inc.
  Description of Modification: - Interface between sc_bigint and sc_bv/sc_lv.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#ifndef SC_BIGINT_H
#define SC_BIGINT_H

#include "systemc/datatypes/int/sc_signed.h"
#include "systemc/datatypes/int/sc_unsigned.h"
#include "systemc/utils/sc_exception.h"

namespace sc_bv_ns
{
    class sc_bv_base;
    class sc_lv_base;
}
using sc_bv_ns::sc_bv_base;
using sc_bv_ns::sc_lv_base;

class sc_fxval;
class sc_fxval_fast;
class sc_fxnum;
class sc_fxnum_fast;

#ifdef SC_MAX_NBITS
template< int W = SC_MAX_NBITS >
#else
template< int W >
#endif
class sc_bigint : public sc_signed {

public:

  sc_bigint() : sc_signed(W)                            { }

  sc_bigint(const sc_bigint<W>&       v) : sc_signed(W) { *this = v; }

  sc_bigint(const sc_signed&          v) : sc_signed(W) { *this = v; }

  sc_bigint(const sc_signed_subref&   v) : sc_signed(W) { *this = v; }

  sc_bigint(const sc_unsigned&        v) : sc_signed(W) { *this = v; }

  sc_bigint(const sc_unsigned_subref& v) : sc_signed(W) { *this = v; }

  sc_bigint(const char*               v) : sc_signed(W) { *this = v; }

  sc_bigint(int64                     v) : sc_signed(W) { *this = v; }

  sc_bigint(uint64                    v) : sc_signed(W) { *this = v; }

  sc_bigint(long                      v) : sc_signed(W) { *this = v; }

  sc_bigint(unsigned long             v) : sc_signed(W) { *this = v; }

  sc_bigint(int                       v) : sc_signed(W) { *this = v; } 

  sc_bigint(unsigned int              v) : sc_signed(W) { *this = v; } 

  sc_bigint(double                    v) : sc_signed(W) { *this = v; }
  
  sc_bigint( const sc_bv_base& v ) : sc_signed( W ) { *this = v; }
  sc_bigint( const sc_lv_base& v ) : sc_signed( W ) { *this = v; }

#ifdef SC_INCLUDE_FX
  explicit sc_bigint( const sc_fxval& );
  explicit sc_bigint( const sc_fxval_fast& );
  explicit sc_bigint( const sc_fxnum& );
  explicit sc_bigint( const sc_fxnum_fast& );
#endif

#ifndef SC_MAX_NBITS
  ~sc_bigint() { }
#endif
 
  sc_bigint<W>& operator=(const sc_bigint<W>&       v)
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(const sc_signed&          v)
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(const sc_signed_subref&   v)
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(const sc_unsigned&        v)
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(const sc_unsigned_subref& v)
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(const char*               v) 
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(int64                     v)
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(uint64                    v)
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(long                      v)
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(unsigned long             v)
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(int                       v) 
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(unsigned int              v) 
  { sc_signed::operator=(v); return *this; }

  sc_bigint<W>& operator=(double                    v)
  { sc_signed::operator=(v); return *this; }


  sc_bigint<W>& operator = ( const sc_bv_base& v )
      { sc_signed::operator = ( v ); return *this; }

  sc_bigint<W>& operator = ( const sc_lv_base& v )
      { sc_signed::operator = ( v ); return *this; }


#ifdef SC_INCLUDE_FX
  sc_bigint& operator = ( const sc_fxval& );
  sc_bigint& operator = ( const sc_fxval_fast& );
  sc_bigint& operator = ( const sc_fxnum& );
  sc_bigint& operator = ( const sc_fxnum_fast& );
#endif
};


#endif
