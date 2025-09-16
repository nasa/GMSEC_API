/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_OPENDDS_CONNECTION_H
#define GMSEC_OPENDDS_CONNECTION_H

#include <gmsec_opendds.h>
#include <Queue.h>

#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/UniqueFilter.h>
#include <gmsec5/internal/Encoder.h>

#include <gmsec5/Config.h>
#include <gmsec5/Message.h>
#include <gmsec5/Status.h>

// OpenDDS required header files
#include <dds/DdsDcpsInfrastructureC.h>
#include <dds/DdsDcpsSubscriptionC.h>

#include <dds/DCPS/Marked_Default_Qos.h>
#include <dds/DCPS/Service_Participant.h>

#include <dds/DCPS/StaticIncludes.h>

#include <OpenDDSMessageListener.h>
#include <OpenDDSMessageTypeSupportImpl.h>

#include <string>


namespace gmsec_opendds
{

// OpenDDS implementation of the GMSEC ConnectionInterface.
class GMSEC_OPENDDS_API OpenDDSConnection : public gmsec::api5::internal::ConnectionInterface
{
public:
	// Create an OpenDDS Connection with configuration.
	OpenDDSConnection(const gmsec::api5::Config& config);
	
	// Destroy an OpenDDS Connection.
	virtual ~OpenDDSConnection();

	// Determines the middleware wrapper tag.
	virtual const char* getLibraryRootName()
	{
		return OPENDDS_LIBROOTNAME;
	}
	
	// Determines the middleware version string.
	virtual const char* getLibraryVersion();
	
	// This function returns the middleware information
	virtual const char* getMWInfo();

	// This function does nothing for OpenDDS.
	virtual void mwConnect();

	// Disconnect from the middleware server.
	virtual void mwDisconnect();

	// Subscribe to subject (OpenDDS software wrapper currently does not support wildcards) using the configuration supplied in the config object.
	virtual void mwSubscribe(const char* subject, const gmsec::api5::Config &config);

	// Unsubscribe from subject (OpenDDS software wrapper currently does not support wildcards).
	virtual void mwUnsubscribe(const char* subject);

	// Publish message msg using configuration.
	virtual void mwPublish(const gmsec::api5::Message& msg, const gmsec::api5::Config& config);

	// Send request message request.
	virtual void mwRequest(const gmsec::api5::Message& request, const std::string& uniqueID);

	// Wait up to @p timeout milliseconds for next message.
	virtual void mwReceive(gmsec::api5::Message*& msg, GMSEC_I32 timeout);

	// Generate a unique identifier string for a request message.
	virtual std::string mwGetUniqueID();
	
private:
	std::string                         m_brokerService;
	std::string                         m_openddsConfig;
	std::string                         m_dcpsInfoRepo;
	std::string                         m_mwInfo;
	Queue                               m_queue;
	GMSEC_U32                           m_requestCounter;
	gmsec::api5::internal::RequestSpecs m_requestSpecs;
	bool                                m_selfSubscribed;

	struct OpenDDSSubscriber
	{
		DDS::Subscriber_var 		subscriber;
		DDS::DataReaderListener_var listener;
		DDS::DataReader_var 		reader;
	};

	struct OpenDDSWriter
	{
		DDS::Publisher_var  publisher;
		DDS::DataWriter_var writer;
	};

	typedef std::map<std::string, OpenDDSSubscriber*> OpenDDSSubscriptions;  // key is subject
	typedef std::map<std::string, OpenDDSWriter*>     OpenDDSWriters;        // key is subject

	OpenDDSSubscriptions               m_subscriptions;
	OpenDDSWriters                     m_writers;
	DDS::DomainParticipantFactory_var  m_dpf;
	DDS::DomainParticipant_var         m_participant;
	DDS::DomainId_t                    m_domainId;


	// Helper methods
	OpenDDSWriter* createWriter(const std::string& subject, const gmsec::api5::Config& config);

	OpenDDSMessage::Message prepareMsg(const gmsec::api5::Message& msg);

	std::string generateUniqueId(GMSEC_U32 id);

	// Encode the meta data associated with the message
	gmsec::api5::Status storeProperties(const gmsec::api5::internal::ValueMap& meta, gmsec::api5::util::DataBuffer& out);


	// Determine the size of the meta blob
	void findLength(const gmsec::api5::internal::ValueMap& meta, size_t& length);

	// Encode the gmsec::internal::Value into the meta blob
	gmsec::api5::Status storeProperty(const char* valueId, const gmsec::api5::internal::Value& value, char** currentBuffer);

	std::string getConnectionEndpoint();
};

}

#endif
