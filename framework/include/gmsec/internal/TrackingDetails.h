
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file TrackingDetails.h
 *  @brief This file contains a class for managing tracking configuration.
**/

#ifndef gmsec_internal_TrackingDetails_h
#define gmsec_internal_TrackingDetails_h

#include <gmsec/util/wdllexp.h>

namespace gmsec
{
namespace internal
{

/** @class TrackingDetails
 * @brief This class stores the tracking details for connections / messages.
**/
class GMSEC_API TrackingDetails
{
private:

	// Declared, but not implemented.
	TrackingDetails(const TrackingDetails &);

	int fTrackingFlag;
	int fNodeFlag;
	int fProcessIdFlag;
	int fUserNameFlag;
	int fConnectionIdFlag;
	int fPublishTimeFlag;
	int fUniqueIdFlag;
	int fMwInfoFlag;

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

	int Get() const;
	int GetNode() const;
	int GetProcessId() const;
	int getUserName() const;
	int GetConnectionId() const;
	int GetPublishTime() const;
	int GetUniqueId() const;
	int GetMwInfo() const;


	/** @fn SetTracking( int flag )
	* @brief Turn tracking fields on/off for this Message
	*/

	void Set(int flag);
	void SetNode(int flag);
	void SetProcessId(int flag);
	void SetUserName(int flag);
	void SetConnectionId(int flag);
	void SetPublishTime(int flag);
	void SetUniqueId(int flag);
	void SetMwInfo(int flag);


	void set(const TrackingDetails &other);
};

} //namespace internal
} //namespace gmsec

#endif  // gmsec_internal_TrackingDetails_h

