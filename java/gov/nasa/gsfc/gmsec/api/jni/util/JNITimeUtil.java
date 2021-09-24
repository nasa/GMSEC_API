/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.util;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;

import gov.nasa.gsfc.gmsec.api.util.TimeSpec;


public class JNITimeUtil
{
	public static TimeSpec getCurrentTime()
	{
		long jarg = gmsecJNI.TimeUtil_GetCurrentTime();

		if (jarg != 0)
		{
			return new JNITimeSpec(jarg, true);
		}

		return null;
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
