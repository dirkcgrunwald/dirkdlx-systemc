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

  sc_lambda_defs.h_ - Macros for declaration of various forms of lambda
                      creation.

  Original Author: Stan Y. Liao, Synopsys, Inc.

 *****************************************************************************/

/*****************************************************************************

  MODIFICATION LOG - modifiers, enter your name, affiliation, date and
  changes you are making here.

      Name, Affiliation, Date:
  Description of Modification:
    
 *****************************************************************************/

#ifndef SC_LAMBDA_DEFS_H
#define SC_LAMBDA_DEFS_H


#define L_DECL1(op, rator, ty1, exp1)                                         \
inline                                                                        \
sc_lambda_ptr                                                                 \
operator op( ty1 s1 )                                                         \
{                                                                             \
    return new sc_lambda( rator,                                              \
                          new sc_lambda_rand( exp1 ) );                       \
}

#define L_DECL1SPEC(op, ty1, code)                                            \
inline                                                                        \
sc_lambda_ptr                                                                 \
operator op( ty1 s1 )                                                         \
{                                                                             \
    code                                                                      \
}

#define L_DECL2(op, rator, ty1, ty2, exp1, exp2)                              \
inline                                                                        \
sc_lambda_ptr                                                                 \
operator op( ty1 s1, ty2 s2 )                                                 \
{                                                                             \
    return new sc_lambda( rator,                                              \
                          new sc_lambda_rand( exp1 ),                         \
                          new sc_lambda_rand( exp2 ) );                       \
}

#define L_DECL2SPEC(op, ty1, ty2, code)                                       \
inline                                                                        \
sc_lambda_ptr                                                                 \
operator op( ty1 s1, ty2 s2 )                                                 \
{                                                                             \
    code                                                                      \
}


#include "systemc/kernel/sc_lambda_exps.h"


#undef L_DECL1
#undef L_DECL1SPEC
#undef L_DECL2
#undef L_DECL2SPEC


#endif
