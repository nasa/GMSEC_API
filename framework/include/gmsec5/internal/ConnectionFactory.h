/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConnectionFactory.h
 *
 * @brief This file contains the methods for creating and destroying Connections.
 */

#ifndef GMSEC_API5_INTERNAL_CONNECTION_FACTORY
#define GMSEC_API5_INTERNAL_CONNECTION_FACTORY


#include <gmsec5/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Config;

namespace internal
{
	class ConnectionInterface;


class GMSEC_API ConnectionFactory
{
public:
    static ConnectionInterface* CALL_TYPE create(const Config& config);

private:
	static ConnectionInterface* createConnection(const std::string& libname, const Config& config);

	static void* lookupDLL(const std::string& libname, const char* functionName);
};


}  // end namespace internal
}  // end namespace api5
}  // end namespace gmsec

#endif
