/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/* This include file fixes the HP-UX gcc bug that makes gcc think it has
   stdint.h when it doesn't.  See
   http://gcc.gnu.org/ml/libstdc++/2003-10/msg00319.html
*/
#ifndef _GMSEC_HP_BUG
#define _GMSEC_HP_BUG

#ifdef __cplusplus
	#include <bits/c++config.h>
	#undef _GLIBCXX_HAVE_STDINT_H
#endif

#endif
