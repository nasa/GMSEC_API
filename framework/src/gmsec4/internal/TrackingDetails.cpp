/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 *  @file TrackingDetails.cpp
 *
 *  This file contains the base class for messages.
 *
 */

#include <gmsec4_defs.h>		// for MESSAGE_TRACKINGFIELDS_* macros

#include <gmsec4/internal/TrackingDetails.h>

#include <sstream>


using namespace gmsec::api::internal;


TrackingDetails::TrackingDetails()
	:
	fTrackingFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fNodeFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fProcessIdFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fUserNameFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fConnectionIdFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fPublishTimeFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fUniqueIdFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fMwInfoFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fActiveSubscriptionsFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fConnectionEndpointFlag(MESSAGE_TRACKINGFIELDS_UNSET)
{
}


TrackingDetails::~TrackingDetails()
{
}


TrackingDetails&
TrackingDetails::operator=(const TrackingDetails& other)
{
	if (this != &other)
	{
		fTrackingFlag            = other.fTrackingFlag;
		fNodeFlag                = other.fNodeFlag;
		fProcessIdFlag           = other.fProcessIdFlag;
		fUserNameFlag            = other.fUserNameFlag;
		fConnectionIdFlag        = other.fConnectionIdFlag;
		fPublishTimeFlag         = other.fPublishTimeFlag;
		fUniqueIdFlag            = other.fUniqueIdFlag;
		fMwInfoFlag              = other.fMwInfoFlag;
		fActiveSubscriptionsFlag = other.fActiveSubscriptionsFlag;
		fConnectionEndpointFlag  = other.fConnectionEndpointFlag;
	}
	return *this;
}


int TrackingDetails::get() const
{
	return fTrackingFlag;
}


int TrackingDetails::getNode() const
{
	return fNodeFlag;
}


int TrackingDetails::getProcessId() const
{
	return fProcessIdFlag;
}


int TrackingDetails::getUserName() const
{
	return fUserNameFlag;
}


int TrackingDetails::getConnectionId() const
{
	return fConnectionIdFlag;
}


int TrackingDetails::getPublishTime() const
{
	return fPublishTimeFlag;
}


int TrackingDetails::getUniqueId() const
{
	return fUniqueIdFlag;
}


int TrackingDetails::getMwInfo() const
{
	return fMwInfoFlag;
}


int TrackingDetails::getActiveSubscriptions() const
{
	return fActiveSubscriptionsFlag;
}


int TrackingDetails::getConnectionEndpoint() const
{
	return fConnectionEndpointFlag;
}


void TrackingDetails::set(int flag)
{
	fTrackingFlag = flag;
}


void TrackingDetails::setNode(int flag)
{
	fNodeFlag = flag;
}


void TrackingDetails::setProcessId(int flag)
{
	fProcessIdFlag = flag;
}


void TrackingDetails::setUserName(int flag)
{
	fUserNameFlag = flag;
}


void TrackingDetails::setConnectionId(int flag)
{
	fConnectionIdFlag = flag;
}


void TrackingDetails::setPublishTime(int flag)
{
	fPublishTimeFlag = flag;
}


void TrackingDetails::setUniqueId(int flag)
{
	fUniqueIdFlag = flag;
}


void TrackingDetails::setMwInfo(int flag)
{
	fMwInfoFlag = flag;
}


void TrackingDetails::setActiveSubscriptions(int flag)
{
	fActiveSubscriptionsFlag = flag;
}


void TrackingDetails::setConnectionEndpoint(int flag)
{
	fConnectionEndpointFlag = flag;
}


void TrackingDetails::set(const TrackingDetails &other)
{
	fTrackingFlag            = other.fTrackingFlag;
	fNodeFlag                = other.fNodeFlag;
	fProcessIdFlag           = other.fProcessIdFlag;
	fUserNameFlag            = other.fUserNameFlag;
	fConnectionIdFlag        = other.fConnectionIdFlag;
	fPublishTimeFlag         = other.fPublishTimeFlag;
	fMwInfoFlag              = other.fMwInfoFlag;
	fActiveSubscriptionsFlag = other.fActiveSubscriptionsFlag;
	fConnectionEndpointFlag  = other.fConnectionEndpointFlag;
}


static const char* flagToString(int flag)
{
	const char* str = "???";

	if (flag == MESSAGE_TRACKINGFIELDS_ON)
	{
		str = "ON";
	}
	else if (flag == MESSAGE_TRACKINGFIELDS_OFF)
	{
		str = "OFF";
	}
	else if (flag == MESSAGE_TRACKINGFIELDS_UNSET)
	{
		str = "UNSET";
	}

	return str;
}


const char* TrackingDetails::toString() const
{
	std::ostringstream oss;

	oss << "TRACKING            : " << flagToString(fTrackingFlag) << "\n";
	oss << "NODE                : " << flagToString(fNodeFlag) << "\n";
	oss << "PROCESS-ID          : " << flagToString(fProcessIdFlag) << "\n";
	oss << "USER-NAME           : " << flagToString(fUserNameFlag) << "\n";
	oss << "CONNECTION-ID       : " << flagToString(fConnectionIdFlag) << "\n";
	oss << "PUBLISH-TIME        : " << flagToString(fPublishTimeFlag) << "\n";
	oss << "UNIQUE-ID           : " << flagToString(fUniqueIdFlag) << "\n";
	oss << "MW-INFO             : " << flagToString(fMwInfoFlag) << "\n";
	oss << "ACTIVE-SUBSCRIPTIONS: " << flagToString(fActiveSubscriptionsFlag) << "\n";
	oss << "MW-CONNECTION-ENDPOINT : " << flagToString(fConnectionEndpointFlag) << "\n";
	oss << "\n";

	fDetails = oss.str();

	return fDetails.c_str();
}
