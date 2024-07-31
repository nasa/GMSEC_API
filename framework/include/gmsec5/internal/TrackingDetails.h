/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 *  @file TrackingDetails.h
 *  @brief This class manages tracking field options.
 */

#ifndef GMSEC_API5_INTERNAL_TRACKING_DETAILS_H
#define GMSEC_API5_INTERNAL_TRACKING_DETAILS_H


#include <gmsec5/util/wdllexp.h>

#include <string>


#define MESSAGE_TRACKINGFIELDS_ON        1
#define MESSAGE_TRACKINGFIELDS_OFF       0
#define MESSAGE_TRACKINGFIELDS_UNSET    -1


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Config;

namespace internal
{


// Internal meta-detail field for determining if tracking fields should be validated.
// Should be set to 'true' when a message is being sent.
// This definition should not be defined in ConfigOptions.h!
//
const char* const GMSEC_CHECK_TRACKING_FIELDS = "GMSEC-CHECK-TRACKING-FIELDS";


/** @class TrackingDetails
 * @brief This class stores the tracking details for connections / messages.
 */
class GMSEC_API TrackingDetails
{
private:
	int fTrackingFlag;
	int fNodeFlag;
	int fProcessIdFlag;
	int fUserNameFlag;
	int fConnectionIdFlag;
	int fPublishTimeFlag;
	int fUniqueIdFlag;
	int fMwInfoFlag;
	int fActiveSubscriptionsFlag;
	int fConnectionEndpointFlag;

	mutable std::string fDetails;

public:
	/** @fn TrackingDetails()
	 * Default Constructor.
	 */
	TrackingDetails();


	/** @fn TrackingDetails(const TrackingDetails& other)
	 * @brief Copy constructor
	 */
	TrackingDetails(const TrackingDetails& other);


	/** @fn ~TrackingDetails()
	 * @brief destructor
	 */
	~TrackingDetails();


	/** @fn operator=()
	 * @brief Assignment operator
	 * @return Same TrackingDetails object, but initialized using other.
	 */
	TrackingDetails& operator=(const TrackingDetails& other);


	/** @fn get<xyz>()
	 * @brief Get the tracking fields state.
	 * @return (MESSAGE_TRACKINGFIELDS_UNSET,MESSAGE_TRACKINGFIELDS_ON,MESSAGE_TRACKINGFIELDS_OFF)
	 */
	int get() const;
	int getNode() const;
	int getProcessId() const;
	int getUserName() const;
	int getConnectionId() const;
	int getPublishTime() const;
	int getUniqueId() const;
	int getMwInfo() const;
	int getActiveSubscriptions() const;
	int getConnectionEndpoint() const;


	/** @fn set<xyz>( int flag )
	 * @brief Turn tracking fields on/off for this Message
	 */
	void set(int flag);
	void setNode(int flag);
	void setProcessId(int flag);
	void setUserName(int flag);
	void setConnectionId(int flag);
	void setPublishTime(int flag);
	void setUniqueId(int flag);
	void setMwInfo(int flag);
	void setActiveSubscriptions(int flag);
	void setConnectionEndpoint(int flag);


	/** @fn TrackingDetails initialize(const Config& config)
	 * @brief Read the given Config to produce a TrackingDetails object.
	 */
	static TrackingDetails initialize(const Config& config);

	const char* toString() const;
};

} //namespace internal
} //namespace api5
} //namespace gmsec

#endif
