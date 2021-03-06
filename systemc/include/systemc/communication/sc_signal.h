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

  sc_signal.h -- The sc_signal<T> primitive channel class.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_SIGNAL_H
#define SC_SIGNAL_H


#include "systemc/communication/sc_port.h"
#include "systemc/communication/sc_prim_channel.h"
#include "systemc/communication/sc_signal_ifs.h"
#include "systemc/kernel/sc_event.h"
#include "systemc/kernel/sc_simcontext.h"
#include "systemc/datatypes/bit/sc_logic.h"
#include "systemc/tracing/sc_trace.h"
#include "systemc/utils/sc_exception.h"
#include "systemc/utils/sc_string.h"
#include <typeinfo>


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<T>
//
//  The sc_signal<T> primitive channel class.
// ----------------------------------------------------------------------------

template <class T>
class sc_signal
: public sc_prim_channel,
  public sc_signal_inout_if<T>
{
public:

    // constructors

    sc_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
          m_output( 0 ), m_cur_val( T() ), m_new_val( T() ), m_old_val( T() ),
          m_delta( ~1ull )
	{}

    explicit sc_signal( const char* name_ )
	: sc_prim_channel( name_ ),
          m_output( 0 ), m_cur_val( T() ), m_new_val( T() ), m_old_val( T() ),
          m_delta( ~1ull )
	{}


    // destructor (does nothing)

    virtual ~sc_signal()
	{}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ return m_value_changed_event; }


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ return m_value_changed_event; }


    // read the current value
    virtual const T& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const T& get_data_ref() const
        { return m_cur_val; }


    // was there an event?
    virtual bool event() const
        { return ( simcontext()->delta_count() == m_delta + 1 ); }

    // write the new value
    virtual void write( const T& );


    // other methods

    operator const T& () const
	{ return read(); }


    sc_signal<T>& operator = ( const T& a )
	{ write( a ); return *this; }

    sc_signal<T>& operator = ( const sc_signal<T>& a )
	{ write( a.read() ); return *this; }


    const T& get_new_value() const
	{ return m_new_val; }

    const T& get_old_value() const
	{ return m_old_val; }


    void trace( sc_trace_file* tf ) const
	{ ::sc_trace( tf, get_data_ref(), name() ); }


#ifdef DEBUG_SYSTEMC
    virtual void print( ostream& ) const;
    virtual void dump( ostream& ) const;
#endif


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    virtual void update();

protected:

    sc_port_base* m_output; // used for static design rule checking

    T             m_cur_val;
    T             m_new_val;
    T             m_old_val;

    sc_event      m_value_changed_event;

    uint64        m_delta; // delta of last event

private:

    // disabled
    sc_signal( const sc_signal<T>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

template <class T>
const char* const sc_signal<T>::kind_string = "sc_signal";


template <class T>
inline
void
sc_signal<T>::register_port( sc_port_base& port_, const char* if_typename_ )
{
#ifdef DEBUG_SYSTEMC
    sc_string nm( if_typename_ );
    if( nm == typeid( sc_signal_inout_if<T> ).name() ) {
	// an out or inout port; only one can be connected
	if( m_output != 0 ) {
	    char msg[BUFSIZ];
	    sprintf( msg, "\n signal `%s'"
                          "\n first driver `%s'"
		          "\n second driver `%s'",
		     name(), m_output->name(), port_.name() );
	    REPORT_ERROR( 7041, msg );
	}
	m_output = &port_;
    }
#endif
}


// write the new value

template <class T>
inline
void
sc_signal<T>::write( const T& value_ )
{
    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }
}


#ifdef DEBUG_SYSTEMC

template <class T>
inline
void
sc_signal<T>::print( ostream& os ) const
{
    os << m_cur_val;
}

template <class T>
inline
void
sc_signal<T>::dump( ostream& os ) const
{
    os << "     name = " << name() << endl;
    os << "    value = " << m_cur_val << endl;
    os << "new value = " << m_new_val << endl;
    os << "old value = " << m_old_val << endl;
}

#endif


