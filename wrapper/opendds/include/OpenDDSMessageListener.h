/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef OPENDDS_MESSAGE_LISTENER_H
#define OPENDDS_MESSAGE_LISTENER_H

#include <OpenDDSConnection.h>
#include <Queue.h>

#include <gmsec4/Message.h>
#include <gmsec4/internal/ConnectionInterface.h>
#include <gmsec4/internal/Encoder.h>

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
	                       const gmsec::api::internal::RequestSpecs& specs,
	                       bool isReplyListener = false,
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
	std::string                               subjectPattern;
	OpenDDSConnection*                        connection;
	Queue&                                    queue;
	const gmsec::api::internal::RequestSpecs& requestSpecs;
	bool                                      isReplyListener;
	bool                                      dropMessages;

	// Decode the meta data associated with the message
	gmsec::api::Status parseProperties(gmsec::api::internal::ValueMap& meta, const gmsec::api::util::DataBuffer& out, gmsec::api::Message& message);
};

}

#endif
