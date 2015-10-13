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

  sc_signal_rv_ports.h -- The resolved vector signal ports.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_SIGNAL_RV_PORTS_H
#define SC_SIGNAL_RV_PORTS_H


#include "systemc/communication/sc_signal_ports.h"
#include "systemc/datatypes/bit/sc_lv.h"

using sc_bv_ns::sc_lv;


// ----------------------------------------------------------------------------
//  CLASS : sc_in_rv<W>
//
//  The sc_signal_rv<W> input port class.
// ----------------------------------------------------------------------------

template <int W>
class sc_in_rv
: public sc_in<sc_lv<W> >
{
public:

    // typedefs

    typedef sc_lv<W>                   data_type;

    typedef sc_in_rv<W>                this_type;
    typedef sc_in<data_type>           base_type;

    typedef typename base_type::in_if_type      in_if_type;
    typedef typename base_type::in_port_type    in_port_type;
    typedef typename base_type::inout_port_type inout_port_type;

public:

    // constructors

    sc_in_rv()
	: base_type()
	{}

    explicit sc_in_rv( const in_if_type& interface_ )
	: base_type( interface_ )
	{}

    explicit sc_in_rv( in_port_type& parent_ )
	: base_type( parent_ )
	{}

    explicit sc_in_rv( inout_port_type& parent_ )
	: base_type()
	{}

    sc_in_rv( this_type& parent_ )
	: base_type( parent_ )
	{}


    // destructor (does nothing)

    virtual ~sc_in_rv()
	{}

private:

    // disabled
    sc_in_rv( const this_type& );
    this_type& operator = ( const this_type& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_inout_rv<W>
//
//  The sc_signal_rv<W> input/output port class.
// ----------------------------------------------------------------------------

template <int W>
class sc_inout_rv
: public sc_inout<sc_lv<W> >
{
public:

    // typedefs

    typedef sc_lv<W>                   data_type;

    typedef sc_inout_rv<W>             this_type;
    typedef sc_inout<data_type>        base_type;

    typedef typename base_type::in_if_type      in_if_type;
    typedef typename base_type::in_port_type    in_port_type;
    typedef typename base_type::inout_if_type   inout_if_type;
    typedef typename base_type::inout_port_type inout_port_type;

public:

    // constructors

    sc_inout_rv()
	: base_type()
	{}

    explicit sc_inout_rv( inout_if_type& interface_ )
	: base_type( interface_ )
	{}

    explicit sc_inout_rv( inout_port_type& parent_ )
	: base_type( parent_ )
	{}

    sc_inout_rv( this_type& parent_ )
	: base_type( parent_ )
	{}


    // destructor (does nothing)

    virtual ~sc_inout_rv()
	{}


    // write the new value

    this_type& operator = ( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

    this_type& operator = ( const in_if_type& interface_ )
	{ (*this)->write( interface_.read() ); return *this; }

    this_type& operator = ( const in_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const inout_port_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

    this_type& operator = ( const this_type& port_ )
	{ (*this)->write( port_->read() ); return *this; }

private:

    // disabled
    sc_inout_rv( const this_type& );
};


// ----------------------------------------------------------------------------
//  CLASS : sc_out_rv<W>
//
//  The sc_signal_rv<W> output port class.
// ----------------------------------------------------------------------------

// sc_out_rv can also read from its port, hence no difference with
// sc_inout_rv.

#define sc_out_rv sc_inout_rv


#endif

// Taf!
