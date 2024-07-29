/*
 * Copyright 2007-2024 United States Government as represented by the
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

#include <gmsec5_defs.h>

#include <gmsec5/internal/Value.h>

#include <gmsec5/Status.h>

#include <cms/Destination.h>

#include <string>


namespace gmsec_amqcms
{


/**
* @class CMSDestination
* @desc Used for storing ActiveMQ CMS Destination object in meta-data (details) of InternalMessage
*/
class GMSEC_ACTIVEMQ_API CMSDestination : public gmsec::api5::internal::Value
{
public:
	CMSDestination(cms::Destination* dest);

	~CMSDestination();

	cms::Destination* getReplyTo() const;

	bool isOpaque() const { return true; }

    gmsec::api5::Status getBoolean(bool& value) const;
    gmsec::api5::Status getI32(GMSEC_I32& value) const;
    gmsec::api5::Status getF64(GMSEC_F64& value) const;
    gmsec::api5::Status getString(std::string& value) const;

	gmsec::api5::internal::Value* clone() const;

private:
	cms::Destination* replyTo;
};


}  // end namespace gmsec_amqcms

#endif
