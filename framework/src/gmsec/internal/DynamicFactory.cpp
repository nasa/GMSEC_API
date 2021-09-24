/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/**
 *  @file DynamicFactory.cpp
 *  Generic implementation of the shared library instantiation mechanism.
*/


#ifdef WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <gmsec/internal/DynamicFactory.h>

#include <gmsec/secure/Policy.h>
#include <gmsec/secure/Access.h>
#include <gmsec/secure/Cipher.h>
#include <gmsec/secure/Signer.h>
#include <gmsec/secure/Random.h>
#include <gmsec/util/sysutil.h>

#include <gmsec/internal/Log.h>
#include <gmsec/internal/strutil.h>


using std::string;

using gmsec::Status;
using gmsec::Config;

using namespace gmsec::util;
using namespace gmsec::secure;


namespace gmsec {
namespace internal {


void * DynamicFactory::findSymbol(Status &result, const char *libname, const char *procname)
{
	if (!libname || !procname)
	{
		result.Set(GMSEC_STATUS_FACTORY_ERROR, GMSEC_LIBRARY_LOAD_FAILURE,
			"findSymbol: Invalid library / function");
		return NULL;
	}

	string errstr;

#if defined(WIN32)
	FARPROC proc = NULL;

	// check to see if this library is already loaded
	HMODULE mod = GetModuleHandle(libname);

	// not loaded - load now
	if (mod == NULL)
		mod = LoadLibrary(libname);

	// If we successfully loaded the library
	if (mod != NULL)
	{
		// lookup the address for the standard connection allocation method
		proc = GetProcAddress(mod, procname);
	}

	if (!proc)
		getErrorString(GetLastError(), errstr);

#else	/* not WIN32, therefore UNIX */

	void *proc = NULL;

	// attempt to link in library
	void *mod = dlopen(libname, RTLD_NOW);

	// If we successfully loaded the library
	if (mod != NULL)
	{
		// lookup the address for the standard connection allocation method
		proc = dlsym(mod, procname);

		if (!proc)
		{
			const char* s = dlerror();

			errstr = (s != NULL ? s : "Unable to find procname");

			dlclose(mod);
		}
	}

#endif /* WIN32 */

	if (!errstr.empty())
	{
		result.Set(GMSEC_STATUS_FACTORY_ERROR, GMSEC_LIBRARY_LOAD_FAILURE, errstr.c_str());
	}

	if (result.isError())
	{
		LOG_ERROR << "DynamicFactory::findSymbol: error: " << result.Get();
	}

	return proc;
}


Status DynamicFactory::determineLibrary(const Config &config, const char * key, string &libname)
{
	const char * value = NULL;
	Status status = config.GetValue(key, value);
	if (status.isError() || !value)
	{
		LOG_WARNING << "determineLibrary: missing '" << key << "'";
		return status;
	}

	// should the library basename depend on the object type?

	const char * fullyQualified = NULL;
	Status policyStatus = config.GetValue(FULLY_QUALIFIED_POLICY, fullyQualified);

	if (!policyStatus.IsError() && fullyQualified && stringIsTrue(fullyQualified))
	{
		libname = value;
	}
	else
	{

#ifdef WIN32

		libname = "gmsec_" + stringToLower(value);

#else
		string ext;

		//	need to resolve full name, including lib<name>.so
		//	But, it's lib<name>.dylib on the Mac.  Running this command:
		//	"cc -E -dM trivial.c"
		//	on a trivial.c that contains an empty main() and no #includes,
		//	reveals the minimal (75) set of macros that gcc #defines.
		//	__APPLE__ is probably not used by any other *nix boxen. (payter)

#if defined (__APPLE__)
		ext = ".dylib";
#elif defined (__hpux)
		// we should only use .sl for old HP-UXs
		ext = ".sl";
#else
		ext = ".so";
#endif

		libname = "libgmsec_" + stringToLower(value) + ext;

#endif /* WIN32 */
	}

	return status;
}


DynamicFactory::CreatorFunction DynamicFactory::getCreator (Status &status,
		const Config &config, const char * typekey, const char * procname)
{
	string libname;
	status = determineLibrary(config, typekey, libname);
	if (status.isError())
		return NULL;

	CreatorFunction proc = (CreatorFunction) findSymbol(status,
			libname.c_str(), procname);

	if (!status.isError() && !proc)
		status.Set(GMSEC_STATUS_FACTORY_ERROR,
				GMSEC_LIBRARY_LOAD_FAILURE,
				"Unable to find Create function");

	if (status.isError())
		return NULL;

	return proc;
}


// The current policy should be asked whether it allows changing the policy and
// its aspects.
// First try common policies, then library
Status DynamicFactory::initialize (Policy &policy, const Config &config)
{
	Status status;

	AbstractPolicy * ptr = 0;

	// TODO: clean this up.  There should be a registry of known types so the
	// particular type names do not appear here.
	const char * value = 0;
	Status test = config.GetValue(SEC_POLICY, value);
	if (test.isError())
		ptr = new API3Policy();
	else if (!value)
		;
	else if (stringEqualsIgnoreCase(value, "API3"))
		ptr = new API3Policy();

	if (!ptr)
	{
		CreatorFunction creator = getCreator(status, config, SEC_POLICY, "CreatePolicy");
		if (!status.isError() && creator)
			ptr = static_cast<AbstractPolicy *>((*creator)(&status));
		else
			ptr = new InvalidPolicy(status);
	}

	ptr->setExternal(policy);
	if (!status.isError())
		status = policy.initialize(config);

	return status;
}


// Gee, this could be a template for Random/Cipher/Signer...
Status DynamicFactory::initialize (Random &random, const Config &config)
{
	Status status;

	AbstractRandom * ptr = 0;

	const char * value = 0;
	Status test = config.GetValue(POLICY_ACCESS, value);
	if (test.isError())
		ptr = new MersenneTwister();

	if (!ptr)
	{
		CreatorFunction creator = getCreator(status, config, POLICY_RANDOM, "CreateRandom");
		if (!status.isError() && creator)
			ptr = static_cast<AbstractRandom *>((*creator)(&status));
	}

	if (!ptr)
	{
		status.Set(GMSEC_STATUS_FACTORY_ERROR,
				GMSEC_OTHER_ERROR,
				"Unable to create Access object");
	}
	else if (!status.isError())
	{
		ptr->setExternal(random);
		status = random.initialize(config);
	}
	else
	{
		delete ptr;
	}

	if (status.isError())
	{
		LOG_WARNING << "DynamicFactory::initialize " << status.Get();
	}

	return status;
}


Status DynamicFactory::initialize (Access &access, const Config &config)
{
	Status status;

	AbstractAccess * ptr = 0;

	const char * value = 0;
	Status test = config.GetValue(POLICY_ACCESS, value);
	if (test.isError())
		ptr = new OpenAccess();

	if (!ptr)
	{
		CreatorFunction creator = getCreator(status, config, POLICY_ACCESS, "CreateAccess");
		if (!status.isError() && creator)
			ptr = static_cast<AbstractAccess *>((*creator)(&status));
	}

	if (!ptr)
	{
		status.Set(GMSEC_STATUS_FACTORY_ERROR,
				GMSEC_OTHER_ERROR,
				"Unable to create Access object");
	}
	else if (!status.isError())
	{
		ptr->setExternal(access);
		status = access.initialize(config);
	}
	else
	{
		delete ptr;
	}

	if (status.isError())
	{
		LOG_WARNING << "DynamicFactory::initialize " << status.Get();
	}

	return status;
}


Status DynamicFactory::initialize (Cipher &cipher, const Config &config)
{
	Status status;

	AbstractCipher * ptr = 0;

	const char * value = 0;
	Status test = config.GetValue(POLICY_CIPHER, value);
	if (test.isError())
		ptr = new NullCipher();

	if (!ptr)
	{
		CreatorFunction creator = getCreator(status, config, POLICY_CIPHER, "CreateCipher");
		if (!status.isError() && creator)
			ptr = static_cast<AbstractCipher *>((*creator)(&status));
	}

	if (!ptr)
	{
		status.Set(GMSEC_STATUS_FACTORY_ERROR,
				GMSEC_OTHER_ERROR,
				"Unable to create Cipher object");
	}
	else if (!status.isError())
	{
		ptr->setExternal(cipher);
		status = cipher.initialize(config);
	}
	else
	{
		delete ptr;
	}

	if (status.isError())
	{
		LOG_WARNING << "DynamicFactory::initialize " << status.Get();
	}

	return status;
}


Status DynamicFactory::initialize (Signer &signer, const Config &config)
{
	Status status;

	AbstractSigner * ptr = 0;

	const char * value = 0;
	Status test = config.GetValue(POLICY_SIGNER, value);
	if (test.isError())
		ptr = new NullSigner();

	if (!ptr)
	{
		CreatorFunction creator = getCreator(status, config, POLICY_SIGNER, "CreateSigner");
		if (!status.isError() && creator)
			ptr = static_cast<AbstractSigner *>((*creator)(&status));
	}

	if (!ptr)
	{
		status.Set(GMSEC_STATUS_FACTORY_ERROR,
				GMSEC_OTHER_ERROR,
				"Unable to create Signer object");
	}
	else if (!status.isError())
	{
		ptr->setExternal(signer);
		status = signer.initialize(config);
	}
	else
	{
		delete ptr;
	}

	if (status.isError())
	{
		LOG_WARNING << "DynamicFactory::initialize " << status.Get();
	}

	return status;
}


} // namespace internal
} // namespace gmsec
