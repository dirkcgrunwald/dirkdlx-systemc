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

  sc_signal_ports.h -- The sc_signal<T> port classes.

  Original Author: Martin Janssen, Synopsys, Inc., 2001-05-21

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_SIGNAL_PORTS_H
#define SC_SIGNAL_PORTS_H


#include "systemc/communication/sc_event_finder.h"
#include "systemc/communication/sc_port.h"
#include "systemc/communication/sc_signal_ifs.h"
#include "systemc/datatypes/bit/sc_logic.h"


// ----------------------------------------------------------------------------
//  CLASS : sc_in<T>
//
//  The sc_signal<T> input port class.
// ----------------------------------------------------------------------------

template <class T>
class sc_in
: public sc_port<sc_signal_in_if<T>,1>
{
public:

    // typedefs

    typedef T                             data_type;

    typedef sc_signal_in_if<data_type>    if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_in<data_type>              this_type;

    typedef if_type                       in_if_type;
    typedef base_type                     in_port_type;
    typedef sc_signal_inout_if<data_type> inout_if_type;
    typedef sc_port<inout_if_type,1>      inout_port_type;

public:

    // constructors

    sc_in()
	: base_type()
	{}

    explicit sc_in( const in_if_type& interface_ )
        : base_type( CCAST<in_if_type&>( interface_ ) )
        {}

    explicit sc_in( in_port_type& parent_ )
	: base_type( parent_ )
	{}

    explicit sc_in( inout_port_type& parent_ )
	: base_type()
	{ sc_port_base::bind( parent_ ); }

    sc_in( this_type& parent_ )
	: base_type( parent_ )
	{}


    // destructor (does nothing)

    virtual ~sc_in()
	{}


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:

    // disabled
    sc_in( const this_type& );
    this_type& operator = ( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// called by pbind (for internal use only)

template <class T>
inline
int
sc_in<T>::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

template <class T>
inline
int
sc_in<T>::vbind( sc_port_base& parent_ )
{
    in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_in<bool>
//
//  Specialization of sc_in<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class sc_in<bool>
: public sc_port<sc_signal_in_if<bool>,1>
{
public:

    // typedefs

    typedef bool                          data_type;

    typedef sc_signal_in_if<data_type>    if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_in<data_type>              this_type;

    typedef if_type                       in_if_type;
    typedef base_type                     in_port_type;
    typedef sc_signal_inout_if<data_type> inout_if_type;
    typedef sc_port<inout_if_type,1>      inout_port_type;

public:

    // constructors

    sc_in()
	: base_type()
	{}

    explicit sc_in( const in_if_type& interface_ )
	: base_type( CCAST<in_if_type&>( interface_ ) )
	{}

    explicit sc_in( in_port_type& parent_ )
	: base_type( parent_ )
	{}

    explicit sc_in( inout_port_type& parent_ )
	: base_type()
	{ sc_port_base::bind( parent_ ); }

    sc_in( this_type& parent_ )
	: base_type( parent_ )
	{}


    // destructor (does nothing)

    virtual ~sc_in()
	{}


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // use for positive edge sensitivity

    sc_event_finder& pos() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::posedge_event );
    }

    // use for negative edge sensitivity

    sc_event_finder& neg() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::negedge_event );
    }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }

    // was there a positive edge event?

    bool posedge() const
        { return (*this)->posedge(); }

    // was there a negative edge event?

    bool negedge() const
        { return (*this)->negedge(); }


    // delayed evaluation
    const sc_signal_bool_deval& delayed() const;


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:

    // disabled
    sc_in( const this_type& );
    this_type& operator = ( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_bool_deval&
sc_in<bool>::delayed() const
{
    const in_if_type* iface = DCAST<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return RCAST<const sc_signal_bool_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return RCAST<const sc_signal_bool_deval&>( *pb );
    }
}


// called by pbind (for internal use only)

inline
int
sc_in<bool>::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

