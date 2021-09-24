/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

import gov.nasa.gsfc.gmsecapi.jni.JNIConfig;

/** @class Config
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief Encapsulates the parameters necessary for initializing connections.  Used
 * by the ConnectionFactory, this class provides a generic means of supplying
 * initialization data without being Connection specific.
 *
 * Example of Config creation & use:
 * @code
 * Status res = new Status();
 * Config cfg(argc,argv);	// create from main parameters
 *
 * GMSEC_String name = new GMSEC_String();
 * GMSEC_String value = new GMSEC_String();
 *
 * res = cfg.GetFirst(name,value);	// get first value
 * while(!res.isError()) {
 *	System.out.println(name + "," + value);
 *	res = cfg.GetNext(name,value);	// get next value
 * }
 * @endcode
 *
 * @sa Connection @n
 *	   Message
 */
@Deprecated
public class Config
{
	private JNIConfig fConfig = null;


	public static JNIConfig getInternal(Config cfg)
	{
		return (cfg == null ? null : cfg.fConfig);
	}


	/**
	 * @brief Create an empty Config
	 */
	public Config()
	{
		fConfig = new JNIConfig();
	}


	/**
	 * @brief This constructor will parse command line parameters and add
	 * values for any param that is of the form \<name\>=\<value\>
	 * 
	 * Example
	 * @code gmsub.exe connectionType=ICSSWB hostname=localhost port=10005 @endcode
	 *
	 * @param argv - an array of string parameters (from main)
	 */
	public Config(String argv[])
	{
		fConfig = new JNIConfig(argv);
	}


	/**
	 * @brief Copy constructor
	 */
	public Config(Config cfg)
	{
		if (cfg != null)
		{
			fConfig = new JNIConfig(cfg.fConfig);
		}
	}


	/**
	 * @brief This constructor will create a config and parse values from the passed in xml string.
	 * The xml format is the same as used by the FromXML() and ToXML() functions.
	 *
	 * @param xml - string in xml format (see FromXML)
	 *
	 * @sa FromXML() @n ToXML()
	 */
	public Config(String xml)
	{
		fConfig = new JNIConfig(xml);
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
        if (name == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Name is null");
        }
        if (value == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Value is null");
        }
		return fConfig.AddValue(name, value);
	}


	/**
	 * @brief Remove a particulare parameter by name
	 * @param name - name of parameter to be removed
	 * @return Status - the operational result
	 */
	public Status ClearValue(String name)
	{
        if (name == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Name is null");
        }
		return fConfig.ClearValue(name);
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
        if (name == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Name is null");
        }
        if (value == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Value is null");
        }
		return fConfig.GetValue(name, value);
	}


	/**
	 * @brief Clear all entries from the container
	 * @return Status - the operational result
	 */
	public Status Clear()
	{
		return fConfig.Clear();
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
        if (name == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Name is null");
        }
        if (value == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Value is null");
        }
		return fConfig.GetFirst(name, value);
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
        if (name == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Name is null");
        }
        if (value == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "Value is null");
        }
		return fConfig.GetNext(name, value);
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
        if (xml == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "XML output string is null");
        }
		return fConfig.ToXML(xml);
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
        if (xml == null)
        {
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "XML input string is null");
        }
		return fConfig.FromXML(xml);
	}


	/**
	 * @brief This function will read a config from XML DOM Node object.
	 *
	 * @note This function has been deprecated.
	 *
	 * @sa FromXML()
	 */
	public Status FromXML(Node node)
	{
		if (node == null)
		{
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "XML Node is null");
		}

		NodeList nl = node.getChildNodes();

		if (nl == null)
		{
            return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_OTHER_ERROR, "XML Node has no child nodes");
		}

		Status result = new Status();

		for (int i = 0; (i < nl.getLength()) && (result.IsError() == false); i++)
		{
			// get field node
			Node fNode = nl.item(i);

			if (fNode != null && fNode.getNodeName().equals("PARAMETER"))
			{
				// get attributes
				NamedNodeMap nm = fNode.getAttributes();
				if (nm != null)
				{
					Node nameNode = nm.getNamedItem("NAME");
					Node valNode  = fNode.getFirstChild();

					if (nameNode != null && valNode != null)
					{
						String name  = nameNode.getNodeValue();
						String value = valNode.getNodeValue();

						// add to config
						result = AddValue(name, value);
					}
				}
			}
		}

		return result;
	}
}
