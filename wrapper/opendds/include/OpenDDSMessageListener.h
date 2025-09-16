/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef OPENDDS_MESSAGE_LISTENER_H
#define OPENDDS_MESSAGE_LISTENER_H

#include <OpenDDSConnection.h>
#include <Queue.h>

#include <gmsec5/Message.h>
#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/Encoder.h>

#include <ace/Global_Macros.h>

#include <dds/DdsDcpsSubscriptionC.h>
#include <dds/DCPS/LocalObject.h>
#include <dds/DCPS/Definitions.h>


namespace gmsec_opendds
{

// Forward declaration
class OpenDDSConnection;

// This class represents a thread that forms a subscription, listens to it, and posts messages to the appropriate queue within a connection object.
class OpenDDSMessageListener : public virtual OpenDDS::DCPS::LocalObject<DDS::DataReaderListener> 
{
public:
	// Constructor
	OpenDDSMessageListener(const char* subjectPattern,
	                       OpenDDSConnection* conn,
	                       Queue &queue,
	                       bool dropMessages = false);

	// Basic destructor
	virtual ~OpenDDSMessageListener(void);

	virtual void on_requested_deadline_missed(DDS::DataReader_ptr reader, const DDS::RequestedDeadlineMissedStatus& status);

	virtual void on_requested_incompatible_qos(DDS::DataReader_ptr reader, const DDS::RequestedIncompatibleQosStatus& status);

	virtual void on_sample_rejected(DDS::DataReader_ptr reader, const DDS::SampleRejectedStatus& status);

	virtual void on_liveliness_changed(DDS::DataReader_ptr reader, const DDS::LivelinessChangedStatus& status);

	virtual void on_data_available(DDS::DataReader_ptr reader);

	virtual void on_subscription_matched(DDS::DataReader_ptr reader, const DDS::SubscriptionMatchedStatus& status);

	virtual void on_sample_lost(DDS::DataReader_ptr reader, const DDS::SampleLostStatus& status);

private:
	std::string        subjectPattern;
	OpenDDSConnection* connection;
	Queue&             queue;
	bool               dropMessages;

	// Decode the meta data associated with the message
	gmsec::api5::Status parseProperties(gmsec::api5::internal::ValueMap& meta, const gmsec::api5::util::DataBuffer& out, gmsec::api5::Message& message);
};

}

#endif
