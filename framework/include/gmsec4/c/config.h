/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**
 * @file config.h
 *
 * @brief This file contains functions for handling configurations for use in the creation of
 * connections and messages.
 *
 * Example construction & use:
 * @code
 * #include <gmsec4_c.h>
 *
 * int main(int argc, char* argv[])
 * {
 *     // create status
 *     GMSEC_Status status = statusCreate();
 *
 *     // create a config from main() arguments
 *     GMSEC_Config cfg = configCreateWithArgs(argc, argv);
 *
 *     const char* name  = NULL;
 *     const char* value = NULL;
 *
 *     // read all config values & print
 *     configGetFirst(cfg, &name, &value, status);
 *
 *     while (statusIsError(status) == GMSEC_FALSE)
 *     {
 *         printf("%s,%s\n", name, value);
 *
 *         configGetNext(cfg, &name, &value, status);
 *     }
 *
 *     // cleanup
 *     configDestroy(&cfg);
 *     statusDestroy(&status);
 *
 *     return 0;
 * }
 * @endcode
 *
 * @sa connectionCreate
 * @sa messageCreate
 * @sa configFileGetConfig
 */


#ifndef GMSEC_API_C_CONFIG_H
#define GMSEC_API_C_CONFIG_H

#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>
#include <gmsec4/util/Deprecated.h>


