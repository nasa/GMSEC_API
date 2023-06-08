/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Config.h
 *
 *  @brief Config class for configuring Connection and Message objects.
 */

#ifndef GMSEC_API5_CONFIG_H
#define GMSEC_API5_CONFIG_H

#include <gmsec5_defs.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{

namespace internal
{
	class InternalConfig;
}

/**
 * @class Config
 *
 * @brief This class is a collection for managing connection configuration items.
 * 
 * Encapsulates the parameters necessary for initialization Connections and setting
 * middleware dependent values in Messages.  Used by the Connection and Message,
 * this class provides a generic means of supplying initialization data without
 * being Connection/Message specific.
 *
 * @sa Connection
 * @sa Message
 */
class GMSEC_API Config
{
public:
	/**
	 * @fn Config()
	 * @brief Default Constructor.
	 */
	Config();


	/**
	 * @fn Config(int argc, char* argv[])
	 *
	 * @brief This constructor will parse command line parameters and add
	 * values for any param that is of the form \<name\>=\<value\>
	 *
	 * @code gmsub.exe connectionType=bolt server=localhost @endcode
	 *
	 * @param argc - number of parameters in argv (from main)
	 * @param argv - an array of char* parameters (from main)
	 */
	Config(int argc, char* argv[]);


	/**
	 * @fn Config(const char* data, DataType type)
	 *
	 * @brief This constructor will create a Config object by parsing key/value pairs from the
	 * given data string.  The data string can contain either XML or JSON formatted configuration
	 * data, or contain raw data consisting of white-space separated key=value pairs.
	 *
	 * @param data - data string containing configuration information
	 * @param type - the type of data being provided
	 *
	 * @throw GmsecException is thrown if the data string is NULL, or contains an empty string.
	 * @throw GmsecException is thrown if the data string is not parsable.
	 *
	 * @sa toXML()
	 * @sa toJSON()
	 */
	Config(const char* data, DataType type);


	/**
	 * @fn Config(const Config &cfg)
	 *
	 * @brief Basic (and safe) deep copy constructor
	 *
	 * @param cfg - config object to copy
	 */
	Config(const Config &cfg);


	/**
	 * @fn ~Config()
	 *
	 * @brief destructor
	 */
	~Config();


	/**
	 * @fn operator=(const Config& other)
	 *
	 * @brief Overloaded assignment operator method for making a copy of other Config object
	 *
	 * @param Config object to copy
	 *
	 * @return A copy of the other Config object that was given.
	 */
	Config& CALL_TYPE operator=(const Config& other);


	/**
	 * @fn addValue(const char* name, const char* value)
	 *
	 * @brief This function adds a value to the config object by name
	 *
	 * @param name - name of value to add
	 * @param value - value to be added
	 *
	 * @throw GmsecException if either name or value are NULL, or are empty strings
	 */
	void CALL_TYPE addValue(const char* name, const char* value);


	/**
	 * @fn clearValue(const char* name);
	 *
	 * @brief This function clears a value by name
	 *
	 * @param name - name of value to clear
	 * @return boolean to indicate whether operation was successful
	 *
	 * @throw GmsecException if name is NULL, or an empty string
	 */
	bool CALL_TYPE clearValue(const char* name);


	/**
	 * @fn const char* getValue(const char* name) const
	 *
	 * @brief This function gets a value by name. The scope of the value is the life of the Config object.
	 *
	 * @param name - name of value to get
	 *
	 * @return If the key entry is found, then the associated value is returned; otherwise NULL is returned.
	 *
	 * @throw GmsecException if name is NULL
	 */
	const char* CALL_TYPE getValue(const char* name) const;


	/**
	 * @fn const char* getValue(const char* name, const char* defaultValue) const
	 *
	 * @brief This function gets a value by name and if there is no value assigned to will return 
	 * the defaultValue parameter. defaultValue can be NULL or empty. The scope of the value is the life of
	 * the Config object.
	 *
	 * @param name - name of value to get
	 * @param defaultValue - default value to be returned if no value exist for name parameter
	 *
	 * @return Value of named item, or default value if it does not exist
	 */
	const char* CALL_TYPE getValue(const char* name, const char* defaultValue) const;


	/**
	 * @fn getBooleanValue(const char* name) const
	 *
	 * @brief This function attempts to get the boolean representation of configuration
	 * element. An exception is thrown if the element does not exist or there is no
	 * valid boolean interpretion of the value.
	 *
	 * @param name - name of value to get
	 * 
	 * @return value of named item
	 *
	 * @throw GmsecException if name is NULL or contains empty string
	 */
	bool CALL_TYPE getBooleanValue(const char* name) const;


	/**
	 * @fn getBooleanValue(const char* name, bool defaultValue) const
	 *
	 * @brief This function attempts to get the boolean representation of configuration
	 * element. If the element does not exist or the element cannot be interpreted as
	 * a boolean, the default value is returned.
	 *
	 * @param name - name of value to get
	 * @param defaultValue - default value to be returned if no value exist for name parameter
	 *
	 * @return Value of named item, or default value if it does not exist
	 */
	bool CALL_TYPE getBooleanValue(const char* name, bool defaultValue) const;


	/**
	 * @fn getIntegerValue(const char* name) const
	 *
	 * @brief This function attempts to get the integer representation of configuration
	 * element. An exception is thrown if the element does not exist or there is no
	 * valid integer interpretion of the value.
	 *
	 * @param name - name of value to get
	 *
	 * @return value of named item
	 *
	 * @throw GmsecException if name is NULL or contains empty string
	 */
	int CALL_TYPE getIntegerValue(const char* name) const;


