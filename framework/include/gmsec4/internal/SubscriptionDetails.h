/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file SubscriptionDetails.h
 * @brief This file contains the class that stores details regarding
 * subscriptions that have been set up with the middleware.
 *
 */

#include <gmsec4/Config.h>

#include <list>
#include <stddef.h>  // for size_t


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Callback;

namespace internal
{


struct Details
{
	Details(Callback* cb, const Config& cfg);

	Callback* callback;
	Config    config;
};


/**
 * @class SubscriptionDetails
 * @brief Bucket for carrying information related to a request to set up a
 * subscription with the middleware.
 */
class SubscriptionDetails
{
public:
	/**
	 * @fn SubscriptionDetails()
	 * @desc Constructor
	 */
	SubscriptionDetails();


	/**
	 * @fn ~SubscriptionDetails()
	 * @desc Destructor
	 */
	~SubscriptionDetails();


	/**
	 * @fn void addDetails(Callback* cb, const Config& cfg)
	 * @brief Adds the given callback and configuration details
	 * to the SubscriptionDetails.
	 * @param cb - a callback object, possibly null, associated with a subscription.
	 * @param cfg - a configuration object associated with a subscription.
	 */
	void addDetails(Callback* cb, const Config& cfg);


	/**
	 * @fn void removeDetails(Callback* cb = NULL)
	 * @brief Removes the details associated with the given callback, or
	 * all details if no callback is provided, from the SubscriptionDetails.
	 * @param cb - a callback object, possibly null, associated with a subscription.
	 */
	void removeDetails(Callback* cb = NULL);


	/**
	 * @fn size_t numDetailsRegistered() const
	 * @brief Returns the number of detail object associated with the SubscriptionDetails.
	 * @return size_t - the number of detail objects.
	 */
	size_t numDetailsRegistered() const;


	/**
	 * @fn bool callbackRegistered(Callback* cb) const
	 * @brief Reports whether the given callback is already associated with the
	 * SubscriptionDetails.
	 * @return bool - true if association with callback is found, false otherwise.
	 */
	bool callbackRegistered(Callback* cb) const;


	/**
	 * @fn void resetIter()
	 * @desc Resets the interator associated with the list of details contained within
	 * the SubscriptionDetails object.
	 */
	void resetIter();


	/**
	 * @fn bool hasNext()
	 * @brief Reports whether there is another details object within the SubscriptionDetails
	 * that can be obtained using next().
	 * @return bool - true if another details object is available; false otherwise.
	 */
	bool hasNext();


	/**
	 * @fn Details* next()
	 * @brief Returns pointer to the next available details object associated with the
	 * SubscriptionDetails, or NULL if none is available.
	 * @return Details* - pointer to Details object, or NULL if none can be accessed.
	 */
	const Details* next();

private:
	typedef std::list<Details*> DetailsList;

	DetailsList           m_details;
	DetailsList::iterator m_iter;
};

}  // namespace internal
}  // namespace api
}  // namespace gmsec