#ifdef __cplusplus
extern "C"
{
#endif


	/**
	 * @fn GMSEC_Config configCreate(void)
	 *
	 * @brief This create a new config and assign it into the empty GMSEC_Config
	 * that has been passed in.
	 *
	 * @return A handle to the create config object.
	 *
	 * @sa configDestroy()
	 */
	GMSEC_API GMSEC_Config configCreate(void);


	/**
	 * @fn GMSEC_Config configCreateWithArgs(int argc, char *argv[])
	 *
	 * @brief This constructor will parse command line parameters and add
	 * values for any param that is of the form \<name>=\<value>
	 *
	 * @param[in]  argc - number of args (from main)
	 * @param[in]  argv - array of args (from main)
	 *
	 * @return A handle to the created config object.
	 *
	 * @sa configDestroy()
	 */
	GMSEC_API GMSEC_Config configCreateWithArgs(int argc, char* argv[]);


	/**
	 * @fn GMSEC_Config configCreateUsingXML(const char* xml, GMSEC_Status status)
	 *
	 * @brief This constructor will create a config and parse values from the passed in XML string.
	 * The XML format is the same as used by the configFromXML() and configToXML() functions.
	 *
	 * @param[in]  xml    - the XML string containing configuration information
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to the created config object, or NULL if an error occurs.  If the latter,
	 * check the Status object.
	 *
	 * @sa configDestroy()
	 *
	 * @note This function has been deprecated; use configCreateUsingData() instead.
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_Config configCreateUsingXML(const char* xml, GMSEC_Status status);


	/**
	 * @fn GMSEC_Config configCreateUsingData(const char* data, GMSEC_Status status)
	 *
	 * @brief This constructor will create a config object by parsing key/value pairs from the
	 * given data string.  The data string can contain either XML or JSON formatted configuration
	 * data, or contain raw data consisting of white-space separated key=value pairs.
	 *
	 * @param[in]  data   - the data string containing configuration information
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to the created config object, or NULL if an error occurs.  If the latter,
	 * check the Status object.
	 *
	 * @sa configToXML()
	 * @sa configDestroy()
	 */
	GMSEC_API GMSEC_Config configCreateUsingData(const char* data, GMSEC_Status status);


	/**
	 * @fn GMSEC_Config configCreateCopy(const GMSEC_Config other, GMSEC_Status status)
	 *
	 * @brief This constructor will create a copy of the given Config object.
	 *
	 * @note A status object MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  other  - handle to the config object to copy
	 * @param[out] status - out parameter operation result status
	 *
	 * @return A handle to the created config object.
	 *
	 * @sa configDestroy()
	 */
	GMSEC_API GMSEC_Config configCreateCopy(const GMSEC_Config other, GMSEC_Status status);


	/**
	 * @fn void configDestroy(GMSEC_Config* cfg)
	 *
	 * @brief This function cleans up memory associated with a config.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in,out] cfg - handle of config to clean up
	 *
	 * @sa configCreate()
	 * @sa configCreateWithArgs()
	 * @sa configCreateCopy()
	 */
	GMSEC_API void configDestroy(GMSEC_Config* cfg);


	/**
	 * @fn void configAddValue(GMSEC_Config cfg, const char* name, const char* value, GMSEC_Status status)
	 *
	 * @brief This function adds a value to the config by name
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with statusCreate() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfg    - on input the handle of the config to be modified.  On output the handle of the config with the new name/value
	 * @param[in]  name   - name of field
	 * @param[in]  value  - value of field
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void configAddValue(GMSEC_Config cfg, const char *name, const char *value, GMSEC_Status status);


	/**
	 * @fn void configClearValue(GMSEC_Config cfg, const char* name, GMSEC_Status status)
	 *
	 * @brief This function clears a value by name
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfg    - on input the handle of the config to modify.  On output the handle of the config without the name field
	 * @param[in]  name   - name of value to clear
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void configClearValue(GMSEC_Config cfg, const char* name, GMSEC_Status status);


	/**
	 * @fn const char* configGetValue(const GMSEC_Config cfg, const char* name, GMSEC_Status status)
	 *
	 * @brief This function gets the current value in a config by name. The client program need not clean up the memory
	 * associated with the returned value, although it must be copied to be stored.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfg    - handle of config to retrieve from
	 * @param[in]  name   - name of field to retrieve
	 * @param[out] status - out parameter operation status
	 *
	 * @return Value of field (if available), or otherwise NULL
	 */
	GMSEC_API const char* configGetValue(const GMSEC_Config cfg, const char* name, GMSEC_Status status);

	/**
	 * @fn const char* configGetValueWithDefault(const GMSEC_Config cfg, const char* name, const char* defaultValue, GMSEC_Status status)
	 *
	 * @brief This function gets the current value in a config by name. The client program need not clean up the memory
	 * associated with the returned value, although it must be copied to be stored.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfg    		- handle of config to retrieve from
	 * @param[in]  name   		- name of field to retrieve
	 * @param[in]  defaultValue - value to return if name does not exist
	 * @param[out] status 		- out parameter operation status
	 *
	 * @return Value of field (if available), or otherwise the given default value
	 */
	GMSEC_API const char* configGetValueWithDefault(const GMSEC_Config cfg, const char* name, const char* defaultValue, GMSEC_Status status);

	/**
	 * @fn GMSEC_BOOL configGetBooleanValue(const GMSEC_Config cfg, const char* name, GMSEC_Status status);
	 *
	 * @brief This function attempts to get the boolean representation of configuration element.
	 * An error status is returned if the element does not exist or there is no valid boolean interpretion of the value.
	 *
	 * @param[in]  cfg    - handle of config to retrieve from
	 * @param[in]  name   - name of field to retrieve
	 * @param[out] status - out parameter operation status
	 *
	 * @return A boolean value; verify whether it is acceptable to trust by examining the Status.
	 */
	GMSEC_API GMSEC_BOOL configGetBooleanValue(const GMSEC_Config cfg, const char* name, GMSEC_Status status);


	/**
	 * @fn GMSEC_BOOL configGetBooleanValueWithDefault(const GMSEC_Config cfg, const char* name, GMSEC_BOOL defaultValue, GMSEC_Status status);
	 *
	 * @brief This function attempts to get the boolean representation of configuration element.  If the configuration item cannot
	 * be found, or cannot be interpreted as a boolean, then the given default value is returned.
	 *
	 * @param[in]  cfg     - handle of config to retrieve from
	 * @param[in]  name    - name of field to retrieve
	 * @param[in]  default - default value to return if the configuration field is not found
	 * @param[out] status  - out parameter operation status
	 *
	 * @return A boolean value; verify whether it is acceptable to trust by examining the Status.
	 */
	GMSEC_API GMSEC_BOOL configGetBooleanValueWithDefault(const GMSEC_Config cfg, const char* name, GMSEC_BOOL defaultValue, GMSEC_Status status);


	/**
	 * @fn int configGetIntegerValue(const GMSEC_Config cfg, const char* name, GMSEC_Status status);
	 *
	 * @brief This function attempts to get the integer representation of configuration element.
	 * An error status is returned if the element does not exist or there is no valid integer interpretion of the value.
	 *
	 * @param[in]  cfg    - handle of config to retrieve from
	 * @param[in]  name   - name of field to retrieve
	 * @param[out] status - out parameter operation status
	 *
	 * @return An integer value; verify whether it is acceptable to trust by examining the Status.
	 */
	GMSEC_API int configGetIntegerValue(const GMSEC_Config cfg, const char* name, GMSEC_Status status);


	/**
	 * @fn int configGetIntegerValueWithDefault(const GMSEC_Config cfg, const char* name, int defaultValue, GMSEC_Status status);
	 *
	 * @brief This function attempts to get the integer representation of configuration element.  If the configuration item cannot
	 * be found, or cannot be interpreted as an integer, then the given default value is returned.
	 *
	 * @param[in]  cfg     - handle of config to retrieve from
	 * @param[in]  name    - name of field to retrieve
	 * @param[in]  default - default value to return if the configuration field is not found
	 * @param[out] status  - out parameter operation status
	 *
	 * @return An integer value; verify whether it is acceptable to trust by examining the Status.
	 */
	GMSEC_API int configGetIntegerValueWithDefault(const GMSEC_Config cfg, const char* name, int defaultValue, GMSEC_Status status);


	/**
	 * @fn double configGetDoubleValue(const GMSEC_Config cfg, const char* name, GMSEC_Status status);
	 *
	 * @brief This function attempts to get the double representation of configuration element.
	 * An error status is returned if the element does not exist or there is no valid double interpretion of the value.
	 *
	 * @param[in]  cfg    - handle of config to retrieve from
	 * @param[in]  name   - name of field to retrieve
	 * @param[out] status - out parameter operation status
	 *
	 * @return An double value; verify whether it is acceptable to trust by examining the Status.
	 */
	GMSEC_API double configGetDoubleValue(const GMSEC_Config cfg, const char* name, GMSEC_Status status);


	/**
	 * @fn double configGetDoubleValueWithDefault(const GMSEC_Config cfg, const char* name, double defaultValue, GMSEC_Status status);
	 *
	 * @brief This function attempts to get the double representation of configuration element.  If the configuration item cannot
	 * be found, or cannot be interpreted as a double, then the given default value is returned.
	 *
	 * @param[in]  cfg     - handle of config to retrieve from
	 * @param[in]  name    - name of field to retrieve
	 * @param[in]  default - default value to return if the configuration field is not found
	 * @param[out] status  - out parameter operation status
	 *
	 * @return A double value; verify whether it is acceptable to trust by examining the Status.
	 */
	GMSEC_API double configGetDoubleValueWithDefault(const GMSEC_Config cfg, const char* name, double defaultValue, GMSEC_Status status);


	/** @fn void configClear(GMSEC_Config cfg, GMSEC_Status status)
	 * @brief This function clears all values from a config
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfg    - handle of config to clear
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_API void configClear(GMSEC_Config cfg, GMSEC_Status status);


	/** @fn void configGetFirst(const GMSEC_Config cfg, const char** name, const char** value, GMSEC_Status status)
	 *
	 * @brief This function gets the first name & value for iteration. The scope of the name and value are the life
	 * of the gmsec::Config object, or until the next %ConfigGetFirst() call.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfg    - handle of config
	 * @param[out] name   - out parameter field name
	 * @param[out] value  - out parameter field value
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_API void configGetFirst(const GMSEC_Config cfg, const char** name, const char** value, GMSEC_Status status);


	/** @fn void configGetNext(const GMSEC_Config cfg, const char** name, const char** value, GMSEC_Status status)
	 *
	 * @brief This function gets the next name & value for iteration. The scope of the name and value are the life
	 * of the gmsec::Config object, or until the next %GetNext() call.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfg    - handle to config
	 * @param[out] name   - out parameter field name
	 * @param[out] value  - out parameter field value
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_API void configGetNext(const GMSEC_Config cfg, const char** name, const char** value, GMSEC_Status status);


	/**
	 * @fn configMerge(GMSEC_Config config, const GMSEC_Config other, GMSEC_BOOL overwriteExisting, GMSEC_Status status)
	 *
	 * @brief Merge the given %Config object with the current %Config object.
	 *
	 * @param[in]  cfg    - handle to config
	 * @param[in]  other  - the %Config object from which to read name/value pairs.
	 * @param[in]  overwriteExisting - indicates whether to overwrite any existing name/value
	 * pairs within the working %Config object.
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_API void configMerge(GMSEC_Config config, const GMSEC_Config other, GMSEC_BOOL overwriteExisting, GMSEC_Status status);


	/** @fn const char* configToXML(const GMSEC_Config cfg, GMSEC_Status status)
	 *
	 * @brief This function dumps a config to an XML string.
	 * The format is the same as that which is parsed by configFromXML().
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfg    - handle to config
	 * @param[out] status - out parameter operation status
	 *
	 * @return An XML string that represents the current Config object.
	 *
	 * @sa configFromXML
	 */
	GMSEC_API const char* configToXML(const GMSEC_Config cfg, GMSEC_Status status);


	/** @fn void configFromXML(GMSEC_Config cfg, const char* xml, GMSEC_Status status)
	 *
	 * @brief This function takes a valid xml config tag and parses it into the passed in object.
	 * The format is the same as that which is generated by configToXML().
	 *
	 * Example xml string:
	 * @code
	 * <CONFIG>
	 *    <PARAMETER NAME="connectiontype">gmsec_ss</PARAMETER>
	 *	  <PARAMETER NAME="server">tcp:10.1.2.159</PARAMETER>
	 *	  <PARAMETER NAME="tracking">true</PARAMETER>
	 *	  <PARAMETER NAME="isthreaded">true</PARAMETER>
	 * </CONFIG>
	 * @endcode
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfg    - handle to config
	 * @param[in]  xml    - xml string to parse
	 * @param[out] status - out parameter operation status
	 *
	 * @sa configToXML
	 */
	GMSEC_API void configFromXML(GMSEC_Config cfg, const char* xml, GMSEC_Status status);


	/** @fn const char* configToJSON(const GMSEC_Config cfg, GMSEC_Status status)
	 *
	 * @brief This function dumps a config to an JSON string.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfg    - handle to config
	 * @param[out] status - out parameter operation status
	 *
	 * @return An JSON string that represents the current Config object.
	 *
	 * @sa configFromXML
	 */
	GMSEC_API const char* configToJSON(const GMSEC_Config cfg, GMSEC_Status status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif
