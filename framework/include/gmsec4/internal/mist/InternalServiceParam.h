/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalServiceParam.h
 *
 *  @brief This file contains declarations for Simple Service containers
**/

#ifndef GMSEC_API_INTERNALSIMPLESERVICE_H
#define GMSEC_API_INTERNALSIMPLESERVICE_H


#include <gmsec4/field/Field.h>
#include <gmsec4/util/wdllexp.h>
#include <string>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

class GMSEC_API InternalServiceParam
{
public:

	InternalServiceParam(const char* dName, const Field& dValue);

	InternalServiceParam(const InternalServiceParam& other);

	~InternalServiceParam();

	const char* CALL_TYPE getName() const;

	const Field& CALL_TYPE getValue() const;

	bool CALL_TYPE operator==(const InternalServiceParam& serviceParam) const;

	bool CALL_TYPE operator!=(const InternalServiceParam& serviceParam) const;

private:
	std::string m_name;
	Field*      m_value;
};

}
}
}
}

#endif
