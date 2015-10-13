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

  sc_fifo_ports.h -- The sc_fifo<T> port classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_FIFO_PORTS_H
#define SC_FIFO_PORTS_H


#include "systemc/communication/sc_port.h"
#include "systemc/communication/sc_fifo_ifs.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_fifo_in<T>
//
//  The sc_fifo<T> input port class.
// ----------------------------------------------------------------------------

template <class T>
class sc_fifo_in
: public sc_port<sc_fifo_in_if<T>,0>
{
public:

    // typedefs

    typedef T                        data_type;

    typedef sc_fifo_in_if<data_type> if_type;
    typedef sc_port<if_type,0>       base_type;
    typedef sc_fifo_in<data_type>    this_type;

    typedef if_type                  in_if_type;
    typedef sc_port_b<in_if_type>    in_port_type;

public:

    // constructors

    sc_fifo_in()
	: base_type()
	{}

    explicit sc_fifo_in( in_if_type& interface_ )
	: base_type( interface_ )
	{}

    explicit sc_fifo_in( in_port_type& parent_ )
	: base_type( parent_ )
	{}

    sc_fifo_in( this_type& parent_ )
	: base_type( parent_ )
	{}


    // destructor (does nothing)

    virtual ~sc_fifo_in()
	{}

private:

    // disabled
    sc_fifo_in( const this_type& );
    this_type& operator = ( const this_type& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_fifo_out<T>
//
//  The sc_fifo<T> output port class.
// ----------------------------------------------------------------------------

template <class T>
class sc_fifo_out
: public sc_port<sc_fifo_out_if<T>,0>
{
public:

    // typedefs

    typedef T                         data_type;

    typedef sc_fifo_out_if<data_type> if_type;
    typedef sc_port<if_type,0>        base_type;
    typedef sc_fifo_out<data_type>    this_type;

    typedef if_type                   out_if_type;
    typedef sc_port_b<out_if_type>    out_port_type;

public:

    // constructors

    sc_fifo_out()
	: base_type()
	{}

    explicit sc_fifo_out( out_if_type& interface_ )
	: base_type( interface_ )
	{}

    explicit sc_fifo_out( out_port_type& parent_ )
	: base_type( parent_ )
	{}

    sc_fifo_out( this_type& parent_ )
	: base_type( parent_ )
	{}


    // destructor (does nothing)

    virtual ~sc_fifo_out()
	{}

private:

    // disabled
    sc_fifo_out( const this_type& );
    this_type& operator = ( const this_type& );
};


#endif

// Taf!
