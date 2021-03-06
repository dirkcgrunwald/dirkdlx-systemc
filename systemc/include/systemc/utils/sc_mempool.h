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

  sc_mempool.h - Memory pools for small objects.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#ifndef SC_MEMPOOL_H
#define SC_MEMPOOL_H

#include "systemc/utils/sc_iostream.h"

class sc_mempool {
public:
    static void* allocate(size_t sz);
    static void release(void* p, size_t sz);
    static void display_statistics();
};

class sc_mpobject {
public:
    static void* operator new(size_t sz)              { return sc_mempool::allocate(sz); }
    static void operator delete(void* p, size_t sz)   { sc_mempool::release(p, sz); }
    static void* operator new[](size_t sz)            { return sc_mempool::allocate(sz); }
    static void operator delete[](void* p, size_t sz) { sc_mempool::release(p, sz); }
};

#endif   // SC_MEMPOOL_H

