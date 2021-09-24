/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file RequestSpecs.cpp
 *
 *  This file contains details regarding the setting up and mgmt of request messages.
 */


#ifndef GMSEC_API_INTERNAL_REQUEST_SPECS_H
#define GMSEC_API_INTERNAL_REQUEST_SPECS_H


#include <string>


namespace gmsec
{
namespace api
{
namespace internal
{


/**
 * @class RequestSpecs
 * @brief This class collects configuration for request/response behavior.
 */
struct RequestSpecs
{
	RequestSpecs()
	 : legacy(true),
	   replySubject(),
	   connectionID(),
	   exposeReplies(false),
	   multiResponse(false),
	   useSubjectMapping(true),
	   requestReplyEnabled(true)
	{
	}

	bool        legacy;
	std::string replySubject;
	std::string connectionID;
	bool        exposeReplies;
	bool        multiResponse;
	bool        useSubjectMapping;
	bool        requestReplyEnabled;
};


}  // end namespace internal
}  // end namespace api
}  // end namespace gmsec

#endif
