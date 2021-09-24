
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

#include <gmsec_defs.h>		// for MESSAGE_TRACKINGFIELDS_* macros

#include <gmsec/internal/TrackingDetails.h>

using namespace gmsec::internal;


TrackingDetails::TrackingDetails()
	:
	fTrackingFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fNodeFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fProcessIdFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fUserNameFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fConnectionIdFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fPublishTimeFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fUniqueIdFlag(MESSAGE_TRACKINGFIELDS_UNSET),
	fMwInfoFlag(MESSAGE_TRACKINGFIELDS_UNSET)
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
		fTrackingFlag     = other.fTrackingFlag;
		fNodeFlag         = other.fNodeFlag;
		fProcessIdFlag    = other.fProcessIdFlag;
		fUserNameFlag     = other.fUserNameFlag;
		fConnectionIdFlag = other.fConnectionIdFlag;
		fPublishTimeFlag  = other.fPublishTimeFlag;
		fUniqueIdFlag     = other.fUniqueIdFlag;
		fMwInfoFlag       = other.fMwInfoFlag;
	}
	return *this;
}


int TrackingDetails::Get() const
{
	return fTrackingFlag;
}


int TrackingDetails::GetNode() const
{
	return fNodeFlag;
}


int TrackingDetails::GetProcessId() const
{
	return fProcessIdFlag;
}


int TrackingDetails::getUserName() const
{
	return fUserNameFlag;
}


int TrackingDetails::GetConnectionId() const
{
	return fConnectionIdFlag;
}


int TrackingDetails::GetPublishTime() const
{
	return fPublishTimeFlag;
}


int TrackingDetails::GetUniqueId() const
{
	return fUniqueIdFlag;
}


int TrackingDetails::GetMwInfo() const
{
	return fMwInfoFlag;
}



void TrackingDetails::Set(int flag)
{
	fTrackingFlag = flag;
}


void TrackingDetails::SetNode(int flag)
{
	fNodeFlag = flag;
}


void TrackingDetails::SetProcessId(int flag)
{
	fProcessIdFlag = flag;
}


void TrackingDetails::SetUserName(int flag)
{
	fUserNameFlag = flag;
}


void TrackingDetails::SetConnectionId(int flag)
{
	fConnectionIdFlag = flag;
}


void TrackingDetails::SetPublishTime(int flag)
{
	fPublishTimeFlag = flag;
}


void TrackingDetails::SetUniqueId(int flag)
{
	fUniqueIdFlag = flag;
}


void TrackingDetails::SetMwInfo(int flag)
{
	fMwInfoFlag = flag;
}


void TrackingDetails::set(const TrackingDetails &other)
{
	fTrackingFlag = other.fTrackingFlag;
	fNodeFlag = other.fNodeFlag;
	fProcessIdFlag = other.fProcessIdFlag;
	fUserNameFlag = other.fUserNameFlag;
	fConnectionIdFlag = other.fConnectionIdFlag;
	fPublishTimeFlag = other.fPublishTimeFlag;
	fMwInfoFlag = other.fMwInfoFlag;
}


