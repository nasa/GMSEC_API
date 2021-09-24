/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.jni;

/** @class JNITimeUtil
 *
 * @brief 
 */
public class JNITimeUtil
{
	public static String FormatTime_s(double time_s)
	{
		return gmsecJNI.TimeUtil_FormatTime_s(time_s);
	}
}
