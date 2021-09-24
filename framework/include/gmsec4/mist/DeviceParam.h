/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file DeviceParam.h
 *
 *  @brief This file contains the declaration for a device parameter class.
**/

#ifndef GMSEC_API_DEVICE_PARAM_H
#define GMSEC_API_DEVICE_PARAM_H


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
	class InternalDeviceParam;
}

/**
 * @class DeviceParam
 *
 * @brief This class is a lightweight container for holding information
 * on a DeviceParam, and used to generate GMSEC Device messages by the 
 * ConnectionManager and Device classes
 *
 * @sa Device @n
 *		ConnectionManager
*/
class GMSEC_API DeviceParam
{
  public:

	/**
	 * @fn DeviceParam(const char* name, const char* timestamp, const Field& value)
	 *
	 * @brief Constructor - Initializes the DeviceParam object with 
	 * a name, timestamp, and value
	 *
	 * @param name - The name of the DeviceParam
	 * @param timestamp - The time at which the sampled value was taken
	 * @param value - The value of the parameter
	 */
	DeviceParam(const char* name, const char* timestamp, const Field& value);

	/**
	 * @fn DeviceParam(const DeviceParam& deviceParam)
	 *
	 * @brief Constructor - Initializes the DeviceParam object from another
	 * DeviceParam
	 *
	 * @param deviceParam - The other DeviceParam to inherit values from
	 */
	DeviceParam(const DeviceParam& deviceParam);

	/**
	 * @fn ~DeviceParam
	 *
	 * @brief Destructor
	 *
	 */
	~DeviceParam();

	/**
	 * @fn const char* getName() const
	 *
	 * @brief Retrieves the name of the DeviceParam
	 *
	 * @return The name of the DeviceParam
	 */
	const char* CALL_TYPE getName() const;

	/**
	 * @fn const char* getTimestamp() const
	 *
	 * @brief Retrieves the timestamp of the DeviceParam
	 *
	 * @return The timestamp of the DeviceParam
	 */
	const char* CALL_TYPE getTimestamp() const;

	/**
	 * @fn const Field& getValue() const
	 *
	 * @brief Retrieves the value of the DeviceParam
	 *
	 * @return The value of the DeviceParam
	 */
	const Field& CALL_TYPE getValue() const;

	/**
	 * @fn bool operator==(const DeviceParam& deviceParam) const
	 *
	 * @brief Equality operator for DeviceParam
	 *
	 */
	bool CALL_TYPE operator==(const DeviceParam& deviceParam) const;

	/**
	 * @fn bool operator!=(const DeviceParam& deviceParam) const
	 *
	 * @brief Non-equality operator for DeviceParam
	 *
	 */
	bool CALL_TYPE operator!=(const DeviceParam& deviceParam) const;

  private:

	DeviceParam(); // not implemented

	gmsec::api::mist::internal::InternalDeviceParam* m_iDeviceParam;

};

}
}
}

#endif
