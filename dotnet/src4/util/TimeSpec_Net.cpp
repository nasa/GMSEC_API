/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <util/TimeSpec_Net.h>


using namespace GMSEC::API::UTIL;


TimeSpec::TimeSpec()
{
	// nothing to do
}


TimeSpec::~TimeSpec()
{
	this->!TimeSpec();
}


TimeSpec::!TimeSpec()
{
	// nothing to do
}
