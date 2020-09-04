/*
 * Copyright 2007-2019 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file CustomSpecification.h
 *
 * @brief Internal custom extension of Specification that is used in support of the C binding.
 * 
 */

#ifndef GMSEC_CUSTOM_SPECIFICATION_H
#define GMSEC_CUSTOM_SPECIFICATION_H

#include <gmsec4/mist/Specification.h>

#include <gmsec4/util/wdllexp.h>

#include <gmsec4_defs.h>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Message;

namespace mist
{
namespace internal
{


class GMSEC_API CustomSpecification : public gmsec::api::mist::Specification
{
public:
    CustomSpecification(gmsec::api::mist::Specification* spec, GMSEC_SpecificationValidateMessage* validateMsg);

    virtual void CALL_TYPE validateMessage(const gmsec::api::Message& message);

private:
    GMSEC_SpecificationValidateMessage*  m_validateMsg;
};


}  // end namespace internal
}  // end namespace mist
}  // end namespace api
}  // end namespace gmsec

#endif
