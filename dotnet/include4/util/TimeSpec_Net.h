/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_TIME_SPEC_NET_H
#define GMSEC_API_TIME_SPEC_NET_H



namespace GMSEC
{
namespace API
{
namespace UTIL
{


/// <summary>
/// GMSEC Time Specification definition
/// </summary>


public ref class TimeSpec
{
public:
	/// <summary>Constructor</summmary>
	TimeSpec();


	/// <summary>Destructor</summmary>
	~TimeSpec();


	long seconds;
	long nanoseconds;


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!TimeSpec();
};


} // end namespace UTIL
} // end namespace API
} // end namespace GMSEC


#endif
