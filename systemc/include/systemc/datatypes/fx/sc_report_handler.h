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

  sc_report_handler.h - 

  Original Author: Martin Janssen, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/


#ifndef SC_REPORT_HANDLER_H
#define SC_REPORT_HANDLER_H


#include "systemc/datatypes/fx/sc_report.h"

#if defined(__BCPLUSPLUS__)
#pragma hdrstop
#endif


// ----------------------------------------------------------------------------
//  CLASS : sc_report_handler
//
//  Default report handler class.
// ----------------------------------------------------------------------------

// ADD USAGE MODEL HERE

class sc_report_handler : public sc_report_handler_base
{

    friend class sc_report_handler_gc;

public:

    static void install();
    static void deinstall();

protected:

    sc_report_handler();
    virtual ~sc_report_handler();

    virtual void report( sc_severity /* report severity */,
			 const char* /* report id */,
			 const char* /* additional message */ ) const;

};


#endif

// Taf!
