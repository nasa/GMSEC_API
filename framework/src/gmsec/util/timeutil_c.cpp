
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file timeutil_c.cpp
 *
 *  @brief This file contains the C-wrapper for time util functions.
 *
 */

#include <gmsec/util/timeutil_c.h>
#include <gmsec/util/timeutil.h>


using namespace gmsec::util;

void CALL_TYPE gmsec_FormatTime_s(double time_s, char *buffer)
{
	formatTime_s(time_s, buffer);
}

