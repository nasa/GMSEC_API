/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file Config.h
 *
 *  @brief This file contains a class for managing configuration values.
**/

#ifndef GMSEC_API_CONFIG_H
#define GMSEC_API_CONFIG_H

#include <gmsec4/util/wdllexp.h>

#include <gmsec4/Status.h>


namespace gmsec
{
namespace api
{

namespace internal
{
	class InternalConfig;
}

/** @class Config
 * @brief This class is a collection for managing connection configuration items.
 * 
 * Encapsulates the parameters necessary for initialization Connections and setting
 * middleware dependent values in Messages.  Used by the Connection and Message,
 * this class provides a generic means of supplying initialization data without
 * being Connection/Message specific.
 *
 * @sa Connection @n
 *     Message
**/
class GMSEC_API Config
{
public:
	/** @fn Config()
	 * Default Constructor.
	 */
	Config();


	/** @fn Config(int argc, char* argv[])
	 * @brief This constructor will parse command line parameters and add
	 * values for any param that is of the form \<name\>=\<value\>
	 *
	 * @code gmsub.exe connectionType=bolt server=localhost @endcode
	 *
	 * @param argc - number of parameters in argv (from main)
	 * @param argv - an array of char* parameters (from main)
	 */
	Config(int argc, char* argv[]);


	/** @fn Config( const char *xml )
	 * @brief This constructor will create a config and parse values from the passed in xml string.
	 * The xml format is the same as used by the FromXML() and ToXML() functions.
	 *
	 * @param xml - string in xml format (see FromXML)
	 *
	 * @sa FromXML() @n ToXML()
	 */
	Config(const char *xml);


	/** @fn Config( const Config &cfg )
	 * @brief Basic (and safe) deep copy constructor
	 *
	 * @param cfg - config object to copy
	 */
	Config(const Config &cfg);


	/** @fn ~Config()
	 * @brief destructor
	 */
	~Config();


	/** @fn operator=(const Config& other)
	 * @brief Overloaded assignment operator method for making a copy of other Config object
	 * @param Config object to copy
	 * @return A copy of the other Config object that was given.
	 */
	Config& CALL_TYPE operator=(const Config& other);


	/** @fn addValue(const char* name, const char* value)
	 * @brief This function adds a value to the config object by name
	 *
	 * @param name - name of value to add
	 * @param value - value to be added
	 * @return status object with the result of the operation
	 * @throws Exception if either name or value are NULL, or are empty strings
	 */
	void CALL_TYPE addValue(const char* name, const char* value);


	/** @fn clearValue(const char* name);
	 * @brief This function clears a value by name
	 *
	 * @param name - name of value to clear
	 * @return boolean to indicate whether operation was successful
	 * @throws Exception if name is NULL
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
	 * @throws Exception if name is NULL
	 */
	const char* CALL_TYPE getValue(const char* name) const;


	/**
	 * @fn const char* getValue(const char* name) const
	 *
	 * @brief This function gets a value by name. The scope of the value is the life of the Config object.
	 *
	 * @param name - name of value to get
	 *
	 * @return If the key entry is found, then the associated value is returned; otherwise NULL is returned.
	 *
	 * @throws Exception if name is NULL
	 */
	const char* CALL_TYPE getValue(const char* name, const char* defaultValue) const;

	/** @fn getBooleanValue(const char* name) const
	 * @brief This function attempts to get the boolean representation of configuration
	 * element. An exception is thrown if the element does not exist or there is no
	 * valid boolean interpretion of the value.
	 *
	 * @param name - name of value to get
	 * @return value of named item
	 * @throws Exception if name is NULL
	 */
	bool CALL_TYPE getBooleanValue(const char* name) const;


	/** @fn getBooleanValue(const char* name, bool defaultValue) const
	 * @brief This function attempts to get the boolean representation of configuration
	 * element. If the element does not exist or the element cannot be interpreted as
	 * a boolean, the default value is returned.
	 *
	 * @param name - name of value to get
	 * @return value of named item
	 * @throws Exception if name is NULL
	 */
	bool CALL_TYPE getBooleanValue(const char* name, bool defaultValue) const;


