
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Config.h
 *
 *  @brief This file contains a class for managing configuration values.
**/

#ifndef gmsec_Config_h
#define gmsec_Config_h


#include <gmsec/ConfigFile.h>
#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{

/** @class Config
 * @brief This class is a collection for managing connection configuration items.
 * 
 * Encapsulates the parameters necessary for initialization Connections and setting middleware dependent
 * values in Messages.  Used by the ConnectionFactory and Message, this class provides a generic means
 * of supplying initialization data without being Connection/Message specific.i
 *
 * Example of Config creation & use:
 * @code
 * Config cfg(argc,argv);	// create from main parameters
 *
 * const char *name = NULL;
 * const char *value = NULL;
 *
 * Status res = cfg.GetFirst(name,value);	// get first value
 * while(!res.isError()) {
 *	cout << name << "," << value << endl;
 *	res = cfg.GetNext(name,value);	// get next value
 * }
 * @endcode
 * 
 * A Config object has a one-to-one mapping to a connection configuration. If a user wishes to maintain
 * multiple connection items within one object, the use of a ConfigFile object is suggested.
 *
 * @sa Connection @n
 *	   Message
**/
namespace internal
{
class BaseConfig;
}

class GMSEC_API Config
{
public:

	/** @fn Config()
	 * Default Constructor.
	 */
	GMSEC_DEPRECATED Config();

	/** @fn Config( int argc, char* argv[] )
	 * @brief This constructor will parse command line parameters and add
	 * values for any param that is of the form \<name\>=\<value\>
	 *
	  * @code gmsub.exe connectionType=ICSSWB hostname=localhost port=10005 @endcode
	 *
	 * @param argc - number of parameters in argv (from main)
	 * @param argv - an array of char* parameters (from main)
	 */
	GMSEC_DEPRECATED Config(int argc, char* argv[]);

	/** @fn Config( const char *xml )
	 * @brief This constructor will create a config and parse values from the passed in xml string.
	 * The xml format is the same as used by the FromXML() and ToXML() functions.
	 *
	 * @param xml - string in xml format (see FromXML)
	 *
	 * @sa FromXML() @n ToXML()
	 */
	GMSEC_DEPRECATED Config(const char *xml);

	/** @fn Config( const Config &cfg )
	 * @brief Basic (and safe) deep copy constructor
	 *
	 * @param cfg - config object to copy
	 */
	GMSEC_DEPRECATED Config(const Config &cfg);

	/** @fn ~Config()
	 * @brief destructor
	 */
	~Config();

	/** @fn AddValue( const char *name, const char *value )
	 * @brief This function adds a value to the config object by name
	 *
	 * @param name - name of value to add
	 * @param value - value to be added
	 * @return status object with the result of the operation
	 */
	Status CALL_TYPE AddValue(const char *name, const char *value);

	/** @fn ClearValue( const char *name );
	 * @brief This function clears a value by name
	 *
	 * @param name - name of value to clear
	 * @return status object with the result of the operation
	 */
	Status CALL_TYPE ClearValue(const char *name);

	/** @fn GetValue( const char *name, const char *&value ) const
	 * @brief This function gets a value by name. The scope of the value
	 * is the life of the Config object, or until the
	 * next %GetValue() call.
	 *
	 * @param name - name of value to get
	 * @param value - output parameter value
	 * @return status object with the result of the operation
	 */
	Status CALL_TYPE GetValue(const char *name, const char *&value) const;

	/** @fn CheckBoolValue( const char *name, bool defaultValue ) const
	 * @brief This function attempts to get a named value as a boolean. (true/false)
	 * It will return the default value if the field is not set, or the value does
	 * not conform to "true" or "false".
	 *
	 * @param name - name of value to get
	 * @param defaultValue - default value describe above
	 * @return value of named item
	 */
	bool CALL_TYPE CheckBoolValue(const char *name, bool defaultValue) const;

	/** @fn Clear()
	 * @brief This function clears all values
	 *
	 * @return status object with the result of the operation
	 */
	Status CALL_TYPE Clear();

	/** @fn GetFirst( const char *&name, const char *&value ) const
	 * @brief This function gets the first name & value for iteration. The scope
	 * of the name and value are the life of the Config object, or
	 * until the next %GetFirst() call.
	 *
	 * @param name - output parameter name of value
	 * @param value - output parameter value
	 * @return status object with the result of the operation
	 */
	Status CALL_TYPE GetFirst(const char *&name, const char *&value) const;

	/** @fn GetNext( const char *&name, const char *&value ) const
	 * @brief This function gets the next name & value for iteration. The scope
	 * of the name and value are the life of the Config object, or
	 * until the next %GetNext() call.
	 *
	 * @param name - output parameter name of value
	 * @param value - output parameter value
	 * @return status object with the result of the operation
	 */
	Status CALL_TYPE GetNext(const char *&name, const char *&value) const;

	/** @fn ToXML( const char *&xml )
	 * @brief This function will dump this config to XML.
	 * The format is the same as that which is parsed by FromXML().
	 *
	 * @param xml - output parameter: xml string
	 * @return status object with the result of the operation
	 *
	 * @sa FromXML
	 */
	Status CALL_TYPE ToXML(const char *&xml);

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
	Status CALL_TYPE FromXML(const char *xml);

private:
	// not implemented
	Config &operator= (const Config &);

	friend class internal::BaseConfig;

	internal::BaseConfig *ptr;

};

} // namespace gmsec


#endif /* gmsec_Config_h */
