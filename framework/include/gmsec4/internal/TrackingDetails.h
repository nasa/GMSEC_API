/*
 * Copyright 2007-2018 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file TrackingDetails.h
 *  @brief This file contains a class for managing tracking configuration.
 */

#ifndef GMSEC_API_INTERNAL_TRACKING_DETAILS_H
#define GMSEC_API_INTERNAL_TRACKING_DETAILS_H


#include <gmsec4_defs.h>

#include <gmsec4/util/wdllexp.h>

#include <string>


#define MESSAGE_TRACKINGFIELDS_UNSET    -1
#define MESSAGE_TRACKINGFIELDS_ON       GMSEC_TRUE
#define MESSAGE_TRACKINGFIELDS_OFF      GMSEC_FALSE


namespace gmsec {
namespace api {
namespace internal {


/** @class TrackingDetails
 * @brief This class stores the tracking details for connections / messages.
 */
class GMSEC_API TrackingDetails
{
private:
	// Defined, but not implemented.
	TrackingDetails(const TrackingDetails &);


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


	/** @fn ~TrackingDetails()
	 * @brief destructor
	 */
	~TrackingDetails();


	/** @fn operator=()
	 * @brief Assignment operator
	 * @return Same TrackingDetails object, but initialized using other.
	 */
	TrackingDetails& operator=(const TrackingDetails& other);


	/** @fn GetTracking()
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


	/** @fn SetTracking( int flag )
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


	void set(const TrackingDetails &other);


	const char* toString() const;
};

} //namespace internal
} //namespace api
} //namespace gmsec

#endif

