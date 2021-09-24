/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file CMSMessage.h
 *  Holds the declaration of the Active MQ subclass of gmsec::Message.
 */

#ifndef CMS_DESTINATION_H
#define CMS_DESTINATION_H

#include "gmsec_activemq.h"

#include <gmsec4_defs.h>

#include <gmsec4/internal/Value.h>

#include <gmsec4/Status.h>

#include <cms/Destination.h>

#include <string>


namespace gmsec_amqcms
{


/**
* @class CMSDestination
* @desc Used for storing ActiveMQ CMS Destination object in meta-data (details) of InternalMessage
*/
class GMSEC_ACTIVEMQ_API CMSDestination : public gmsec::api::internal::Value
{
public:
	CMSDestination(cms::Destination* dest);

	~CMSDestination();

	cms::Destination* getReplyTo() const;

	bool isOpaque() const { return true; }

    gmsec::api::Status getBoolean(bool& value) const;
    gmsec::api::Status getI32(GMSEC_I32& value) const;
    gmsec::api::Status getF64(GMSEC_F64& value) const;
    gmsec::api::Status getString(std::string& value) const;

	gmsec::api::internal::Value* clone() const;

private:
	cms::Destination* replyTo;
};


}  // end namespace gmsec_amqcms

#endif
