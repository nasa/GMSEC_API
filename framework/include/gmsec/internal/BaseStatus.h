/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Status.h
 *  @brief This file contains the base class for GMSEC Status object.
**/


#ifndef gmsec_internal_BaseStatus_h
#define gmsec_internal_BaseStatus_h

#include <gmsec_defs.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{
namespace internal
{

/** @class Status
 * @brief Returned by the API function calls to provide status feedback.
 * BaseStatus provides an code and message specific to the status type,
 * but independent of source.
 *
 * @sa gmsec_errors.h
 */
class GMSEC_API BaseStatus
{
public:

    /** @fn BaseStatus()
     *  @brief Create a status set to GMSEC_STATUS_NO_ERROR.
     */
    BaseStatus();

    /** @fn BaseStatus(GMSEC_STATUS_CLASS errClass, unsigned int errCode, const char* errStr)
     *  @brief Create a status with specific class, code, and text.
     *
     *  @param errClass - error class
     *  @param errCode  - error code
     *  @param errStr   - error string
     */
    BaseStatus(GMSEC_STATUS_CLASS errClass, unsigned int errCode, const char* errStr);

    /** @fn BaseStatus(const BaseStatus& other)
     *  @brief Copy constructor.
     */
    BaseStatus(const BaseStatus& other);

    /** @fn ~BaseStatus()
     *  @brief Destructor.
     */
    virtual ~BaseStatus();

    /** @fn operator=(const BaseStatus& rhs)
     *  @brief Assignment operator to make a copy of status object.
     */
    BaseStatus& operator=(const BaseStatus& rhs);

    /** @fn setClass(GMSEC_STATUS_CLASS errClass)
     *  @brief This will set the class ID.
     */
    void setClass(GMSEC_STATUS_CLASS errClass);

    /** @fn getClass()
     *  @brief This will return the class ID.
     *  @sa gmsec_errors.h
     */
    GMSEC_STATUS_CLASS getClass() const;

    /** @fn setCode(unsigned int errCode)
     *  @brief This will set the code number for this status.
     *  @sa gmsec_errors.h
     */
    void setCode(unsigned int errCode);

    /** @fn getCode()
     *  @brief This will return the code number for this status.
     *  @sa gmsec_errors.h
     */
    unsigned int getCode() const;

    /** @fn setString(const char* errStr)
     *  @brief This will set the string for this status.
     */
    void setString(const char* errStr);

    /** @fn getString()
     *  @brief This will return the string for this status.
     */
    const char* getString() const;

    /** @fn setCustomCode(GMSEC_I32 code)
     *  @brief This will set the specific code for this status.
     */
    void setCustomCode(GMSEC_I32 code);

    /** @fn getCustomCode()
     *  @brief This will return the specific code for this status.
     */
    GMSEC_I32 getCustomCode() const;

    /** @fn get()
     *  @brief This will return a custom string.
     */
    const char* get();

    /** @fn isError()
     *  @brief This will return true if the status indicates an error; false otherwise.
     */
    bool isError() const;

    /** @fn set(GMSEC_STATUS_CLASS errClass, unsigned int errCode, const char* errStr);
     *  @brief This is a convenience function that will set the class, code, and text.
     */
    void set(GMSEC_STATUS_CLASS errClass, unsigned int errCode, const char* errStr);

    /** @fn reset()
     *  @brief This clears the status to GMSEC_STATUS_NO_ERROR.
     */
    void reset();

private:

    char*              m_str;
    mutable char*      m_outputStr;
    GMSEC_STATUS_CLASS m_class;
    unsigned int       m_code;
    GMSEC_I32          m_customCode;

};

}    // namespace internal
}    // namespace gmsec

#endif
