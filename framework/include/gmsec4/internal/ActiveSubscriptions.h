/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ActiveSubscriptions.h
 * @brief This file contains the class that stores details regarding
 * subscriptions that have been set up by an application.
 */

#include <gmsec4/internal/TicketMutex.h>

#include <gmsec4/util/wdllexp.h>

#include <map>
#include <set>
#include <string>


namespace gmsec
{
namespace api
{
namespace internal
{


/**
 * @class ActiveSubscriptions
 */
class GMSEC_API ActiveSubscriptions
{
public:
	ActiveSubscriptions();


	~ActiveSubscriptions();


	void addTopic(const char* topic);


	void removeTopic(const char* topic);


	std::set<std::string> getTopics();


private:
	// Defined, but not implemented.
	ActiveSubscriptions(const ActiveSubscriptions&);
	ActiveSubscriptions& operator=(const ActiveSubscriptions&);

	typedef std::map<std::string, int> RegisteredTopics;

	RegisteredTopics              m_registered;
	gmsec::api::util::TicketMutex m_mutex;
	std::set<std::string>         m_topics;
};

}  // namespace internal
}  // namespace api
}  // namespace gmsec
