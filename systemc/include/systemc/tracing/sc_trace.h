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

  sc_trace.h - Functions for tracing signals and variables.

  Author: Abhijit Ghosh, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:

 *****************************************************************************/

/*****************************************************************************

   Acknowledgement: The tracing mechanism is based on the tracing
   mechanism developed at Infineon (formerly Siemens HL). Though this
   code is somewhat different, the basics are identical to what was
   originally contributed by Infineon.  The contribution of Infineon
   in the development of this tracing technology is hereby
   acknowledged.

 *****************************************************************************/

#ifndef SC_TRACE_H
#define SC_TRACE_H

#include <stdio.h>

#include "systemc/utils/sc_string.h"

// Some forward declarations
class sc_logic;
class sc_signed;
class sc_unsigned;
class sc_int_base;
class sc_uint_base;
class sc_bit;
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
class sc_logic_resolve;
class sc_time;

template <class T> class sc_signal_in_if;
class sc_signal_resolved;

// Base class for all kinds of trace files. 

class sc_trace_file
{
    friend class sc_simcontext;
    
public:

    // Constructor
    sc_trace_file();
    
    // All functions are pure virtual because they need to be defined by the
    // particular tracing mechanism


#define DECL_TRACE_METHOD_A(tp)                                               \
    virtual void trace( const tp& object,                                     \
			const sc_string& name ) = 0;

#define DECL_TRACE_METHOD_B(tp)                                               \
    virtual void trace( const tp& object,                                     \
			const sc_string& name,                                \
			int width ) = 0;


    DECL_TRACE_METHOD_A( bool )
    DECL_TRACE_METHOD_A( sc_bit )
    DECL_TRACE_METHOD_A( sc_logic )

    DECL_TRACE_METHOD_B( unsigned char )
    DECL_TRACE_METHOD_B( unsigned short )
    DECL_TRACE_METHOD_B( unsigned int )
    DECL_TRACE_METHOD_B( unsigned long )
    DECL_TRACE_METHOD_B( char )
    DECL_TRACE_METHOD_B( short )
    DECL_TRACE_METHOD_B( int )
    DECL_TRACE_METHOD_B( long )

    DECL_TRACE_METHOD_A( float )
    DECL_TRACE_METHOD_A( double )
    DECL_TRACE_METHOD_A( sc_int_base )
    DECL_TRACE_METHOD_A( sc_uint_base )
    DECL_TRACE_METHOD_A( sc_signed )
    DECL_TRACE_METHOD_A( sc_unsigned )

#ifdef SC_INCLUDE_FX
    DECL_TRACE_METHOD_A( sc_fxval )
    DECL_TRACE_METHOD_A( sc_fxval_fast )
    DECL_TRACE_METHOD_A( sc_fxnum )
    DECL_TRACE_METHOD_A( sc_fxnum_fast )
#endif

    DECL_TRACE_METHOD_A( sc_signal_resolved )
    DECL_TRACE_METHOD_A( sc_bv_base )
    DECL_TRACE_METHOD_A( sc_lv_base )


#undef DECL_TRACE_METHOD_A
#undef DECL_TRACE_METHOD_B

    // Trace an enumerated object - where possible output the enumeration
    // literals in the trace file. Enum literals is a null terminated array
    // of null terminated char* literal strings.
    virtual void trace( const unsigned int& object,
			const sc_string& name,
			const char** enum_literals ) = 0;

    // Output a comment to the trace file
    virtual void write_comment( const sc_string& comment ) = 0;

    // Set the amount of space before next column
    // (For most formats this does nothing)
    virtual void space( int n );

    // Also trace transitions between delta cycles if flag is true.
    virtual void delta_cycles( bool flag );

protected:

    // Write trace info for cycle
    virtual void cycle( bool delta_cycle ) = 0;

    // Flush results and close file
    virtual ~sc_trace_file()
	{ /* Intentionally blank */ };
};

/*****************************************************************************/

// Now comes all the SystemC defined tracing functions.
// We define two sc_trace() versions for scalar types; one where the object to
// be traced is passed as a reference and the other where a pointer to the
// tracing object is passed.

