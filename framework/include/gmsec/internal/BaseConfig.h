
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file BaseConfig.h
 *  @brief This file contains a class for managing configuration values.
**/

#ifndef gmsec_internal_BaseConfig_h
#define gmsec_internal_BaseConfig_h


#include <string>
#include <map>

#include <gmsec/Config.h>
#include <gmsec/Status.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec/internal/ci_less.h>
#include <gmsec/internal/tinyxml2.h>


namespace gmsec
{
namespace internal
{

/** @class BaseConfig
 * @brief This class implements the Config interface.
 * In the future, there may be alternative implementations, but for now
 * it just uses the pre-3.0 implementation.
 * @sa gmsec::Config
**/
class GMSEC_API BaseConfig
{
private:

	// Declared, but not implemented.
	BaseConfig &operator=(const BaseConfig &);

	std::string  fXmlOutput;

	typedef std::map<std::string, std::string, ci_less> conBaseConfigMap;
	typedef conBaseConfigMap::iterator configItr;

	conBaseConfigMap fConfigs;
	configItr fCfgItr;

public:

	/** @fn BaseConfig()
	 * Default Constructor.
	 */
	BaseConfig();

	/** @fn BaseConfig( int argc, char* argv[] )
	 * @brief This constructor will parse command line parameters and add
	 * values for any param that is of the form \<name\>=\<value\>
	 *
 	 * @code gmsub.exe connectionType=ICSSWB hostname=localhost port=10005 @endcode
	 *
	 * @param argc - number of parameters in argv (from main)
	 * @param argv - an array of char* parameters (from main)
	 */
	BaseConfig(int argc, char* argv[]);

	/** @fn BaseConfig( const char *xml )
	 * @brief This constructor will create a config and parse values from the passed in xml string.
	 * The xml format is the same as used by the FromXML() and ToXML() functions.
	 *
	 * @param xml - string in xml format (see FromXML)
	 *
	 * @sa FromXML() @n ToXML()
	 */
	BaseConfig(const char *xml);

	/** @fn BaseConfig( const BaseConfig &cfg )
	 * @brief Basic (and safe) deep copy constructor
	 *
	 * @param cfg - config object to copy
	 */
	BaseConfig(const BaseConfig &cfg);

	/** @fn ~BaseConfig()
	 * @brief destructor
	 */
	virtual ~BaseConfig();

	/** @fn AddValue( const char *name, const char *value )
	 * @brief This function adds a value to the config object by name
	 *
	 * @param name - name of value to add
	 * @param value - value to be added
	 * @return status object with the result of the operation
	 */
	Status AddValue(const char *name, const char *value);

	/** @fn ClearValue( const char *name );
	 * @brief This function clears a value by name
	 *
	 * @param name - name of value to clear
	 * @return status object with the result of the operation
	 */
	Status ClearValue(const char *name);

	/** @fn GetValue( const char *name, const char *&value ) const
	 * @brief This function gets a value by name. The scope of the value
	 * is the life of the BaseConfig object, or until the
	 * next %GetValue() call.
	 *
	 * @param name - name of value to get
	 * @param value - output parameter value
	 * @return status object with the result of the operation
	 */
	Status GetValue(const char *name, const char *&value) const;

	/** @fn CheckBoolValue( const char *name, bool defaultValue ) const
	 * @brief This function attempts to get a named value as a boolean. (true/false)
	 * It will return the default value if the field is not set, or the value does
	 * not conform to "true" or "false".
	 *
	 * @param name - name of value to get
	 * @param defaultValue - default value describe above
	 * @return value of named item
	 */
	bool CheckBoolValue(const char *name, bool defaultValue) const;

	/** @fn Clear()
	 * @brief This function clears all values
	 *
	 * @return status object with the result of the operation
	 */
	Status Clear();

	/** @fn GetFirst( const char *&name, const char *&value ) const
	 * @brief This function gets the first name & value for iteration. The scope
	 * of the name and value are the life of the BaseConfig object, or
	 * until the next %GetFirst() call.
	 *
	 * @param name - output parameter name of value
	 * @param value - output parameter value
	 * @return status object with the result of the operation
	 */
	Status GetFirst(const char *&name, const char *&value) const;

	/** @fn GetNext( const char *&name, const char *&value ) const
	 * @brief This function gets the next name & value for iteration. The scope
	 * of the name and value are the life of the BaseConfig object, or
	 * until the next %GetNext() call.
	 *
	 * @param name - output parameter name of value
	 * @param value - output parameter value
	 * @return status object with the result of the operation
	 */
	Status GetNext(const char *&name, const char *&value) const;

	/** @fn ToXML( const char *&xml )
	 * @brief This function will dump this config to XML.
	 * The format is the same as that which is parsed by FromXML().
	 *
	 * @param xml - output parameter: xml string
	 * @return status object with the result of the operation
	 *
	 * @sa FromXML
	 */
	Status ToXML(const char *&xml);

	/** @fn FromXML( const char *xml )
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
	 * @sa ToXML
	 */
	Status FromXML(const char *xml);


	/** @fn FromXML( XMLElement *element )
	 * @brief helper function that parses a config xmlNode
	 */
	Status FromXML(tinyxml2::XMLElement *element);


	static BaseConfig *getInternal(Config *cfg);
};

} // namespace internal
} // namespace gmsec

#endif  // gmsec_internal_BaseConfig_h

