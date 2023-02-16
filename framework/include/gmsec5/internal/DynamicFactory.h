/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_DYNAMIC_FACTORY_H
#define GMSEC_API5_INTERNAL_DYNAMIC_FACTORY_H

#include <gmsec5/Config.h>

#include <string>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	namespace secure
	{
	class Policy;
	class Cipher;
	class Signer;
	class Access;
	class Random;
	}

namespace internal
{
	using ::gmsec::api5::Status;
	using ::gmsec::api5::Config;

class DynamicFactory
{
public:
	typedef void * (*CreatorFunction) (Status *);

	static Status initialize (gmsec::api5::secure::Policy &policy, const Config &config);

	static Status initialize (gmsec::api5::secure::Random &random, const Config &config);

	static Status initialize (gmsec::api5::secure::Access &access, const Config &config);

	static Status initialize (gmsec::api5::secure::Cipher &cipher, const Config &config);

	static Status initialize (gmsec::api5::secure::Signer &signer, const Config &config);

	// static Status initialize (gmsec::api5::Connection &connection, const Config &config);

private:
	// Declared, but not implemented.
	DynamicFactory();
	DynamicFactory(const DynamicFactory &);
	DynamicFactory &operator=(const DynamicFactory &);

	static CreatorFunction getCreator (Status &status, const Config &config, const char * typekey, const char * procname);

	static void * findSymbol (Status &result, const char * basename, const char *procname);

	static Status determineLibrary (const Config &config, const char * key, std::string &libname);

};


} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
