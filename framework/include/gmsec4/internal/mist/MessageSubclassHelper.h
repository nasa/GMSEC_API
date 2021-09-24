/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MessageSubclassHelper.h
 *
 * @brief This file a number of static functions to help MIST in handling
 * its collection of subclasses of message
 */

#ifndef GMSEC_API_INTERNAL_MESSAGE_SUBCLASS_HELPER_H
#define GMSEC_API_INTERNAL_MESSAGE_SUBCLASS_HELPER_H

#include <gmsec4/field/Field.h>

#include <gmsec4/internal/InternalMessage.h>

#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>

#include <string>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

class GMSEC_API MessageSubclassHelper
{
public:
	/**
	 * @fn std::string extractStringField(const char* fieldName, const char* loggableInvocationPoint, const gmsec::api::internal::InternalMessage& msg)
	 *
	 * @brief Extracts a string from given message, formatting and throwing an approriate exception
	 * if the supplied message does not have a valid field.
	 *
	 * @param fieldName - The name of the field to look for
	 * @param loggableInvocationPoint - The string descriptor of this functions invocation point 
	 * for exception formatting purposes
	 * @param msg - The message to extract data from
	 */
	static std::string CALL_TYPE extractStringField(const char* fieldName,
                                                    const char* loggableInvocationPoint,
                                                    const gmsec::api::internal::InternalMessage& msg);


	/**
	 * @fn GMSEC_I16 extractI16Field(const char* fieldName, const char* loggableInvocationPoint, const gmsec::api::internal::InternalMessage& msg)
	 *
	 * @brief Extracts a string from given message, formatting and throwing an approriate exception
	 * if the supplied message does not have a valid field.
	 *
	 * @param fieldName - The name of the field to look for
	 * @param loggableInvocationPoint - The string descriptor of this functions invocation point 
	 * for exception formatting purposes
	 * @param msg - The message to extract data from
	 */
	static GMSEC_I16 CALL_TYPE extractI16Field(const char* fieldName,
                                               const char* loggableInvocationPoint,
                                               const gmsec::api::internal::InternalMessage& msg);

	/**
	 * @fn GMSEC_I16 extractU16Field(const char* fieldName, const char* loggableInvocationPoint, const gmsec::api::internal::InternalMessage& msg)
	 *
	 * @brief Extracts a string from given message, formatting and throwing an approriate exception
	 * if the supplied message does not have a valid field.
	 *
	 * @param fieldName - The name of the field to look for
	 * @param loggableInvocationPoint - The string descriptor of this functions invocation point 
	 * for exception formatting purposes
	 * @param msg - The message to extract data from
	 */
	static GMSEC_U16 CALL_TYPE extractU16Field(const char* fieldName,
                                               const char* loggableInvocationPoint,
                                               const gmsec::api::internal::InternalMessage& msg);

	/**
	 * @fn const Field extractField(const char* fieldName, const char* loggableInvocationPoint, const gmsec::api::internal::InternalMessage& msg)
	 *
	 * @brief Extracts a field from given message, formatting and throwing an approriate exception
	 * if the supplied message does not have a valid field. This function will not return null.
	 *
	 * @param fieldName - The name of the field to look for
	 * @param loggableInvocationPoint - The string descriptor of this functions invocation point 
	 * for exception formatting purposes
	 * @param msg - The message to extract data from
	 */
	static const Field* CALL_TYPE extractField(const char* fieldName,
                                               const char* loggableInvocationPoint,
                                               const gmsec::api::internal::InternalMessage& msg);


	/**
	 * @fn bool getOptionalString(const char* fieldName, const gmsec::api::internal::InternalMessage& msg, std::string& target)
	 *
	 * @brief Extracts a string from given message, returning true or false
	 *
	 * @param fieldName - The name of the field to look for
	 * @param msg - The message to extract data from
	 * @target - The target string object into which the contents will be placed
	 */
	static bool CALL_TYPE getOptionalString(const char* fieldName,
                                            const gmsec::api::internal::InternalMessage& msg,
                                            std::string& target);