inline
int
sc_in<bool>::vbind( sc_port_base& parent_ )
{
    in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_in<sc_logic>
//
//  Specialization of sc_in<T> for type sc_logic.
// ----------------------------------------------------------------------------

template <>
class sc_in<sc_logic>
: public sc_port<sc_signal_in_if<sc_logic>,1>
{
public:

    // typedefs

    typedef sc_logic                      data_type;

    typedef sc_signal_in_if<data_type>    if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_in<data_type>              this_type;

    typedef if_type                       in_if_type;
    typedef base_type                     in_port_type;
    typedef sc_signal_inout_if<data_type> inout_if_type;
    typedef sc_port<inout_if_type,1>      inout_port_type;

public:

    // constructors

    sc_in()
	: base_type()
	{}

    explicit sc_in( const in_if_type& interface_ )
	: base_type( CCAST<in_if_type&>( interface_ ) )
	{}

    explicit sc_in( in_port_type& parent_ )
	: base_type( parent_ )
	{}

    explicit sc_in( inout_port_type& parent_ )
	: base_type()
	{ sc_port_base::bind( parent_ ); }

    sc_in( this_type& parent_ )
	: base_type( parent_ )
	{}


    // destructor (does nothing)

    virtual ~sc_in()
	{}


    // bind to in interface

    void bind( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }

    void operator () ( const in_if_type& interface_ )
	{ sc_port_base::bind( CCAST<in_if_type&>( interface_ ) ); }


    // bind to parent in port

    void bind( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }

    void operator () ( in_port_type& parent_ )
        { sc_port_base::bind( parent_ ); }


    // bind to parent inout port

    void bind( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }

    void operator () ( inout_port_type& parent_ )
	{ sc_port_base::bind( parent_ ); }


    // interface access shortcut methods

    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // delayed evaluation
    const sc_signal_logic_deval& delayed() const;


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }

protected:

    // called by pbind (for internal use only)
    virtual int vbind( sc_interface& );
    virtual int vbind( sc_port_base& );

private:

    // disabled
    sc_in( const this_type& );
    this_type& operator = ( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_logic_deval&
sc_in<sc_logic>::delayed() const
{
    const in_if_type* iface = DCAST<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return RCAST<const sc_signal_logic_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return RCAST<const sc_signal_logic_deval&>( *pb );
    }
}


// called by pbind (for internal use only)

inline
int
sc_in<sc_logic>::vbind( sc_interface& interface_ )
{
    return sc_port_b<if_type>::vbind( interface_ );
}

inline
int
sc_in<sc_logic>::vbind( sc_port_base& parent_ )
{
    in_port_type* in_parent = DCAST<in_port_type*>( &parent_ );
    if( in_parent != 0 ) {
	sc_port_base::bind( *in_parent );
	return 0;
    }
    inout_port_type* inout_parent = DCAST<inout_port_type*>( &parent_ );
    if( inout_parent != 0 ) {
	sc_port_base::bind( *inout_parent );
	return 0;
    }
    // type mismatch
    return 2;
}


// ----------------------------------------------------------------------------
//  CLASS : sc_inout<T>
//
//  The sc_signal<T> input/output port class.
// ----------------------------------------------------------------------------

template <class T>
class sc_inout
: public sc_port<sc_signal_inout_if<T>,1>
{
public:

    // typedefs

    typedef T                             data_type;

    typedef sc_signal_inout_if<data_type> if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_inout<data_type>           this_type;

    typedef sc_signal_in_if<data_type>    in_if_type;
    typedef sc_port<in_if_type,1>         in_port_type;
    typedef if_type                       inout_if_type;
    typedef base_type                     inout_port_type;

public:

    // constructors

    sc_inout()
	: base_type(), m_init_val( 0 )
	{}

    explicit sc_inout( inout_if_type& interface_ )
	: base_type( interface_ ), m_init_val( 0 )
	{}

    explicit sc_inout( inout_port_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 )
	{}

    sc_inout( this_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 )
	{}


    // destructor

    virtual ~sc_inout()
	{ if( m_init_val != 0 ) { delete m_init_val; } }


    // interface access shortcut methods

    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // write the new value

    this_type& write( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

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


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }

protected:

    data_type* m_init_val;

private:

    // disabled
    sc_inout( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// set initial value (can also be called when port is not bound yet)

template <class T>
inline
void
sc_inout<T>::initialize( const data_type& value_ )
{
    inout_if_type* iface = DCAST<inout_if_type*>( this->get_interface() );
    if( iface != 0 ) {
	iface->write( value_ );
    } else {
	if( m_init_val == 0 ) {
	    m_init_val = new data_type;
	}
	*m_init_val = value_;
    }
}


// called when elaboration is done

template <class T>
inline
void
sc_inout<T>::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_inout<bool>
//
//  Specialization of sc_inout<T> for type bool.
// ----------------------------------------------------------------------------

template <>
class sc_inout<bool>
: public sc_port<sc_signal_inout_if<bool>,1>
{
public:

    // typedefs

    typedef bool                          data_type;

    typedef sc_signal_inout_if<data_type> if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_inout<data_type>           this_type;

    typedef sc_signal_in_if<data_type>    in_if_type;
    typedef sc_port<in_if_type,1>         in_port_type;
    typedef if_type                       inout_if_type;
    typedef base_type                     inout_port_type;

public:

    // constructors

    sc_inout()
	: base_type(), m_init_val( 0 )
	{}

    explicit sc_inout( inout_if_type& interface_ )
	: base_type( interface_ ), m_init_val( 0 )
	{}

    explicit sc_inout( inout_port_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 )
	{}

    sc_inout( this_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 )
	{}


    // destructor

    virtual ~sc_inout()
	{ if( m_init_val != 0 ) { delete m_init_val; } }


    // interface access shortcut methods

    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // use for positive edge sensitivity

    sc_event_finder& pos() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::posedge_event );
    }

    // use for negative edge sensitivity

    sc_event_finder& neg() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::negedge_event );
    }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }

    // was there a positive edge event?

    bool posedge() const
        { return (*this)->posedge(); }

    // was there a negative edge event?

    bool negedge() const
        { return (*this)->negedge(); }


    // delayed evaluation
    const sc_signal_bool_deval& delayed() const;


    // write the new value

    this_type& write( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

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


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }

protected:

    data_type* m_init_val;

private:

    // disabled
    sc_inout( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_bool_deval&
sc_inout<bool>::delayed() const
{
    const in_if_type* iface = DCAST<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return RCAST<const sc_signal_bool_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return RCAST<const sc_signal_bool_deval&>( *pb );
    }
}


// set initial value (can also be called when port is not bound yet)

inline
void
sc_inout<bool>::initialize( const data_type& value_ )
{
    inout_if_type* iface = DCAST<inout_if_type*>( get_interface() );
    if( iface != 0 ) {
	iface->write( value_ );
    } else {
	if( m_init_val == 0 ) {
	    m_init_val = new data_type;
	}
	*m_init_val = value_;
    }
}


// called when elaboration is done

inline
void
sc_inout<bool>::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_inout<sc_logic>
//
//  Specialization of sc_inout<T> for type sc_logic.
// ----------------------------------------------------------------------------

template <>
class sc_inout<sc_logic>
: public sc_port<sc_signal_inout_if<sc_logic>,1>
{
public:

    // typedefs

    typedef sc_logic                      data_type;

    typedef sc_signal_inout_if<data_type> if_type;
    typedef sc_port<if_type,1>            base_type;
    typedef sc_inout<data_type>           this_type;

    typedef sc_signal_in_if<data_type>    in_if_type;
    typedef sc_port<in_if_type,1>         in_port_type;
    typedef if_type                       inout_if_type;
    typedef base_type                     inout_port_type;

public:

    // constructors

    sc_inout()
	: base_type(), m_init_val( 0 )
	{}

    explicit sc_inout( inout_if_type& interface_ )
	: base_type( interface_ ), m_init_val( 0 )
	{}

    explicit sc_inout( inout_port_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 )
	{}

    sc_inout( this_type& parent_ )
	: base_type( parent_ ), m_init_val( 0 )
	{}


    // destructor

    virtual ~sc_inout()
	{ if( m_init_val != 0 ) { delete m_init_val; } }


    // interface access shortcut methods

    // read the current value

    const data_type& read() const
	{ return (*this)->read(); }

    operator const data_type& () const
	{ return (*this)->read(); }


    // was there a value changed event?

    bool event() const
	{ return (*this)->event(); }


    // delayed evaluation
    const sc_signal_logic_deval& delayed() const;


    // write the new value

    this_type& write( const data_type& value_ )
	{ (*this)->write( value_ ); return *this; }

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


    // set initial value (can also be called when port is not bound yet)

    void initialize( const data_type& value_ );

    void initialize( const in_if_type& interface_ )
	{ initialize( interface_.read() ); }


    // called when elaboration is done
    /*  WHEN DEFINING THIS METHOD IN A DERIVED CLASS, */
    /*  MAKE SURE THAT THIS METHOD IS CALLED AS WELL. */

    virtual void end_of_elaboration();


    // (other) event finder method(s)

    sc_event_finder& value_changed() const
    {
	return *new sc_event_finder_t<in_if_type>(
	    *this, &in_if_type::value_changed_event );
    }

protected:

    data_type* m_init_val;

private:

    // disabled
    sc_inout( const this_type& );
};


// IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII

// delayed evaluation

inline
const sc_signal_logic_deval&
sc_inout<sc_logic>::delayed() const
{
    const in_if_type* iface = DCAST<const in_if_type*>( get_interface() );
    if( iface != 0 ) {
	return RCAST<const sc_signal_logic_deval&>( *iface );
    } else {
	// the tricky part
	const sc_port_base* pb = this;
	return RCAST<const sc_signal_logic_deval&>( *pb );
    }
}


// set initial value (can also be called when port is not bound yet)

inline
void
sc_inout<sc_logic>::initialize( const data_type& value_ )
{
    inout_if_type* iface = DCAST<inout_if_type*>( get_interface() );
    if( iface != 0 ) {
	iface->write( value_ );
    } else {
	if( m_init_val == 0 ) {
	    m_init_val = new data_type;
	}
	*m_init_val = value_;
    }
}


// called when elaboration is done

inline
void
sc_inout<sc_logic>::end_of_elaboration()
{
    if( m_init_val != 0 ) {
	write( *m_init_val );
	delete m_init_val;
	m_init_val = 0;
    }
}


// ----------------------------------------------------------------------------
//  CLASS : sc_out<T>
//
//  The sc_signal<T> output port class.
// ----------------------------------------------------------------------------

// sc_out can also read from its port, hence no difference with sc_inout.

#define sc_out sc_inout


#endif

// Taf!