template <class T>
inline
void
sc_signal<T>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
	m_old_val = m_cur_val;
	m_cur_val = m_new_val;
	m_value_changed_event.notify_delayed();
	m_delta = simcontext()->delta_count();
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<bool>
//
//  Specialization of sc_signal<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class sc_signal<bool>
: public sc_prim_channel,
  public sc_signal_inout_if<bool>
{
public:

    // constructors

    sc_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
          m_output( 0 ),
          m_cur_val( false ),
          m_new_val( false ),
          m_old_val( false ),
          m_delta( ~1ull )
	{}

    explicit sc_signal( const char* name_ )
	: sc_prim_channel( name_ ),
          m_output( 0 ),
          m_cur_val( false ),
          m_new_val( false ),
          m_old_val( false ),
          m_delta( ~1ull )
	{}


    // destructor (does nothing)

    virtual ~sc_signal()
	{}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ return m_value_changed_event; }


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ return m_value_changed_event; }

    // get the positive edge event
    virtual const sc_event& posedge_event() const
	{ return m_posedge_event; }

    // get the negative edge event
    virtual const sc_event& negedge_event() const
	{ return m_negedge_event; }


    // read the current value
    virtual const bool& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const bool& get_data_ref() const
        { return m_cur_val; }


    // was there a value changed event?
    virtual bool event() const
        { return ( simcontext()->delta_count() == m_delta + 1 ); }

    // was there a positive edge event?
    virtual bool posedge() const
	{ return ( event() && m_cur_val ); }

    // was there a negative edge event?
    virtual bool negedge() const
	{ return ( event() && ! m_cur_val ); }


    // write the new value
    virtual void write( const bool& );


    // delayed evaluation
    virtual const sc_signal_bool_deval& delayed() const;


    // other methods

    operator const bool& () const
	{ return read(); }


    sc_signal<bool>& operator = ( const bool& a )
	{ write( a ); return *this; }

    sc_signal<bool>& operator = ( const sc_signal<bool>& a )
	{ write( a.read() ); return *this; }


    const bool& get_new_value() const
	{ return m_new_val; }

    const bool& get_old_value() const
	{ return m_old_val; }


    void trace( sc_trace_file* tf ) const
	{ ::sc_trace( tf, get_data_ref(), name() ); }


#ifdef DEBUG_SYSTEMC
    virtual void print( ostream& ) const;
    virtual void dump( ostream& ) const;
#endif


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    virtual void update();

protected:

    sc_port_base* m_output; // used for static design rule checking

    bool          m_cur_val;
    bool          m_new_val;
    bool          m_old_val;

    sc_event      m_value_changed_event;
    sc_event      m_posedge_event;
    sc_event      m_negedge_event;

    uint64        m_delta; // delta of last event

private:

    // disabled
    sc_signal( const sc_signal<bool>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
sc_signal<bool>::register_port( sc_port_base& port_, const char* if_typename_ )
{
#ifdef DEBUG_SYSTEMC
    sc_string nm( if_typename_ );
    if( nm == typeid( sc_signal_inout_if<bool> ).name() ) {
	// an out or inout port; only one can be connected
	if( m_output != 0 ) {
	    char msg[BUFSIZ];
	    sprintf( msg, "\n signal `%s'"
                          "\n first driver `%s'"
		          "\n second driver `%s'",
		     name(), m_output->name(), port_.name() );
	    REPORT_ERROR( 7041, msg );
	}
	m_output = &port_;
    }
#endif
}


// write the new value

inline
void
sc_signal<bool>::write( const bool& value_ )
{
    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }
}


// delayed evaluation

inline
const sc_signal_bool_deval&
sc_signal<bool>::delayed() const
{
    const sc_signal_in_if<bool>* iface = this;
    return RCAST<const sc_signal_bool_deval&>( *iface );
}


#ifdef DEBUG_SYSTEMC

inline
void
sc_signal<bool>::print( ostream& os ) const
{
    os << m_cur_val;
}

inline
void
sc_signal<bool>::dump( ostream& os ) const
{
    os << "     name = " << name() << endl;
    os << "    value = " << m_cur_val << endl;
    os << "new value = " << m_new_val << endl;
    os << "old value = " << m_old_val << endl;
}

#endif


inline
void
sc_signal<bool>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
	m_old_val = m_cur_val;
	m_cur_val = m_new_val;
	m_value_changed_event.notify_delayed();
	if( m_cur_val ) {
	    m_posedge_event.notify_delayed();
	} else {
	    m_negedge_event.notify_delayed();
	}
	m_delta = simcontext()->delta_count();
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_signal<sc_logic>
//
//  Specialization of sc_signal<T> for type sc_logic.
// ----------------------------------------------------------------------------

