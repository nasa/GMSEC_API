/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ServiceParam.h
 *
 *  @brief This file contains declarations for Simple Service containers
**/

#ifndef GMSEC_API_SIMPLESERVICE_H
#define GMSEC_API_SIMPLESERVICE_H

#include <gmsec4/field/Field.h>
#include <gmsec4/util/wdllexp.h>

namespace gmsec
{
namespace api
{
namespace mist
{

namespace internal
{
	class InternalServiceParam;
}

/**
 * @class ServiceParam
 *
 * @brief This class is a lightweight container for holding information
 * on a Simple Service parameter, and used to generate GMSEC Simple Service
 * messages by the ConnectionManager class
 *
 * @sa ConnectionManager @n
 *		
*/
class GMSEC_API ServiceParam
{
  public:

	/**
	 * @fn ServiceParam(const char* dName, const Field& dValue)
	 *
	 * @brief Constructor - Initializes the ServiceParam object with 
	 * a name and value
	 *
	 * @param dName - The name of the ServiceParam
	 * @param dValue - The value of the parameter
	 */
	ServiceParam(const char* dName, const Field& dValue);

	/**
	 * @fn ServiceParam(const ServiceParam& other)
	 *
	 * @brief Constructor - Initializes the ServiceParam object from another
	 * ServiceParam
	 *
	 * @param other - The other ServiceParam to inherit values from
	 */
	ServiceParam(const ServiceParam& other);

	/**
	 * @fn ~ServiceParam
	 *
	 * @brief Destructor
	 *
	 */
	~ServiceParam();

	/**
	 * @fn const char* getName() const
	 *
	 * @brief Retrieves the name of the ServiceParam
	 *
	 * @return The name of the ServiceParam
	 */
	const char* CALL_TYPE getName() const;

	/**
	 * @fn const Field& getValue() const
	 *
	 * @brief Retrieves the value of the ServiceParam
	 *
	 * @return The value of the ServiceParam
	 */
	const Field& CALL_TYPE getValue() const;

	/**
	 * @fn bool operator==(const ServiceParam& serviceParam) const
	 *
	 * @brief Equality operator for ServiceParam
	 *
	 */
	bool CALL_TYPE operator==(const ServiceParam& serviceParam) const;

	/**
	 * @fn bool operator!=(const ServiceParam& serviceParam) const
	 *
	 * @brief Non-equality operator for ServiceParam
	 *
	 */
	bool CALL_TYPE operator!=(const ServiceParam& serviceParam) const;

  private:

	gmsec::api::mist::internal::InternalServiceParam* m_iServiceParam;

};

}
}
}

#endif
