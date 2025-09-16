/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api5.jni.util;

import gov.nasa.gsfc.gmsec.api5.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api5.util.TimeSpec;

import gov.nasa.gsfc.gmsec.api5.GmsecException;


public class JNITimeUtil
{
	public static void millisleep(int ms)
	{
		gmsecJNI.TimeUtil_Millisleep(ms);
	}


	public static TimeSpec getCurrentTime()
	{
		long jarg = gmsecJNI.TimeUtil_GetCurrentTime();

		if (jarg != 0)
		{
			return new JNITimeSpec(jarg, true);
		}

		return null;
	}


	public static double getCurrentTime_s()
	{
		return gmsecJNI.TimeUtil_GetCurrentTime_s();
	}


	public static TimeSpec convertTimeString(String timeString)
		throws GmsecException
	{
		return new JNITimeSpec(gmsecJNI.TimeUtil_ConvertTimeString(timeString), true);
	}


	public static String formatTime(TimeSpec spec)
	{
		return gmsecJNI.TimeUtil_FormatTime(spec.getSeconds(), spec.getNanoseconds(), 3);
	}


	public static String formatTime(TimeSpec spec, int subseconds)
	{
		return gmsecJNI.TimeUtil_FormatTime(spec.getSeconds(), spec.getNanoseconds(), subseconds);
	}
}
