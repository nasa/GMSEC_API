/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConnectionFactory.h
 *
 * @brief This file contains the methods for creating and destroying Connections.
 */

#ifndef GMSEC_API_INTERNAL_CONNECTION_FACTORY
#define GMSEC_API_INTERNAL_CONNECTION_FACTORY


#include <gmsec4/util/wdllexp.h>

#include <string>


namespace gmsec
{
namespace api
{
// Forward declaration(s)
class Config;
class Connection;

namespace internal
{


class GMSEC_API ConnectionFactory
{
public:
    static Connection* CALL_TYPE create(const Config& config);

    static void CALL_TYPE destroy(Connection*& conn);

private:
	static Connection* createConnection(const std::string& libname, const Config& config);

	static void* lookupDLL(const std::string& libname, const char* functionName);
};


}  // end namespace internal
}  // end namespace api
}  // end namespace gmsec

#endif