#define DECL_TRACE_FUNC_REF_A(tp)                                             \
void                                                                          \
sc_trace( sc_trace_file* tf,                                                  \
	  const tp& object,                                                   \
	  const sc_string& name );

#define DECL_TRACE_FUNC_PTR_A(tp)                                             \
void                                                                          \
sc_trace( sc_trace_file* tf,                                                  \
	  const tp* object,                                                   \
	  const sc_string& name );                                            \

#define DECL_TRACE_FUNC_A(tp)                                                 \
DECL_TRACE_FUNC_REF_A(tp)                                                     \
DECL_TRACE_FUNC_PTR_A(tp)


DECL_TRACE_FUNC_A( sc_bit )
DECL_TRACE_FUNC_A( sc_logic )

DECL_TRACE_FUNC_A( sc_int_base )
DECL_TRACE_FUNC_A( sc_uint_base )
DECL_TRACE_FUNC_A( sc_signed )
DECL_TRACE_FUNC_A( sc_unsigned )

DECL_TRACE_FUNC_REF_A( sc_bv_base )
DECL_TRACE_FUNC_REF_A( sc_lv_base )

DECL_TRACE_FUNC_REF_A( sc_signal_resolved )


#undef DECL_TRACE_FUNC_REF_A
#undef DECL_TRACE_FUNC_PTR_A
#undef DECL_TRACE_FUNC_A


// ----------------------------------------------------------------------------

#define DEFN_TRACE_FUNC_REF_A(tp)                                             \
inline                                                                        \
void                                                                          \
sc_trace( sc_trace_file* tf, const tp& object, const sc_string& name )        \
{                                                                             \
    if( tf ) {                                                                \
	tf->trace( object, name );                                            \
    }                                                                         \
}

#define DEFN_TRACE_FUNC_PTR_A(tp)                                             \
inline                                                                        \
void                                                                          \
sc_trace( sc_trace_file* tf, const tp* object, const sc_string& name )        \
{                                                                             \
    if( tf ) {                                                                \
	tf->trace( *object, name );                                           \
    }                                                                         \
}

#define DEFN_TRACE_FUNC_A(tp)                                                 \
DEFN_TRACE_FUNC_REF_A(tp)                                                     \
DEFN_TRACE_FUNC_PTR_A(tp)


#define DEFN_TRACE_FUNC_REF_B(tp)                                             \
inline                                                                        \
void                                                                          \
sc_trace( sc_trace_file* tf, const tp& object, const sc_string& name,         \
          int width = 8 * sizeof( tp ) )                                      \
{                                                                             \
    if( tf ) {                                                                \
	tf->trace( object, name, width );                                     \
    }                                                                         \
}

#define DEFN_TRACE_FUNC_PTR_B(tp)                                             \
inline                                                                        \
void                                                                          \
sc_trace( sc_trace_file* tf, const tp* object, const sc_string& name,         \
          int width = 8 * sizeof( tp ) )                                      \
{                                                                             \
    if( tf ) {                                                                \
	tf->trace( *object, name, width );                                    \
    }                                                                         \
}


#define DEFN_TRACE_FUNC_B(tp)                                                 \
DEFN_TRACE_FUNC_REF_B(tp)                                                     \
DEFN_TRACE_FUNC_PTR_B(tp)


DEFN_TRACE_FUNC_A( bool )
DEFN_TRACE_FUNC_A( float )
DEFN_TRACE_FUNC_A( double )

DEFN_TRACE_FUNC_B( unsigned char )
DEFN_TRACE_FUNC_B( unsigned short )
DEFN_TRACE_FUNC_B( unsigned int )
DEFN_TRACE_FUNC_B( unsigned long )
DEFN_TRACE_FUNC_B( char )
DEFN_TRACE_FUNC_B( short )
DEFN_TRACE_FUNC_B( int )
DEFN_TRACE_FUNC_B( long )


#undef DEFN_TRACE_FUNC_REF_A
#undef DEFN_TRACE_FUNC_PTR_A
#undef DEFN_TRACE_FUNC_A

