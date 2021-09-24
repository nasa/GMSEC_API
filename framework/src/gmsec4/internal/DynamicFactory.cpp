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

#include <gmsec4/internal/DynamicFactory.h>

#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>

#include <gmsec4/secure/Policy.h>
#include <gmsec4/secure/Access.h>
#include <gmsec4/secure/Cipher.h>
#include <gmsec4/secure/Signer.h>
#include <gmsec4/secure/Random.h>

#include <gmsec4/util/Log.h>


#include <sstream>


using gmsec::api::Status;
using gmsec::api::Config;

using namespace gmsec::api::util;
using namespace gmsec::api::secure;


namespace gmsec {
namespace api {
namespace internal {


void * DynamicFactory::findSymbol(Status &result, const char *libname, const char *procname)
{
	if (!libname || !procname)
	{
		result.set(FACTORY_ERROR, LIBRARY_LOAD_FAILURE, "findSymbol: Invalid library / function");
		return NULL;
	}

	std::string errstr;

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
	{
		SystemUtil::getErrorString(GetLastError(), errstr);
	}

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
		result.set(FACTORY_ERROR, LIBRARY_LOAD_FAILURE, errstr.c_str());
	}

	if (result.isError())
	{
		GMSEC_ERROR << "DynamicFactory::findSymbol: error: " << result.get();
	}

	return proc;
}


Status DynamicFactory::determineLibrary(const Config& config, const char* key, std::string& libname)
{
	Status status;

	const char* value = config.getValue(key);

	if (!value)
	{
		std::ostringstream oss;
		oss << "Config does not have an entry for: " << key;

		GMSEC_WARNING << oss.str().c_str();

		status.set(FACTORY_ERROR, INVALID_CONFIG_VALUE, oss.str().c_str());

		return status;
	}

	// should the library basename depend on the object type?

	const char* fullyQualified = config.getValue(GMSEC_POLICY_FULL_PATH);

	if (fullyQualified && StringUtil::stringIsTrue(fullyQualified))
	{
		libname = value;
	}
	else
	{

#ifdef WIN32

		libname = "gmsec_" + StringUtil::stringToLower(value);

#else
		std::string ext;

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

		libname = "libgmsec_" + StringUtil::stringToLower(value) + ext;

#endif /* WIN32 */
	}

	return status;
}


DynamicFactory::CreatorFunction DynamicFactory::getCreator(Status& status,
		const Config& config, const char* typekey, const char* procname)
{
	std::string libname;

	status = determineLibrary(config, typekey, libname);

	if (status.isError())
	{
		return NULL;
	}

	CreatorFunction proc = (CreatorFunction) findSymbol(status, libname.c_str(), procname);

	if (!proc)
	{
		status.set(FACTORY_ERROR, LIBRARY_LOAD_FAILURE, "Unable to find Create function");
		return NULL;
	}

	return proc;
}


// The current policy should be asked whether it allows changing the policy and
// its aspects.
// First try common policies, then library
Status DynamicFactory::initialize (Policy &policy, const Config &config)
{
	Status status;

	AbstractPolicy* ptr = 0;

	// TODO: clean this up.  There should be a registry of known types so the
	// particular type names do not appear here.
	const char* value = config.getValue(GMSEC_POLICY);

	if (!value)
	{
		ptr = new API4Policy();
	}
	else if (StringUtil::stringEqualsIgnoreCase(value, "API4"))
	{
		ptr = new API4Policy();
	}
	else if (StringUtil::stringEqualsIgnoreCase(value, "API3"))
	{
		ptr = new API3Policy();
	}

	if (!ptr)
	{
		CreatorFunction creator = getCreator(status, config, GMSEC_POLICY, "createPolicy");

		if (!status.isError() && creator)
		{
			ptr = static_cast<AbstractPolicy *>((*creator)(&status));
		}
		else
		{
			ptr = new InvalidPolicy(status);
		}
	}

	ptr->setExternal(policy);

	if (!status.isError())
	{
		status = policy.initialize(config);
	}

	return status;
}


// Gee, this could be a template for Random/Cipher/Signer...
Status DynamicFactory::initialize (Random &random, const Config &config)
{
	Status status;

	AbstractRandom* ptr = 0;

	const char* value = config.getValue(GMSEC_POLICY_ACCESS);
	if (!value)
	{
		ptr = new MersenneTwister();
	}

	if (!ptr)
	{
		CreatorFunction creator = getCreator(status, config, POLICY_RANDOM, "CreateRandom");

		if (!status.isError() && creator)
		{
			ptr = static_cast<AbstractRandom *>((*creator)(&status));
		}
	}

	if (!ptr)
	{
		status.set(FACTORY_ERROR, OTHER_ERROR_CODE, "Unable to create Access object");
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
		GMSEC_WARNING << "DynamicFactory::initialize " << status.get();
	}

	return status;
}


Status DynamicFactory::initialize (Access &access, const Config &config)
{
	Status status;

	AbstractAccess* ptr = 0;

	const char* value = config.getValue(GMSEC_POLICY_ACCESS);

	if (!value)
	{
		ptr = new OpenAccess();
	}

	if (!ptr)
	{
		CreatorFunction creator = getCreator(status, config, GMSEC_POLICY_ACCESS, "CreateAccess");

		if (!status.isError() && creator)
		{
			ptr = static_cast<AbstractAccess *>((*creator)(&status));
		}
	}

	if (!ptr)
	{
		status.set(FACTORY_ERROR, OTHER_ERROR_CODE, "Unable to create Access object");
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
		GMSEC_WARNING << "DynamicFactory::initialize " << status.get();
	}

	return status;
}


Status DynamicFactory::initialize (Cipher &cipher, const Config &config)
{
	Status status;

	AbstractCipher* ptr = 0;

	const char * value = config.getValue(GMSEC_POLICY_CIPHER);

	if (!value)
	{
		ptr = new NullCipher();
	}

	if (!ptr)
	{
		CreatorFunction creator = getCreator(status, config, GMSEC_POLICY_CIPHER, "CreateCipher");

		if (!status.isError() && creator)
		{
			ptr = static_cast<AbstractCipher *>((*creator)(&status));
		}
	}

	if (!ptr)
	{
		status.set(FACTORY_ERROR, OTHER_ERROR_CODE, "Unable to create Cipher object");
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
		GMSEC_WARNING << "DynamicFactory::initialize " << status.get();
	}

	return status;
}


Status DynamicFactory::initialize (Signer &signer, const Config &config)
{
	Status status;

	AbstractSigner* ptr = 0;

	const char* value = config.getValue(GMSEC_POLICY_SIGNER);

	if (!value)
	{
		ptr = new NullSigner();
	}

	if (!ptr)
	{
		CreatorFunction creator = getCreator(status, config, GMSEC_POLICY_SIGNER, "CreateSigner");

		if (!status.isError() && creator)
		{
			ptr = static_cast<AbstractSigner *>((*creator)(&status));
		}
	}

	if (!ptr)
	{
		status.set(FACTORY_ERROR, OTHER_ERROR_CODE, "Unable to create Signer object");
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
		GMSEC_WARNING << "DynamicFactory::initialize " << status.get();
	}

	return status;
}


} // namespace internal
} // namespace api
} // namespace gmsec