template <>
class sc_signal<sc_logic>
: public sc_prim_channel,
  public sc_signal_inout_if<sc_logic>
{
public:

    // constructors

    sc_signal()
	: sc_prim_channel( sc_gen_unique_name( "signal" ) ),
          m_output( 0 ), m_cur_val( 'X' ), m_new_val( 'X' ), m_old_val( 'X' ),
          m_delta( ~1ull )
	{}

    explicit sc_signal( const char* name_ )
	: sc_prim_channel( name_ ),
          m_output( 0 ), m_cur_val( 'X' ), m_new_val( 'X' ), m_old_val( 'X' ),
          m_delta( ~1ull )
	{}


    // destructor (does nothing)

    virtual ~sc_signal()
	{}


    // interface methods

    virtual void register_port( sc_port_base&, const char* );


    // get the default event
    virtual const sc_event& default_event() const
	{ return m_value_changed_event; }


    // get the value changed event
    virtual const sc_event& value_changed_event() const
	{ return m_value_changed_event; }


    // read the current value
    virtual const sc_logic& read() const
	{ return m_cur_val; }

    // get a reference to the current value (for tracing)
    virtual const sc_logic& get_data_ref() const
        { return m_cur_val; }


    // was there an event?
    virtual bool event() const
        { return ( simcontext()->delta_count() == m_delta + 1 ); }


    // write the new value
    virtual void write( const sc_logic& );


    // delayed evaluation
    virtual const sc_signal_logic_deval& delayed() const;


    // other methods

    operator const sc_logic& () const
	{ return read(); }


    sc_signal<sc_logic>& operator = ( const sc_logic& a )
	{ write( a ); return *this; }

    sc_signal<sc_logic>& operator = ( const sc_signal<sc_logic>& a )
	{ write( a.read() ); return *this; }


    const sc_logic& get_new_value() const
	{ return m_new_val; }

    const sc_logic& get_old_value() const
	{ return m_old_val; }


    void trace( sc_trace_file* tf ) const
	{ ::sc_trace( tf, get_data_ref(), name() ); }


#ifdef DEBUG_SYSTEMC
    virtual void print( ostream& ) const;
    virtual void dump( ostream& ) const;
#endif


    static const char* const kind_string;

    virtual const char* kind() const
        { return kind_string; }

protected:

    virtual void update();

protected:

    sc_port_base* m_output; // used for static design rule checking

    sc_logic      m_cur_val;
    sc_logic      m_new_val;
    sc_logic      m_old_val;

    sc_event      m_value_changed_event;

    uint64        m_delta; // delta of last event

private:

    // disabled
    sc_signal( const sc_signal<sc_logic>& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

inline
void
sc_signal<sc_logic>::register_port( sc_port_base& port_,
				    const char* if_typename_ )
{
#ifdef DEBUG_SYSTEMC
    sc_string nm( if_typename_ );
    if( nm == typeid( sc_signal_inout_if<sc_logic> ).name() ) {
	// an out or inout port; only one can be connected
	if( m_output != 0 ) {
	    char msg[BUFSIZ];
	    sprintf( msg, "\n signal `%s'"
                          "\n first driver `%s'"
		          "\n second driver `%s'",
		     name(), m_output->name(), port_.name() );
	    REPORT_ERROR( 7041, msg );
	}
	m_output = &port_;
    }
#endif
}


// write the new value

inline
void
sc_signal<sc_logic>::write( const sc_logic& value_ )
{
    m_new_val = value_;
    if( !( m_new_val == m_cur_val ) ) {
	request_update();
    }
}


// delayed evaluation

inline
const sc_signal_logic_deval&
sc_signal<sc_logic>::delayed() const
{
    const sc_signal_in_if<sc_logic>* iface = this;
    return RCAST<const sc_signal_logic_deval&>( *iface );
}


#ifdef DEBUG_SYSTEMC

inline
void
sc_signal<sc_logic>::print( ostream& os ) const
{
    os << m_cur_val;
}

inline
void
sc_signal<sc_logic>::dump( ostream& os ) const
{
    os << "     name = " << name() << endl;
    os << "    value = " << m_cur_val << endl;
    os << "new value = " << m_new_val << endl;
    os << "old value = " << m_old_val << endl;
}

#endif


inline
void
sc_signal<sc_logic>::update()
{
    if( !( m_new_val == m_cur_val ) ) {
	m_old_val = m_cur_val;
	m_cur_val = m_new_val;
	m_value_changed_event.notify_delayed();
	m_delta = simcontext()->delta_count();
    }
}


// ----------------------------------------------------------------------------

template <class T>
inline
ostream&
operator << ( ostream& os, const sc_signal<T>& a )
{
    return ( os << a.read() );
}


#endif

// Taf!
