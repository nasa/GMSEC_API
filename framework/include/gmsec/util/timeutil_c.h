
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file timeutil_c.h
 *
 *  @brief This file contains the C-wrapper for time util functions.
 *
 */

#ifndef gmsec_util_timeutil_c_h
#define gmsec_util_timeutil_c_h


#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>


#ifdef __cplusplus
extern "C"
{
#endif


	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE gmsec_FormatTime_s(double time_s, char *buffer);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_util_timeutil_c_h */
