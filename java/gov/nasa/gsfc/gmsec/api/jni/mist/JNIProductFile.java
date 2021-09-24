/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


package gov.nasa.gsfc.gmsec.api.jni.mist;

import gov.nasa.gsfc.gmsec.api.*;
import gov.nasa.gsfc.gmsec.api.mist.*;

import gov.nasa.gsfc.gmsec.api.jni.gmsecJNI;


/**
 * @class JNIProductFile
 *
 * @brief This class is a lightweight container for holding information
 * on a Product File, and used to generate GMSEC Product File messages by the 
 * ConnectionManager class
 *
 * @sa ConnectionManager @n
 *		
*/
public class JNIProductFile
{
	private long      swigCPtr;
	protected boolean swigCMemOwn;


	public JNIProductFile(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;
	}


	protected void finalize() throws Throwable
	{
		delete();
		super.finalize();
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_ProductFile(swigCPtr, this);
		}

		swigCPtr    = 0;
		swigCMemOwn = false;
	}


	public static long getCPtr(JNIProductFile obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	public JNIProductFile(String name, String description, String version, String format, String uri)
	{
		this(gmsecJNI.new_ProductFile(name, description, version, format, uri), true);
	}


	public JNIProductFile(String name, String description, String version, String format, byte[] data)
	{
		this(gmsecJNI.new_ProductFile(name, description, version, format, data, data.length), true);
	}


	public JNIProductFile(JNIProductFile other)
	{
		this(gmsecJNI.new_ProductFile_Copy(JNIProductFile.getCPtr(other), other), true);
	}


	public String getName()
	{
		return gmsecJNI.ProductFile_GetName(swigCPtr, this);
	}


	public String getDescription()
	{
		return gmsecJNI.ProductFile_GetDescription(swigCPtr, this);
	}


	public String getVersion()
	{
		return gmsecJNI.ProductFile_GetVersion(swigCPtr, this);
	}


	public String getFormat()
	{
		return gmsecJNI.ProductFile_GetFormat(swigCPtr, this);
	}


	public boolean isURIAvailable()
	{
		return gmsecJNI.ProductFile_IsURIAvailable(swigCPtr, this);
	}


	public String getURI() throws GMSEC_Exception
	{
		return gmsecJNI.ProductFile_GetURI(swigCPtr, this);
	}


	public boolean contentsAvailable()
	{
		return gmsecJNI.ProductFile_ContentsAvailable(swigCPtr, this);
	}


	public byte[] getContents() throws GMSEC_Exception
	{
		return gmsecJNI.ProductFile_GetContents(swigCPtr, this);
	}
}
