
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */








/** @file status_c.h
 *
 *  @brief This file contains function for the management of status.
 *
 * Example creationg & use
 * @code
 * GMSEC_STATUS_HANDLE result = NULL;
 *
 * CreateStatus(&result);
 *
 * // call a function that returns status
 * CreateConfigParams(&cfg,argc,argv,result);
 * // check for error
 * if( isStatusError(result) )
 * {
 *	// dump class, code, and string
 *	printf("%s\n",GetStatus(result));
 *	return -1;
 * }
 *
 * @endcode
 *
 *
**/

#ifndef gmsec_c_status_c_h
#define gmsec_c_status_c_h

#include <gmsec/util/Deprecated.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/** @fn CreateStatus(GMSEC_STATUS_HANDLE *sts)
	 * @brief This function will create a status that can be used for C API calls.
	 * This status can be reused for multiple calls, but must be cleaned up when
	 * it is no longer needed.
	 *
	 * @param[out] sts - status handle to receive created status
	 *
	 * @sa DestroyStatus()
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE CreateStatus(GMSEC_STATUS_HANDLE *sts);

	/** @fn DestroyStatus(GMSEC_STATUS_HANDLE *sts)
	 * @brief This function will clean up a status and any related memory.
	 *
	 * @param[in] sts - status handle to clean up
	 *
	 * @sa CreateStatus()
	 */
	GMSEC_DEPRECATED GMSEC_API void CALL_TYPE DestroyStatus(GMSEC_STATUS_HANDLE *sts);

	/** @fn GetStatus(GMSEC_STATUS_HANDLE sts)
	 * @brief Get the complete status string from this status.
	 * This string will be the form: "[class,code] error string"
	 *
	 * @param[out] sts - status to get string from
	 * @return error string
	 */
	GMSEC_DEPRECATED GMSEC_API const char * CALL_TYPE GetStatus(GMSEC_STATUS_HANDLE sts);

	/** @fn GetStatusString(GMSEC_STATUS_HANDLE sts)
	 * @brief Get the error string from the status.
	 *
	 * @param[in] sts - status to get string from
	 * @return error string
	 */
	GMSEC_DEPRECATED GMSEC_API const char * CALL_TYPE GetStatusString(GMSEC_STATUS_HANDLE sts);

	/** @fn GetStatusClass(GMSEC_STATUS_HANDLE sts)
	 * @brief Get "class" code of this status. This code will indicate which layer, or
	 * module is reporting the error.
	 *
	 * @param[in] sts - status handle
	 * @return GMSEC_STATUS_CLASS code
	 *
	 * @sa gmsec_errors.h
	 */
	GMSEC_DEPRECATED GMSEC_API GMSEC_STATUS_CLASS CALL_TYPE GetStatusClass(GMSEC_STATUS_HANDLE sts);

	/** @fn GetStatusCode(GMSEC_STATUS_HANDLE sts)
	 * @brief Get the error code of this status. This code will indicate the error
	 * that is being reported.
	 *
	 * @param[in] sts - status handle
	 * @return error code
	 *
	 * @sa gmsec_errors.h
	 */
	GMSEC_DEPRECATED GMSEC_API unsigned int CALL_TYPE GetStatusCode(GMSEC_STATUS_HANDLE sts);

	/** @fn GetStatusCustomCode(GMSEC_STATUS_HANDLE sts)
	 * @brief This will return the custom error code number of this status object.
	 *
	 * @param[in] sts - status handle
	 * @return error code
	 *
	 * @sa gmsec_errors.h
	 */
	GMSEC_DEPRECATED GMSEC_API int CALL_TYPE gmsec_GetStatusCustomCode(GMSEC_STATUS_HANDLE sts);

	/** @fn isStatusError(GMSEC_STATUS_HANDLE sts)
	 * @brief This will return 1 if the status is indicating an error, 0 otherwise.
	 *  Returns 0 if the Handle is NULL
	 *
	 * @param[in] sts - status handle
	 * @return 1 if there is an error, 0 otherwise
	 */
	GMSEC_DEPRECATED GMSEC_API int CALL_TYPE isStatusError(GMSEC_STATUS_HANDLE sts);

#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_c_status_c_h */
