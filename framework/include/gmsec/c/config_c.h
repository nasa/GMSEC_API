
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file config_c.h
 *
 *  @brief This file contains function for handling configurations for use in the creation of
 *	connections and messages.
 *
 *	Example construction & use:
 *	@code
 * GMSEC_STATUS_HANDLE result = NULL;
 * GMSEC_CONFIG_HANDLE cfg = NULL;
 *
 * // create status to get return results
 * CreateStatus(&result);
 * // create a config from main() arguments
 * CreateConfigParams(&cfg,argc,argv, result);
 *
 * if( isStatusError(result) ) {
 *	printf("%s\n",GetStatus(result));
 *	return -1;
 * }
 *
 * const char *name = NULL;
 * const char *value = NULL;
 *
 * // read all config values & print
 * ConfigGetFirst(cfg, &name, &value, result);
 * while(!isStatusError(result)) {
 *	printf("%s,%s\n",name,value);
 *	ConfigGetNext(cfg, &name, &value, result);
 * }
 * @endcode
 *
 * @sa CreateConnectionForConfig @n
 *		MsgSetConfig @n
 *		LookupConfigFileConfig @n
 *
**/

#ifndef gmsec_c_config_c_h
#define gmsec_c_config_c_h

#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/** @fn CreateConfig(GMSEC_CONFIG_HANDLE *cfg, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This create a new config and assign it into the empty GMSEC_CONFIG_HANDLE
	 * that has been passed in. The created status MUST be cleaned up with DestroyConfig()
	 * when no longer needed.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] cfg - handle to be assigned the created config
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa DestroyConfig()
	 */
	GMSEC_DEPRECATED GMSEC_API void CreateConfig(GMSEC_CONFIG_HANDLE *cfg, GMSEC_STATUS_HANDLE status);


	/** @fn CreateConfigParams(GMSEC_CONFIG_HANDLE *cfg, int argc, char *argv[], GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This constructor will parse command line parameters and add
	 * values for any param that is of the form \<name>=\<value>
	 *
	 * @code gmsub.exe connectionType=gmsec_mb hostname=localhost port=9000 @endcode
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] cfg - handle to be assigned the created config
	 * @param[in]  argc - number of args (from main)
	 * @param[in]  argv - array of args (from main)
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa DestroyConfig()
	 */
	GMSEC_DEPRECATED GMSEC_API void CreateConfigParams(GMSEC_CONFIG_HANDLE *cfg, int argc, char *argv[], GMSEC_STATUS_HANDLE status);


	/** @fn DestroyConfig(GMSEC_CONFIG_HANDLE *cfg, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This function cleans up memory associated with a config created with CreateConfig() or CreateConfigParams()
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  cfg - hanble of config to clean up
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa CreateConfig() @n
	 *	   CreateConfigParams()
	 */
	GMSEC_DEPRECATED GMSEC_API void DestroyConfig(GMSEC_CONFIG_HANDLE *cfg, GMSEC_STATUS_HANDLE status);


	/** @fn ConfigAddValue(GMSEC_CONFIG_HANDLE cfg, const char *name, const char *value, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This function adds a value to the config by name
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in,out] cfg - on input the handle of the config to be modified.  On output the handle of the config with the new name/value
	 * @param[in]     name - name of field
	 * @param[in]     value - value of field
	 * @param[out]    status - out parameter operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigAddValue(GMSEC_CONFIG_HANDLE cfg, const char *name, const char *value, GMSEC_STATUS_HANDLE status);


	/** @fn ConfigClearValue(GMSEC_CONFIG_HANDLE cfg, const char *name, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This function clears a value by name
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in,out] cfg - on input the handle of the config to modify.  On output the handle of the config without the name field
	 * @param[in]     name - name of value to clear
	 * @param[out]    status - out parameter operation result status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigClearValue(GMSEC_CONFIG_HANDLE cfg, const char *name, GMSEC_STATUS_HANDLE status);


	/** @fn ConfigGetValue(GMSEC_CONFIG_HANDLE cfg, const char *name, const char **value, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This function gets the current value in a config by name. The client program need not clean up the memory
	 * associated with the returned value, although it must be copied to be stored.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  cfg - handle of config to retrieve from
	 * @param[in]  name - name of field to retrieve
	 * @param[out] value - out parameter value of field (if available)
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigGetValue(GMSEC_CONFIG_HANDLE cfg, const char *name, const char **value, GMSEC_STATUS_HANDLE status);


	/** @fn ConfigClear(GMSEC_CONFIG_HANDLE cfg, GMSEC_STATUS_HANDLE status)
	 * @brief This function clears all values from a config
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  cfg - handle of config to clear
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigClear(GMSEC_CONFIG_HANDLE cfg, GMSEC_STATUS_HANDLE status);


	/** @fn ConfigGetFirst(GMSEC_CONFIG_HANDLE cfg, const char **name, const char **value, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This function gets the first name & value for iteration. The scope
	 * of the name and value are the life of the gmsec::Config object, or
	 * until the next %ConfigGetFirst() call.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  cfg - handle of config
	 * @param[out] name - out parameter field name
	 * @param[out] value - out parameter field value
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigGetFirst(GMSEC_CONFIG_HANDLE cfg, const char **name, const char **value, GMSEC_STATUS_HANDLE status);


	/** @fn ConfigGetNext(GMSEC_CONFIG_HANDLE cfg, const char **name, const char **value, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This function gets the next name & value for iteration. The scope
	 * of the name and value are the life of the gmsec::Config object, or
	 * until the next %GetNext() call.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  cfg - handle to config
	 * @param[out] name - out parameter field name
	 * @param[out] value - out parameter field value
	 * @param[out] status - out parameter operation status
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigGetNext(GMSEC_CONFIG_HANDLE cfg, const char **name, const char **value, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigToXML(GMSEC_CONFIG_HANDLE cfg, const char **xml, GMSEC_STATUS_HANDLE status )
	 *
	 * @brief This function dumps a config to an xml string
	 * The format is the same as that which is parsed by ConfigFromXML().
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[in]  cfg - handle to config
	 * @param[out] xml - out parameter xml string
	 * @param[out] status - out parameter operation status
	 *
	 * @sa ConfigFromXML
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigToXML(GMSEC_CONFIG_HANDLE cfg, const char **xml, GMSEC_STATUS_HANDLE status);

	/** @fn ConfigFromXML(GMSEC_CONFIG_HANDLE cfg, const char *xml, GMSEC_STATUS_HANDLE status )
	 *
	 * @brief This function takes a valid xml config tag and parses it into the passed in object.
	 * The format is the same as that which is generated by ConfigToXML().
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
	 * passed in as the status parameter in order to recieve any resulting status.
	 *
	 * @param[out] cfg - handle to config
	 * @param[in]  xml - xml string to parse
	 * @param[out] status - out parameter operation status
	 *
	 * @sa ConfigToXML
	 */
	GMSEC_DEPRECATED GMSEC_API void ConfigFromXML(GMSEC_CONFIG_HANDLE cfg, const char *xml, GMSEC_STATUS_HANDLE status);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_c_config_c_h */

