/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi.jni;

import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Status;
import gov.nasa.gsfc.gmsecapi.GMSEC_String;

public class JNIConfig
{
	private long swigCPtr;
	protected boolean swigCMemOwn;

	protected JNIConfig(long cPtr, boolean cMemoryOwn)
	{
		swigCPtr    = cPtr;
		swigCMemOwn = cMemoryOwn;
	}


	protected void finalize() throws Throwable
	{
		try {
			delete();
		}
		finally {
			super.finalize();
		}
	}


	public synchronized void delete()
	{
		if (swigCPtr != 0 && swigCMemOwn)
		{
			gmsecJNI.delete_Config(swigCPtr, this);
			swigCMemOwn = false;
		}
		swigCPtr = 0;
	}


	protected static long getCPtr(JNIConfig obj)
	{
		return (obj == null) ? 0 : obj.swigCPtr;
	}


	/**
	 * @brief Create an empty Config
	 */
	public JNIConfig()
	{
		this(gmsecJNI.new_Config(), true);
	}


	/**
	 * @brief This constructor will parse command line parameters and add
	 * values for any param that is of the form <name>=<value>
	 * 
	 * Example
	 * @code gmsub.exe connectionType=ICSSWB hostname=localhost port=10005 @endcode
	 *
	 * @param argv - an array of string parameters (from main)
	 */
	public JNIConfig(String argv[])
	{
		this(gmsecJNI.new_Config_String(argv), true);
	}


	/**
	 * @brief Copy constructor
	 * @param Config object
	 */
	public JNIConfig(Config cfg)
	{
		this(gmsecJNI.new_Config_Copy(JNIConfig.getCPtr(Config.getInternal(cfg)), Config.getInternal(cfg)), true);
	}


	/**
	 * @brief Copy constructor
	 * @param JNIConfig object
	 */
	public JNIConfig(JNIConfig cfg)
	{
		this(gmsecJNI.new_Config_Copy(JNIConfig.getCPtr(cfg), cfg), true);
	}


	/**
	 * @brief This constructor will create a config and parse values from the passed in xml string.
	 * The xml format is the same as used by the FromXML() and ToXML() functions.
	 *
	 * @param xml - string in xml format (see FromXML)
	 *
	 * @sa FromXML() @n ToXML()
	 */
	public JNIConfig(String xml)
	{
		this(gmsecJNI.new_Config(), true);
		FromXML(xml);
	}


	public void Set(JNIConfig cfg)
	{
		GMSEC_String name = new GMSEC_String();
		GMSEC_String value = new GMSEC_String();

		Status res = new Status();

		res = cfg.GetFirst(name, value); // get first value
		while (!res.isError())
		{
			AddValue(name.Get(), value.Get());
			res = cfg.GetNext(name, value); // get next value
		}
	}


	/**
	 * @brief Store a name / value pair
	 * @param name - the name associated with the value.  Each middleware will have a set
	 *               of constants defined for this value
	 * @param value - the value associated with the name
	 * @return Status - The operational result
	 */
	public Status AddValue(String name, String value)
	{
		long cPtr = gmsecJNI.Config_AddValue(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief Remove a particular parameter by name
	 * @param name - name of parameter to be removed
	 * @return Status - the operational result
	 */
	public Status ClearValue(String name)
	{
		long cPtr = gmsecJNI.Config_ClearValue(swigCPtr, this, name);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief Retrieve the value for a parameter specified by the parameter name
	 * @param name - name of parameter to retrieve
	 * @param value - return value associated with the name.  This value may be null
	 *                if the parameter does not exist
	 * @return Status - the operational result
	 */
	public Status GetValue(String name, GMSEC_String value)
	{
		long cPtr = gmsecJNI.Config_GetValue(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief Clear all entries from the container
	 * @return Status - the operational result
	 */
	public Status Clear()
	{
		long cPtr = gmsecJNI.Config_Clear(swigCPtr, this);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief This function gets the first name & value for iteration. The scope
	 * of the name and value are the life of the Config object, or
	 * until the next %GetFirst() call.
	 * 
	 * @param name - return value of the first parameter name
	 * @param value - return value of the first parameter value
	 * @return Status - the operational result
	 */
	public Status GetFirst(GMSEC_String name, GMSEC_String value)
	{
		long cPtr = gmsecJNI.Config_GetFirst(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief This function gets the next name & value for iteration. The scope
	 * of the name and value are the life of the Config object, or
	 * until the next %GetNext() call.
	 *
	 * Example
	 * @code
	 * res = cfg.GetFirst(name,value);	// get first value
	 * while(!res.isError()) {
	 *	System.out.println(name + "," + value);
	 *	res = cfg.GetNext(name,value);	// get next value
	 * }
	 * @endcode
	 *
	 * @param name - return name parameter of the next field
	 * @param value - return value parameter of the next field
	 * @return Status - the operational result
	 */
	public Status GetNext(GMSEC_String name, GMSEC_String value)
	{
		long cPtr = gmsecJNI.Config_GetNext(swigCPtr, this, name, value);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief This function will dump this config to XML.
	 * The format is the same as that which is parsed by FromXML().
	 * 
	 * @param xml - output parameter: xml string
	 * @return status object with the result of the operation
	 * 
	 * @sa FromXML()
	 */
	public Status ToXML(GMSEC_String xml)
	{
		long cPtr = gmsecJNI.Config_ToXML(swigCPtr, this, xml);
		return new JNIStatus(cPtr, true);
	}


	/**
	 * @brief This function will read a config from XML. 
	 * The format is the same as that which is generated by ToXML().
	 * 
	 * Example xml string:
	 * @code
	 * <CONFIG>
	 *    <PARAMETER NAME="connectiontype">gmsec_ss</PARAMETER>
	 *    <PARAMETER NAME="server">tcp:10.1.2.159</PARAMETER>
	 *    <PARAMETER NAME="tracking">true</PARAMETER>
	 *    <PARAMETER NAME="isthreaded">true</PARAMETER>
	 * </CONFIG>
	 * @endcode
	 * 
	 * @sa ToXML()
	 */
	public Status FromXML(String xml)
	{
		long cPtr = gmsecJNI.Config_FromXML(swigCPtr, this, xml);
		return new JNIStatus(cPtr, true);
	}
}