	/**
	 * @fn getIntegerValue(const char* name, int defaultValue) const
	 *
	 * @brief This function attempts to get the integer representation of configuration
	 * element. If the element does not exist or the element cannot be interpreted as
	 * a integer, the default value is returned.
	 *
	 * @param name - name of value to get
	 * @param defaultValue - default value to be returned if no value exist for name parameter
	 *
	 * @return Value of named item, or default value if it does not exist
	 */
	int CALL_TYPE getIntegerValue(const char* name, int defaultValue) const;


	/**
	 * @fn getDoubleValue(const char* name) const
	 *
	 * @brief This function attempts to get the double representation of configuration
	 * element. An exception is thrown if the element does not exist or there is no
	 * valid double interpretion of the value.
	 *
	 * @param name - name of value to get
	 *
	 * @return value of named item
	 *
	 * @throw GmsecException if name is NULL or contains empty string
	 */
	double CALL_TYPE getDoubleValue(const char* name) const;


	/**
	 * @fn getDoubleValue(const char* name, double defaultValue) const
	 *
	 * @brief This function attempts to get the double representation of configuration
	 * element. If the element does not exist or the element cannot be interpreted as
	 * a double, the default value is returned.
	 *
	 * @param name - name of value to get
	 * @param defaultValue - default value to be returned if no value exist for name parameter
	 *
	 * @return Value of named item, or default value if it does not exist
	 */
	double CALL_TYPE getDoubleValue(const char* name, double defaultValue) const;


	/**
	 * @fn clear()
	 *
	 * @brief This function clears all values
	 */
	void CALL_TYPE clear();


	/**
	 * @fn getFirst(const char*& name, const char*& value) const
	 *
	 * @brief This function gets the first name & value for iteration. The scope
	 * of the name and value are the life of the Config object, or
	 * until the next %getFirst() call.
	 *
	 * @param name - output parameter name of value
	 * @param value - output parameter value
	 *
	 * @return boolean status as to whether name/value pair are available
	 */
	bool CALL_TYPE getFirst(const char*& name, const char*& value) const;


	/**
	 * @fn getNext(const char*& name, const char*& value) const
	 *
	 * @brief This function gets the next name & value for iteration. The scope
	 * of the name and value are the life of the Config object, or
	 * until the next %getNext() call.
	 *
	 * @param name - output parameter name of value
	 * @param value - output parameter value
	 *
	 * @return boolean status as to whether name/value pair are available
	 */
	bool CALL_TYPE getNext(const char*& name, const char*& value) const;


	/**
	 * @fn merge(const Config& other, bool overwriteExisting = true)
	 *
	 * @brief Merge the given %Config object with the current %Config object.
	 *
	 * @param other - the %Config object from which to read name/value pairs.
	 * @param [optional] overwriteExisting - indicates whether to overwrite any existing name/value
	 * pairs within the working %Config object.  If flag is omitted, then the default is
	 * to overwrite existing name/value pairs.
	 */
	void CALL_TYPE merge(const Config& other, bool overwriteExisting = true);


	/**
	 * @fn toXML() const
	 *
	 * @brief This function will dump the Config into XML format.
	 * The format is the same as that which is parsed by fromXML().
	 *
	 * @return XML string
	 *
	 * @sa fromXML(const char* xml)
	 */
	const char* CALL_TYPE toXML() const;


	/**
	 * @fn fromXML(const char* xml)
	 *
	 * @brief This function will read a configuration from an XML string.
	 * The format is the same as that which is generated by toXML().
	 *
	 * Example XML string:
	 * @code
	 * <CONFIG NAME="Bolt">
	 *    <PARAMETER NAME="mw-id">bolt</PARAMETER>
	 *    <PARAMETER NAME="server">127.0.0.1</PARAMETER>
	 *    <PARAMETER NAME="tracking">false</PARAMETER>
	 * </CONFIG>
	 * @endcode
	 *
	 * @param xml - XML string
	 *
	 * @throw GmsecException if a malformed XML, or NULL, string is given
	 *
	 * @sa toXML(const char*& xml)
	 * @sa Config(const char* data, DataType type)
	 */
	void CALL_TYPE fromXML(const char* xml);


	/**
	 * @fn Config CALL_TYPE getFromFile(const char* filename, const char* configName)
	 *
	 * @brief Attempts to open the given file and use the contents to create a Config object.
	 *
	 * @param filename   - The configuration file to open.
	 * @param configName - The name of the configuration to load/process.
	 *
	 * @return A Config object.
	 *
	 * @throw GmsecException if the file name is NULL, or consists of an empty string.
	 * @throw GmsecException if the file cannot be opened.
	 * @throw GmsecException if the file does not contain a valid configuration construct.
	 * @throw GmsecException if the file does not contain the named configuration construct.
	 *
	 * @sa fromXML
	 */
	static Config CALL_TYPE getFromFile(const char* filename, const char* configName);


	/**
	 * @fn toJSON() const
	 *
	 * @brief This function will dump the Config into JSON format.
	 * The format is the same as that which is accepted by Config(const char* data).
	 *
	 * @return JSON formatted string
	 *
	 * @sa Config(const char* data)
	 */
	const char* CALL_TYPE toJSON() const; 


private:
	internal::InternalConfig* m_internal;
};

} // namespace api5
} // namespace gmsec

#endif