	/**
	 * @fn bool getOptionalI16(const char* fieldName, const gmsec::api::internal::InternalMessage& msg, GMSEC_I16& target)
	 *
	 * @brief Extracts an I16 from given message, returning true or false
	 *
	 * @param fieldName - The name of the field to look for
	 * @param msg - The message to extract data from
	 * @target - The target I16 variable into which the contents will be placed
	 */
	static bool CALL_TYPE getOptionalI16(const char* fieldName,
                                         const gmsec::api::internal::InternalMessage& msg,
                                         GMSEC_I16& target);

	/**
	 * @fn bool getOptionalI32(const char* fieldName, const gmsec::api::internal::InternalMessage& msg, GMSEC_I32& target)
	 *
	 * @brief Extracts an I32 from given message, returning true or false
	 *
	 * @param fieldName - The name of the field to look for
	 * @param msg - The message to extract data from
	 * @target - The target I32 variable into which the contents will be placed
	 */
	static bool CALL_TYPE getOptionalI32(const char* fieldName,
                                         const gmsec::api::internal::InternalMessage& msg,
                                         GMSEC_I32& target);

	/**
	 * @fn bool getOptionalU32(const char* fieldName, const gmsec::api::internal::InternalMessage& msg, GMSEC_U32& target)
	 *
	 * @brief Extracts an U32 from given message, returning true or false
	 *
	 * @param fieldName - The name of the field to look for
	 * @param msg - The message to extract data from
	 * @target - The target U32 variable into which the contents will be placed
	 */
	static bool CALL_TYPE getOptionalU32(const char* fieldName,
                                         const gmsec::api::internal::InternalMessage& msg,
                                         GMSEC_U32& target);

	/**
	 * @fn bool getOptionalBool(const char* fieldName, const gmsec::api::internal::InternalMessage& msg, bool& target)
	 *
	 * @brief Extracts a boolean field from given message, returning true or false
	 *
	 * @param fieldName - The name of the field to look for
	 * @param msg - The message to extract data from
	 * @target - The target boolean variable into which the contents will be placed
	 */
	static bool CALL_TYPE getOptionalBool(const char* fieldName,
                                         const gmsec::api::internal::InternalMessage& msg,
                                         bool& target);

	/**
	 * @fn void checkStringField(const char* fieldName, const char* loggableInvocationPoint, 
	 *	                         const char* expectedValue, const gmsec::api::internal::InternalMessage& msg)
	 *
	 * @brief Extracts a string from given message and checks the value, throws an exception if there is
	 * no match
	 *
	 * @param fieldName - The name of the field to look for
	 * @param loggableInvocationPoint - The string descriptor of this functions invocation point 
	 * for exception formatting purposes
	 * @param expectedValue - The desired value
	 * @param msg - The message to extract data from
	 */
	static void CALL_TYPE checkStringField(const char* fieldName,
                                           const char* loggableInvocationPoint,
                                           const char* expectedValue,
                                           const gmsec::api::internal::InternalMessage& msg);


	/**
	 * @fn void checkFloatField(const char* fieldName, const char* loggableInvocationPoint,
	 *	                        GMSEC_F32 expectedValue, const gmsec::api::internal::InternalMessage& msg)
	 *
	 * @brief Extracts a float from given message and checks the value, throws an exception if there is
	 * no match
	 *
	 * @param fieldName - The name of the field to look for
	 * @param loggableInvocationPoint - The string descriptor of this functions invocation point 
	 * for exception formatting purposes
	 * @param expectedValue - The desired value
	 * @param msg - The message to extract data from
	 */
	static void CALL_TYPE checkFloatField(const char* fieldName,
                                          const char* loggableInvocationPoint,
                                          GMSEC_F32 expectedValue,
                                          const gmsec::api::internal::InternalMessage& msg);


	/**
	 * @fn addRenamedFieldToMessage(const Field& valueField, gmsec::api::internal::InternalMessage& msg,
	 *                              const char* newFieldName)
	 *
	 * @brief Adds a new field to a message with the new name specified and the value
	 * of the specified field
	 *
	 * @param valueField - The old field from which a value will be taken
	 * @param msg - The message to which the new field will be added
	 * @param newFieldName - The name of the new field
	 */
	static void CALL_TYPE addRenamedFieldToMessage(const Field& valueField,
                                                   gmsec::api::internal::InternalMessage& msg,
                                                   const char* newFieldName);
};

} //namespace internal
} //namespace mist
} //namespace api
} //namespace gmsec

#endif