#undef DEFN_TRACE_FUNC_REF_B
#undef DEFN_TRACE_FUNC_PTR_B
#undef DEFN_TRACE_FUNC_B


template <class T> 
inline
void
sc_trace( sc_trace_file* tf,
	  const sc_signal_in_if<T>& object,
	  const sc_string& name )
{
    sc_trace( tf, object.get_data_ref(), name );
}

template< class T >
inline
void
sc_trace( sc_trace_file* tf,
	  const sc_signal_in_if<T>& object,
	  const char* name )
{
    sc_trace( tf, object.get_data_ref(), name );
}


// specializations for signals of type char, short, int, long

void sc_trace( sc_trace_file* tf,
	       const sc_signal_in_if<char>& object,
	       const sc_string& name,
	       int width );

void sc_trace( sc_trace_file* tf,
	       const sc_signal_in_if<short>& object,
	       const sc_string& name,
	       int width );

void sc_trace( sc_trace_file* tf,
	       const sc_signal_in_if<int>& object,
	       const sc_string& name,
	       int width );

void sc_trace( sc_trace_file* tf,
	       const sc_signal_in_if<long>& object,
	       const sc_string& name,
	       int width );


// 1. non-template function is better than template
// 2. more-specialized template is better than less-specialized
// 3. no partial specialization for template functions


// Trace an enumerated object - where possible output the enumeration literals
// in the trace file. Enum literals is a null terminated array of null
// terminated char* literal strings.

void
sc_trace( sc_trace_file* tf,
	  const unsigned int& object,
	  const sc_string& name,
	  const char** enum_literals );


#if defined( __BCPLUSPLUS__ )
#pragma warn -8027
// inline will not be effective for functions with loops
#endif


// // Functions for tracing channels
// 
// template <class T>
// inline
// void
// sc_trace( sc_trace_file* tf,
// 	  const sc_channel<T>& object,
// 	  const sc_string& name,
// 	  int nbuf = 1 )
// {
//     char strbuf[100];
//     for (int i = 0; i < object.bufsize(); i++) {
// 	if (i >= nbuf) break;
// 	sprintf(strbuf, "_%d", i);
// 	sc_trace(tf, (T&) *(object.get_data_ptr(i)), name + strbuf);
//     }
// }
// 
// 
// // For the same nasty reason as for signals, we need the following for
// // channels too.
// 
// template <class T>
// inline
// void sc_trace( sc_trace_file* tf,
// 	       const sc_channel<T>& object,
// 	       const char* name,
// 	       int nbuf = 1 )
// {
//     char strbuf[100];
//     sc_string namestr = name;
//     for (int i = 0; i < object.bufsize(); i++) {
// 	if (i >= nbuf) break;
// 	sprintf(strbuf, "_%d", i);
// 	sc_trace(tf, (T&) *(object.get_data_ptr(i)), namestr + strbuf);
//     }
// }


#if defined(__BCPLUSPLUS__)
#pragma warn .8027
#endif


// Dummy function for arbitrary types of value, does nothing

extern void sc_trace( sc_trace_file* tf,
		      const void* object,
		      const sc_string& name );


// Turn on/off delta cycle tracing on trace file `tf'.
// Default is to turn on delta cycle tracing.

inline
void
sc_trace_delta_cycles( sc_trace_file* tf, bool on = true )
{
    if( tf ) tf->delta_cycles( on );
}


// Output a comment to the trace file

inline
void
sc_write_comment( sc_trace_file* tf, const sc_string& comment )
{
    if( tf ) tf->write_comment( comment );
}


// Equivalent of fprintf for trace files!

#ifdef __GNUC__
void tprintf( sc_trace_file* tf,  const char* format, ... )
    __attribute__ ((format (printf,2,3)));
#else
void tprintf( sc_trace_file* tf,  const char* format, ... );
#endif    


// Convert double time to 64-bit integer

extern void double_to_special_int64( double in,
				     unsigned* high,
				     unsigned* low );


#endif
