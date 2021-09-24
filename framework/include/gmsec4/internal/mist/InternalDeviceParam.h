/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalDeviceParam.h
 *
 *  @brief This file contains the declaration for a device parameter class.
**/

#ifndef GMSEC_API_INTERNAL_DEVICE_PARAM_H
#define GMSEC_API_INTERNAL_DEVICE_PARAM_H

#include <gmsec4/field/Field.h>

#include <gmsec4/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api
{
namespace mist
{

class DeviceParam;

namespace internal
{

class GMSEC_API InternalDeviceParam
{
public:

	InternalDeviceParam(const char* name, const char* timestamp, const Field& value);

	InternalDeviceParam(const InternalDeviceParam& deviceParam);

	~InternalDeviceParam();

	const char* CALL_TYPE getName() const;

	const char* CALL_TYPE getTimestamp() const;

	const Field& CALL_TYPE getValue() const;

	bool CALL_TYPE operator==(const InternalDeviceParam& deviceParam) const;

	bool CALL_TYPE operator!=(const InternalDeviceParam& deviceParam) const;

private:

	InternalDeviceParam(); // not implemented

	std::string  m_name;
	std::string  m_timestamp;
	Field*       m_value;

};

}
}
}
}

#endif
