/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CMSDestination.cpp
 *
 *  This file contains the implementation for the CMS Message.
 */

#include "CMSDestination.h"


using namespace gmsec::api;
using namespace gmsec::api::internal;

using namespace gmsec_amqcms;


CMSDestination::CMSDestination(cms::Destination* dest)
	: replyTo(dest)
{
}

CMSDestination::~CMSDestination()
{
	delete replyTo;
}

cms::Destination* CMSDestination::getReplyTo() const
{
	return replyTo;
}

Status CMSDestination::getBoolean(bool& value) const
{
	return Status(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Opaque value cannot be converted to a bool value");
}

Status CMSDestination::getI32(GMSEC_I32& value) const
{
	return Status(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Opaque value cannot be converted to a GMSEC_I32 value");
}

Status CMSDestination::getF64(GMSEC_F64& value) const
{
	return Status(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Opaque value cannot be converted to a GMSEC_F64 value");
}

Status CMSDestination::getString(std::string& value) const
{
	return Status(FIELD_ERROR, FIELD_TYPE_MISMATCH, "Opaque value cannot be converted to an std::string value");
}

Value* CMSDestination::clone() const
{
	if (replyTo)
	{
		return new CMSDestination(replyTo->clone());
	}
	return NULL;
}
