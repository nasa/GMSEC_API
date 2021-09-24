/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_DYNAMIC_FACTORY_H
#define GMSEC_API_INTERNAL_DYNAMIC_FACTORY_H

#include <gmsec4/Config.h>

#include <string>


namespace gmsec {

namespace api {

namespace secure {
class Policy;
class Cipher;
class Signer;
class Access;
class Random;
} // namespace secure


namespace internal {


using ::gmsec::api::Status;
using ::gmsec::api::Config;


class DynamicFactory
{
public:

	typedef void * (*CreatorFunction) (Status *);

	static Status initialize (gmsec::api::secure::Policy &policy, const Config &config);

	static Status initialize (gmsec::api::secure::Random &random, const Config &config);

	static Status initialize (gmsec::api::secure::Access &access, const Config &config);

	static Status initialize (gmsec::api::secure::Cipher &cipher, const Config &config);

	static Status initialize (gmsec::api::secure::Signer &signer, const Config &config);

	// static Status initialize (gmsec::api::Connection &connection, const Config &config);

private:

	// Declared, but not implemented.
	DynamicFactory();
	DynamicFactory(const DynamicFactory &);
	DynamicFactory &operator=(const DynamicFactory &);

	static CreatorFunction getCreator (Status &status, const Config &config,
				const char * typekey, const char * procname);

	static void * findSymbol (Status &result, const char * basename, const char *procname);

	static Status determineLibrary (const Config &config, const char * key, std::string &libname);

}; // class DynamicFactory


} // namespace internal
} // namespace api
} // namespace gmsec

#endif
