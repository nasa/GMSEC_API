/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







package gov.nasa.gsfc.gmsecapi;

import gov.nasa.gsfc.gmsecapi.Config;
import gov.nasa.gsfc.gmsecapi.Message;
import gov.nasa.gsfc.gmsecapi.jni.JNIConfigFile;


/** @class ConfigFile
 *
 * @deprecated API 3.x has been deprecated; use API 4.x instead.
 *
 * @brief This class encapsulates the load. parse, and use of standard XML configuration files.
 *
 *  Example configuration file format:
 *  @code
 * <?xml version="1.0" encoding="UTF-8"?>
 * <DEFINITIONS>
 *	<SUBSCRIPTION NAME="events" PATTERN="gmsec.mission.const.sat.evt.msg.>"/>
 *	<SUBSCRIPTION NAME="custom1" PATTERN="custom.message.subjects.*"/>
 * 	<CONFIG NAME="config1">
 * 		<PARAMETER NAME="connectiontype">gmsec_icsswb</PARAMETER>
 * 		<PARAMETER NAME="hostname">localhost</PARAMETER>
 * 		<PARAMETER NAME="port">10005</PARAMETER>
 * 	</CONFIG>		
 * 	<CONFIG NAME="config2">
 * 		<PARAMETER NAME="connectiontype">gmsec_ss</PARAMETER>
 * 		<PARAMETER NAME="server">tcp:10.1.2.159</PARAMETER>
 * 		<PARAMETER NAME="tracking">true</PARAMETER>
 * 		<PARAMETER NAME="isthreaded">true</PARAMETER>
 * 		<PARAMETER NAME="compress">true</PARAMETER>
 * 	</CONFIG>
 *	<MESSAGE NAME="msg1" SUBJECT="gmsec.mission.const.sat.evt.msg" TYPE="publish">
 * 		<CONFIG NAME="msg_config">
 * 			<PARAMETER NAME="KIND">GMSEC_MSG_PUBLISH</PARAMETER>
 * 		</CONFIG>		
 * 		<FIELD TYPE="CHAR" NAME="C">c</FIELD>
 * 		<FIELD TYPE="BOOL" NAME="B">TRUE</FIELD>
 * 		<FIELD TYPE="I16" NAME="ID">123</FIELD>
 * 		<FIELD TYPE="U16" NAME="UD">123</FIELD>
 * 		<FIELD TYPE="I32" NAME="IN">123</FIELD>
 * 		<FIELD TYPE="U32" NAME="UN">123</FIELD>
 * 		<FIELD TYPE="STRING" NAME="S">This is a test</FIELD>
 * 		<FIELD TYPE="F32" NAME="F">123</FIELD>
 * 		<FIELD TYPE="F64" NAME="E">123</FIELD>
 * 		<FIELD TYPE="BIN" NAME="BLOB">4a4c4d4e4f5051</FIELD>
 * 	</MESSAGE>
 * </DEFINITIONS>
 *  @endcode
 * 
 */
@Deprecated
public class ConfigFile
{
	private JNIConfigFile fConfigFile;


	/**
	 * @desc Once the ConfigFile object has been created, use Load(String filepath)
	 * to load a configuration file, or if the plan is to add elements to the empty
	 * ConfigFile object, then use Save(String filepath) to save any changes.
	 *
	 * @brief Basic constructor that creates an empty configuration file object.
	 */
	public ConfigFile()
	{
		fConfigFile = new JNIConfigFile();
	}


	/**
	 * @note This constructor has been deprecated; use ConfigFile() instead.
	 * @brief This constructor creates the association of this object
	 * with a configuration file.
	 *
	 * @param filepath - full or relative path to configuration file
	 */
	public ConfigFile(String filepath)
	{
		if (filepath == null)
		{
			filepath = "";
		}

		fConfigFile = new JNIConfigFile(filepath);
	}


	/**
	 * @note This method has been deprecated; use Load(String filepath) instead.
	 * @brief This function opens the configuration file and parses the XML
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @return status of whether the load and parse was successful
	 */
	public Status Load()
	{
		return fConfigFile.Load();
	}


	/**
	 * @brief This function opens the configuration file and parses the XML
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param filepath - full or relative path to configuration file
	 *
	 * @return status of whether the load and parse was successful
	 */
	public Status Load(String filepath)
	{
		return fConfigFile.Load(filepath);
	}


	/**
	 * @brief This function saves the currently open configuration file
	 * without compacting the XML strings it contains.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @return status of whether the file save operation was successful
	 */
	public Status Save()
	{
		return Save(null, false);
	}


	/**
	 * @brief This function saves the currently open configuration file to
	 *  the location specified without compacting the XML strings it contains.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param filePath - The location to which the file will be saved
	 * if the path is specified as null, then the path, if any, that
	 * was used with Load(const char* filepath) will be used when saving the file.
	 * @return status of whether the file save operation was successful
	 */
	public Status Save(String filePath)
	{
		return Save(filePath, false);
	}
	

