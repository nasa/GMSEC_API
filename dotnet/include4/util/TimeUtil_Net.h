/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_TIME_UTIL_NET_H
#define GMSEC_API_TIME_UTIL_NET_H


namespace GMSEC
{
namespace API
{
namespace UTIL
{

// Forward declaration
ref class TimeSpec;


/// <summary>
/// Utility methods for converting time from one format to another.
/// </summary>


public ref class TimeUtil
{
public:
	/// <summary>Return the current time in seconds since Jan 1 1970 00:00:00 UTC.</summary>
	static TimeSpec^ GetCurrentTime();


	/// <summary>Converts the given time to a GMSEC style time-string.</summary>
	///
	/// <param name="spec">The time, as a TimeSpec.</param>
	///
	/// <returns>A string with the format of YYYY-DDD-HH-MM-SS.sss</returns>
	///
	/// <exception cref="GMSEC_Exception">Exception is thrown if the TimeSpec object is null.</exception>
	static System::String^ FormatTime(TimeSpec^ spec);


	/// <summary>Converts the given time to a GMSEC style time-string.</summary>
	///
	/// <param name="spec">The time, as a TimeSpec.</param>
	/// <param name="subseconds">The number of digits to allow for subseconds [0..9].</param>
	///
	/// <returns>A string with the format of YYYY-DDD-HH-MM-SS.<sss...ss></returns>
	///
	/// <exception cref="GMSEC_Exception">Exception is thrown if the TimeSpec object is null.</exception>
	/// <exception cref="GMSEC_Exception">Exception is thrown if the subseconds are outside the range of [0..9].</exception>
	static System::String^ FormatTime(TimeSpec^ spec, int subseconds);
};


} // end namespace UTIL
} // end namespace API
} // end namespace GMSEC


#endif