	/** @fn getIntegerValue(const char* name) const
	 * @brief This function attempts to get the integer representation of configuration
	 * element. An exception is thrown if the element does not exist or there is no
	 * valid integer interpretion of the value.
	 *
	 * @param name - name of value to get
	 * @return value of named item
	 * @throws Exception if name is NULL
	 */
	int CALL_TYPE getIntegerValue(const char* name) const;


	/** @fn getIntegerValue(const char* name, int defaultValue) const
	 * @brief This function attempts to get the integer representation of configuration
	 * element. If the element does not exist or the element cannot be interpreted as
	 * a integer, the default value is returned.
	 *
	 * @param name - name of value to get
	 * @return value of named item
	 * @throws Exception if name is NULL
	 */
	int CALL_TYPE getIntegerValue(const char* name, int defaultValue) const;


	/** @fn getDoubleValue(const char* name) const
	 * @brief This function attempts to get the double representation of configuration
	 * element. An exception is thrown if the element does not exist or there is no
	 * valid double interpretion of the value.
	 *
	 * @param name - name of value to get
	 * @return value of named item
	 * @throws Exception if name is NULL
	 */
	double CALL_TYPE getDoubleValue(const char* name) const;


	/** @fn getDoubleValue(const char* name, double defaultValue) const
	 * @brief This function attempts to get the double representation of configuration
	 * element. If the element does not exist or the element cannot be interpreted as
	 * a double, the default value is returned.
	 *
	 * @param name - name of value to get
	 * @return value of named item
	 * @throws Exception if name is NULL
	 */
	double CALL_TYPE getDoubleValue(const char* name, double defaultValue) const;


	/** @fn clear()
	 * @desc This function clears all values
	 */
	void CALL_TYPE clear();


	/** @fn getFirst(const char*& name, const char*& value) const
	 * @brief This function gets the first name & value for iteration. The scope
	 * of the name and value are the life of the Config object, or
	 * until the next %GetFirst() call.
	 *
	 * @param name - output parameter name of value
	 * @param value - output parameter value
	 * @return boolean status as to whether name/value pair are available
	 */
	bool CALL_TYPE getFirst(const char*& name, const char*& value) const;


	/** @fn getNext(const char*& name, const char*& value) const
	 * @brief This function gets the next name & value for iteration. The scope
	 * of the name and value are the life of the Config object, or
	 * until the next %getNext() call.
	 *
	 * @param name - output parameter name of value
	 * @param value - output parameter value
	 * @return boolean status as to whether name/value pair are available
	 */
	bool CALL_TYPE getNext(const char*& name, const char*& value) const;


	/**
	 * @fn merge(const Config& other, bool overwriteExisting = true)
	 *
	 * @brief Merge the given %Config object with the current %Config object.
	 *
	 * @param other - the %Config object from which to read name/value pairs.
	 * @param overwriteExisting - indicates whether to overwrite any existing name/value
	 * pairs within the working %Config object.  If flag is omitted, then the default is
	 * to overwrite existing name/value pairs.
	 */
	void CALL_TYPE merge(const Config& other, bool overwriteExisting = true);


	/** @fn toXML() const
	 * @brief This function will dump the Config into XML format.
	 * The format is the same as that which is parsed by fromXML().
	 *
	 * @return XML string
	 *
	 * @sa fromXML(const char* xml)
	 */
	const char* CALL_TYPE toXML() const;


	/** @fn fromXML(const char* xml)
	 * @brief This function will read a configuration from an XML string.
	 * The format is the same as that which is generated by toXML().
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
	 * @param xml - xml string
	 * @throws Exception if a malformed XML, or NULL, string is given
	 * @sa toXML(const char*& xml)
	 */
	void CALL_TYPE fromXML(const char* xml);


private:
	internal::InternalConfig* m_iConfig;
};

} // namespace api
} // namespace gmsec

#endif
