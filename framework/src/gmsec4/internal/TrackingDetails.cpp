/*
 * Copyright 2007-2021 United States Government as represented by the
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

#include <gmsec4/internal/TrackingDetails.h>

#include <gmsec4/internal/StringUtil.h>

#include <gmsec4/Config.h>
#include <gmsec4/ConfigOptions.h>
#include <gmsec4/Message.h>

#include <gmsec4/util/cxx.h>
#include <gmsec4/util/Log.h>

#include <gmsec4_defs.h>		// for MESSAGE_TRACKINGFIELDS_* macros

#include <sstream>

#if GMSEC_CXX_11_AVAILABLE == 1
#include <regex>
#endif


using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;


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


TrackingDetails::TrackingDetails(const TrackingDetails& other)
	: fTrackingFlag(other.fTrackingFlag),
	  fNodeFlag(other.fNodeFlag),
	  fProcessIdFlag(other.fProcessIdFlag),
	  fUserNameFlag(other.fUserNameFlag),
	  fConnectionIdFlag(other.fConnectionIdFlag),
	  fPublishTimeFlag(other.fPublishTimeFlag),
	  fUniqueIdFlag(other.fUniqueIdFlag),
	  fMwInfoFlag(other.fMwInfoFlag),
	  fActiveSubscriptionsFlag(other.fActiveSubscriptionsFlag),
	  fConnectionEndpointFlag(other.fConnectionEndpointFlag)
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


TrackingDetails TrackingDetails::initialize(const Config& config)
{
	TrackingDetails tracking;

	tracking.set(config.getBooleanValue(GMSEC_TRACKING, true));

	if (config.getValue(GMSEC_TRACKING_NODE))
	{
		tracking.setNode(config.getBooleanValue(GMSEC_TRACKING_NODE, true));
	}
	if (config.getValue(GMSEC_TRACKING_PROCESS_ID))
	{
		tracking.setProcessId(config.getBooleanValue(GMSEC_TRACKING_PROCESS_ID, true));
	}
	if (config.getValue(GMSEC_TRACKING_USERNAME))
	{
		tracking.setUserName(config.getBooleanValue(GMSEC_TRACKING_USERNAME, true));
	}
	if (config.getValue(GMSEC_TRACKING_CONNECTION_ID))
	{
		tracking.setConnectionId(config.getBooleanValue(GMSEC_TRACKING_CONNECTION_ID, true));
	}
	if (config.getValue(GMSEC_TRACKING_PUBLISH_TIME))
	{
		tracking.setPublishTime(config.getBooleanValue(GMSEC_TRACKING_PUBLISH_TIME, true));
	}
	if (config.getValue(GMSEC_TRACKING_UNIQUE_ID))
	{
		tracking.setUniqueId(config.getBooleanValue(GMSEC_TRACKING_UNIQUE_ID, true));
	}
	if (config.getValue(GMSEC_TRACKING_MW_INFO))
	{
		tracking.setMwInfo(config.getBooleanValue(GMSEC_TRACKING_MW_INFO, true));
	}
	if (config.getValue(GMSEC_TRACKING_ACTIVE_SUBSCRIPTIONS))
	{
		tracking.setActiveSubscriptions(config.getBooleanValue(GMSEC_TRACKING_ACTIVE_SUBSCRIPTIONS, true));
	}
	if (config.getValue(GMSEC_TRACKING_CONNECTION_ENDPOINT))
	{
		tracking.setConnectionEndpoint(config.getBooleanValue(GMSEC_TRACKING_CONNECTION_ENDPOINT, true));
	}

	return tracking;
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

	oss << "TRACKING               : " << flagToString(fTrackingFlag) << "\n";
	oss << "NODE                   : " << flagToString(fNodeFlag) << "\n";
	oss << "PROCESS-ID             : " << flagToString(fProcessIdFlag) << "\n";
	oss << "USER-NAME              : " << flagToString(fUserNameFlag) << "\n";
	oss << "CONNECTION-ID          : " << flagToString(fConnectionIdFlag) << "\n";
	oss << "PUBLISH-TIME           : " << flagToString(fPublishTimeFlag) << "\n";
	oss << "UNIQUE-ID              : " << flagToString(fUniqueIdFlag) << "\n";
	oss << "MW-INFO                : " << flagToString(fMwInfoFlag) << "\n";
	oss << "ACTIVE-SUBSCRIPTIONS   : " << flagToString(fActiveSubscriptionsFlag) << "\n";
	oss << "MW-CONNECTION-ENDPOINT : " << flagToString(fConnectionEndpointFlag) << "\n";
	oss << "\n";

	fDetails = oss.str();

	return fDetails.c_str();
}


#if 0
bool TrackingDetails::isReservedField(const char* fieldName)
{
	static const char* reservedFields[] =
	{
		"CONNECTION-ID",
		"MW-INFO",
		"NODE",
		"PROCESS-ID",
		"PUBLISH-TIME",
		"UNIQUE-ID",
		"USER-NAME",
		"NUM-OF-SUBSCRIPTIONS",
		"SUBSCRIPTION.(.*).SUBJECT-PATTERN",
		"CONNECTION-ENDPOINT",
		"MW-CONNECTION-ENDPOINT",
		NULL
	};

	bool reserved = false;

#if GMSEC_CXX_11_AVAILABLE == 1

	for (size_t i = 0; !reserved && reservedFields[i]; ++i)
	{
		reserved = std::regex_match(fieldName, std::regex(reservedFields[i]));
	}

#else

	int index;
	if (sscanf(fieldName, "SUBSCRIPTION.%d.SUBJECT-PATTERN", &index) == 1)
	{
		reserved = true;
	}
	else
	{
		for (size_t i = 0; !reserved && reservedFields[i]; ++i)
		{
			reserved = (std::string(fieldName) == reservedFields[i]);
		}
	}

#endif

	return reserved;
}


Status TrackingDetails::checkTrackingField(const char* fieldName, const TrackingDetails& configTracking, const TrackingDetails& msgTracking)
{
	Status status;

	if (isReservedField(fieldName))
	{
		const int ON    = MESSAGE_TRACKINGFIELDS_ON;
		const int OFF   = MESSAGE_TRACKINGFIELDS_OFF;
		const int UNSET = MESSAGE_TRACKINGFIELDS_UNSET;

		bool addTracking = (configTracking.get() == ON && (msgTracking.get() == ON || msgTracking.get() == UNSET));
		bool haveError   = false;

		if (StringUtil::stringEquals(fieldName, "CONNECTION-ID"))
		{
			if ((addTracking || configTracking.getConnectionId() == ON || msgTracking.getConnectionId() == ON) &&
		    	(configTracking.getConnectionId() != OFF && msgTracking.getConnectionId() != OFF))
			{
				haveError = true;
			}
		}
		else if (StringUtil::stringEquals(fieldName, "MW-INFO"))
		{
			if ((addTracking || configTracking.getMwInfo() == ON || msgTracking.getMwInfo() == ON) &&
		    	(configTracking.getMwInfo() != OFF && msgTracking.getMwInfo() != OFF))
			{
				haveError = true;
			}
		}
		else if (StringUtil::stringEquals(fieldName, "NODE"))
		{
			if ((addTracking || configTracking.getNode() == ON || msgTracking.getNode() == ON) &&
		    	(configTracking.getNode() != OFF && msgTracking.getNode() != OFF))
			{
				haveError = true;
			}
		}
		else if (StringUtil::stringEquals(fieldName, "PROCESS-ID"))
		{
			if ((addTracking || configTracking.getProcessId() == ON || msgTracking.getProcessId() == ON) &&
		    	(configTracking.getProcessId() != OFF && msgTracking.getProcessId() != OFF))
			{
				haveError = true;
			}
		}
		else if (StringUtil::stringEquals(fieldName, "PUBLISH-TIME"))
		{
			if ((addTracking || configTracking.getPublishTime() == ON || msgTracking.getPublishTime() == ON) &&
		    	(configTracking.getPublishTime() != OFF && msgTracking.getPublishTime() != OFF))
			{
				haveError = true;
			}
		}
		else if (StringUtil::stringEquals(fieldName, "UNIQUE-ID"))
		{
			if ((addTracking || configTracking.getUniqueId() == ON || msgTracking.getUniqueId() == ON) &&
		    	(configTracking.getUniqueId() != OFF && msgTracking.getUniqueId() != OFF))
			{
				haveError = true;
			}
		}
		else if (StringUtil::stringEquals(fieldName, "USER-NAME"))
		{
			if ((addTracking || configTracking.getUserName() == ON || msgTracking.getUserName() == ON) &&
		    	(configTracking.getUserName() != OFF && msgTracking.getUserName() != OFF))
			{
				haveError = true;
			}
		}
		else if (StringUtil::stringEquals(fieldName, "NUM-OF-SUBSCRIPTIONS"))
		{
			if ((addTracking || configTracking.getActiveSubscriptions() == ON || msgTracking.getActiveSubscriptions() == ON) &&
		    	(configTracking.getActiveSubscriptions() != OFF && msgTracking.getActiveSubscriptions() != OFF))
			{
				haveError = true;
			}
		}
		else if (std::string(fieldName).find("SUBSCRIPTION.") != std::string::npos && std::string(fieldName).find(".SUBJECT-PATTERN") != std::string::npos)
		{
			if ((addTracking || configTracking.getActiveSubscriptions() == ON || msgTracking.getActiveSubscriptions() == ON) &&
		    	(configTracking.getActiveSubscriptions() != OFF && msgTracking.getActiveSubscriptions() != OFF))
			{
				int val;
				int tmp = sscanf(fieldName, "SUBSCRIPTION.%d.SUBJECT-PATTERN", &val);

				haveError = (tmp == 1);
			}
		}
		else if (StringUtil::stringEquals(fieldName, "CONNECTION-ENDPOINT") || StringUtil::stringEquals(fieldName, "MW-CONNECTION-ENDPOINT"))
		{
			if ((addTracking || configTracking.getConnectionEndpoint() == ON || msgTracking.getConnectionEndpoint() == ON) &&
		    	(configTracking.getConnectionEndpoint() != OFF && msgTracking.getConnectionEndpoint() != OFF))
			{
				haveError = true;
			}
		}

		if (haveError)
		{
			std::stringstream err;
			err << fieldName << " is a reserved tracking field for the GMSEC API";
			status.set(MIST_ERROR, NON_ALLOWED_FIELD, err.str().c_str());
		}
	}

	return status;
}
#endif