	/**
	 * @brief This function saves the currently open configuration file to
	 *  the location specified.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @param filePath - The location to which the file will be saved
	 * if the path is specified as null, then the path, if any, that
	 * was used with Load(const char* filepath) will be used when saving the file.
	 * @param compact - determines whether the XML output will be compacted or not
	 * @return status of whether the file save operation was successful
	 */
	public Status Save(String filePath, boolean compact)
	{
		return fConfigFile.Save(filePath, compact);
	}


	/**
	 * @brief This function will parse the xml string as a config file.
	 *
	 * @return status of whether the parse was successful
	 */
	public Status FromXML(String xml)
	{
		return fConfigFile.FromXML(xml);
	}


	/**
	 * @brief This function will dump this config file to XML.
	 * The format is the same as that which is parsed by FromXML().
	 * 
	 * @param xml - output parameter: xml string
	 * @return status object with the result of the operation
	 * 
	 * @sa FromXML()
	 */
	public Status ToXML(GMSEC_String xml)
	{
		return fConfigFile.ToXML(xml);
	}


	/**
	 * @brief This function will return true if the config file has been successfully loaded.
	 *
	 * @return true if file has been loaded
	 */
	public boolean IsLoaded()
	{
		return fConfigFile.IsLoaded();
	}


	/**
	 * @brief This function will attempt to find a named connection in the
	 * config file and set the passed in config object with
	 * the loaded values.
	 *
	 *  Example config file load and get config:
	 *  @code
	 * // create configfile object and attach to file
	 * ConfigFile cf = new ConfigFile("path/to/file.xml");
	 * 
	 * // load & parse
	 * result = cf.Load();
	 * if( result.isError() ) {
	 *	//handle error
	 * 
	 * // get 2 configs from file
	 * Config c1 = new Config();
	 * Config c2 = new Config();
	 * cf.LookupConfig("config1",c1);
	 * cf.LookupConfig("config2",c2);
	 *  @endcode
	 *
	 * @param name - value in the name="" attribute of the \<CONFIG/\> block to seed the gmsec::Config object
	 * @param cfg - Config object to set
	 * @return status of whether named CONFIG block was found
	 * 
	 */
	public Status LookupConfig(String name, Config cfg)
	{
		if (name == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_INVALID_CONFIG_NAME, "Name is null");
		}
		if (cfg == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_INVALID_CONFIG, "Config is null");
		}

		return fConfigFile.LookupConfig(name, Config.getInternal(cfg));
	}


	/**
	 * @brief This function will attempt to find a named message in the
	 * config file and create a new message from that xml
	 * the loaded fields and values.
	 *
	 *  Example config file load and get message:
	 *  @code
	 * // create configfile object and attach to file
	 * ConfigFile cf = new ConfigFile("path/to/file.xml");
	 * 
	 * // load & parse
	 * result = cf.Load();
	 * if( result.isError() ) {
	 *	//handle error
	 * 
	 * // create message from config file entry
	 * Message msg;
	 * result = conn.CreateMessage(msg);
	 * if( result.isError() )
	 *	//handle error
	 * 	
	 * // load message subject, type, and fields from config file
	 * result = cf.LookupMessage("msg1",msg);
	 * if( result.isError() )
	 *	//handle error
	 *  @endcode
	 *
	 * @param name - value in the name="" attribute of the \<MESSAGE/\> block to seed the gmsec::Message object
	 * @param msg - Message object to set
	 * @return status of whether named MESSAGE block was found
	 */
	public Status LookupMessage(String name, Message msg)
	{
		if (name == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Name is null");
		}
		if (msg == null || msg.GetImpl() == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONNECTION_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Message is null");
		}

		return fConfigFile.LookupMessage(name, msg);
	}


	/**
	 * @brief This function will look up a subscription pattern defined in
	 * the config file. This is useful to allow easy modification of subject
	 * names without code changes.
	 *
	 * Example 
	 * @code
	 * GMSEC_String pattern = new GMSEC_String();
	 *
	 * // lookup subject pattern by name
	 * result = cf.LookupSubscription( "events", pattern );
	 * if( result.isError() )
	 *	//handle error
	 *
	 * System.out.println("Subscribe to: " + pattern);
	 *
	 * // subscribe on the connection
	 * result = conn.Subscribe(pattern);
	 * if( result.isError() )
	 *	//handle error
	 * @endcode
	 * 
	 * @param name - value in the name="" attribute of the \<SUBSCRIPTION/\> block containing the subscription pattern
	 * @param pattern - out parameter, the pattern defined in the config file
	 * @return status of whether named SUBSCRIPTION block was found
	 */
	public Status LookupSubscription(String name, GMSEC_String pattern)
	{
		if (name == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Name is null");
		}
		if (pattern == null)
		{
			return new Status(gmsecAPI.GMSEC_STATUS_CONFIG_ERROR, gmsecAPI.GMSEC_UNINITIALIZED_OBJECT, "Pattern is null");
		}

		return fConfigFile.LookupSubscription(name, pattern);
	}


	/**
	 * @brief Adds a Subscription definition to the list of subscriptions.
	 * 	
	 * Programmatic XML element addition will use pre-defined hierarchical rules for
	 * determining the placement of elements within the document, if saved.
	 * When adding a subscription definition, it will initially be inserted after
	 * the last <SUBSCRIPTION> element in the document, if there are no subscription
	 * definitions then it will be added as the very first child of the root element.
	 * 
	 * Adding a subscription definition with a name that currently exists will
	 * overwrite the current subscription.
	 * Adding a Subscription definition to a newly instantiated ConfigFile object that has not
	 * loaded a document will create a new XML document.
	 * 
	 * @param name - name to be associated with the Subscription
	 * @param subscription - XML string representation of a GMSEC Subscription
	 * @return status - whether the addition was successful or not
	 */
	public Status AddSubscription(String name, String subscription)
	{
		return fConfigFile.AddSubscription(name, subscription);
	}


	/**
	 * @brief Adds a config definition to the list of configs.
	 *
	 * Programmatic XML element addition will use pre-defined hierarchical rules for
	 * determining the placement of elements within the document, if saved.
	 * When adding a config element, it will initially be inserted after
	 * the last <CONFIG> element in the document, if no other config definitions are
	 * present in the document, the new config definition will be added after the
	 * last <SUBSCRIPTION> element in the document, if there are no subscription
	 * definitions then it will be added as the very first child of the root element.
	 *
	 * Adding a config definition with a name that currently exists will overwrite
	 * the current config.
	 * Adding a Config definition to a newly instantiated ConfigFile object that has not
	 * loaded a document will create a new XML document.
	 * 
	 * @param name - name to be associated with the Config definition
	 * @param config - a GMSEC Config object
	 * @return status - whether the addition was successful or not
	 */
	public Status AddConfig(String name, Config config)
	{
		return fConfigFile.AddConfig(name, config);
	}


	/**
	 * @brief Adds a message definition to the list of messages.
	 *
	 * Programmatic XML element addition will use pre-defined hierarchical rules for
	 * determining the placement of elements within the document, if saved.
	 * When adding a message definition, it will initially be inserted after
	 * the last <MESSAGE> element in the document, if no message definitions
	 * are present in the document, it will be added after the last <CONFIG> element
	 * in the document, if no other config definitions are present in the document, the new
	 * config definition will be added after the last <SUBSCRIPTION> element in the
	 * document, if there are no subscription definitions then it will be added as
	 * the very first child of the root element.
	 *
	 * Adding a message definition with a name that currently exists will overwrite
	 * the current message.
	 * Adding a Message definition to a newly instantiated ConfigFile object that has not
	 * loaded a document will create a new XML document.
	 * 
	 * @param name - name to be associated with the Message definition
	 * @param message - a GMSEC Message object
	 * @return status - whether the addition was successful or not
	 */
	public Status AddMessage(String name, Message message)
	{
		return fConfigFile.AddMessage(name, message);
	}


	/**
	 * @brief Adds a custom (non-GMSEC) XML string to the ConfigFile.
	 *
	 * Programmatic XML element addition will use pre-defined hierarchical rules for
	 * determining the placement of elements within the document, if saved.
	 * When adding a custom xml definition, it will initially be inserted after
	 * the last <MESSAGE> element in the document, if no message definitions
	 * are present in the document, it will be added after the last <CONFIG> element
	 * in the document, if no other config definitions are present in the document, the new
	 * config definition will be added after the last <SUBSCRIPTION> element in the
	 * document, if there are no subscription definitions then it will be added as
	 * the very first child of the root element.
	 *
	 * Adding a custom XML string to a newly instantiated ConfigFile object that has not
	 * loaded a document will create a new XML document.
	 * 
	 * @param xml - a custom, user-created XML string
	 * @return status - whether the addition was successful or not
	 */
	public Status AddCustomXML(String xml)
	{
		return fConfigFile.AddCustomXML(xml);
	}


	/**
	 * @brief Removes the Subscription definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the Subscription definition
	 * @return status - whether the removal was successful or not
	 */
	public Status RemoveSubscription(String name)
	{
		return fConfigFile.RemoveSubscription(name);
	}


	/**
	 * @brief Removes the Config definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the Config definition
	 * @return status - whether the removal was successful or not
	 */
	public Status RemoveConfig(String name)
	{
		return fConfigFile.RemoveConfig(name);
	}


	/**
	 * @brief Removes the Message definition which is mapped to the provided name.
	 *
	 * @param name - name associated with the Message definition
	 * @return status - whether the removal was successful or not
	 */
	public Status RemoveMessage(String name)
	{
		return fConfigFile.RemoveMessage(name);
	}


	/**
	 * @brief Removes the provided xml element.
	 *
	 * @param xml - XML element string matching the custom XML definition
	 * @return status - whether the removal was successful or not
	 */
	public Status RemoveCustomXML(String xml)
	{
		return fConfigFile.RemoveCustomXML(xml);
	}

	
}
