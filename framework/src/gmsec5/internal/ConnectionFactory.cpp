/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ConnectionFactory.cpp
 *
 * @brief This file contains the methods for creating and destroying Connections.
 */

#include <gmsec5/internal/ConnectionFactory.h>

#include <gmsec5/internal/ConnectionInterface.h>
#include <gmsec5/internal/InternalConnection.h>
#include <gmsec5/internal/StringUtil.h>
#include <gmsec5/internal/SystemUtil.h>

#include <gmsec5/Config.h>
#include <gmsec5/ConfigOptions.h>
#include <gmsec5/GmsecException.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/Log.h>
#include <gmsec5/util/Mutex.h>

#include <sstream>

#if defined(WIN32)
#include <windows.h>
#else
#include <dlfcn.h>
#include <cstdio>
#endif


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;

static Mutex g_mutex;


typedef void GMSEC_CREATECONN_PROC(const Config*, ConnectionInterface**, Status * status);


ConnectionInterface* ConnectionFactory::create(const Config& config)
{
	// Deduce the middleware type (connectionType or mw-id)
	//
	std::string libname;
	const char* value = config.getValue(GMSEC_MIDDLEWARE_ID);
	if (value)
	{
		libname = std::string("gmsec_") + value;
	}
	else
	{
		value = config.getValue(GMSEC_CONN_TYPE);

		if (value)
		{
			GMSEC_WARNING << "The " << GMSEC_CONN_TYPE << " configuration option is deprecated; use "
			              << GMSEC_MIDDLEWARE_ID << " instead, sans the 'gmsec_'";

			libname = value;
		}
		else
		{
			throw GmsecException(FACTORY_ERROR, INVALID_CONNECTION_TYPE, "mw-id in Config is not specified");
		}
	}

	// Check if we are dealing with an apollo-style m/w; e.g. gmsec_apollo383
	// If so, then replace 'apollo' with 'activemq'.
	size_t apollo = libname.find("apollo");

	if (apollo != std::string::npos)
	{
		libname.replace(apollo, 6, "activemq");
	}


	AutoMutex lock(g_mutex);

	// Load and create connection using appropriate connection interface
	return createConnection(libname, config);
}


ConnectionInterface* ConnectionFactory::createConnection(const std::string& libname, const Config& config)
{
	ConnectionInterface* connIf = NULL;

	GMSEC_CREATECONN_PROC* ccproc = (GMSEC_CREATECONN_PROC*) lookupDLL(libname, "createConnection");

	if (ccproc)
	{
		Status status;

		ccproc(&config, &connIf, &status);

		if (status.hasError())
		{
			throw GmsecException(status);
		}
	}
	else
	{
		throw GmsecException(FACTORY_ERROR, INVALID_CONNECTION_TYPE, "Unable to load createConnection() from DLL");
	}

	return connIf;
}


void* ConnectionFactory::lookupDLL(const std::string& libname0, const char* functionName)
{
	std::string libname = libname0;

#ifdef WIN32
	FARPROC proc = NULL;

	#if _DEBUG
		libname = libname + "_d";
	#endif

	// check to see if this library is already loaded
	HMODULE mod = GetModuleHandle(libname.c_str());

	// if not loaded, load now
	if (!mod)
	{
		mod = LoadLibrary(libname.c_str());
	}

	if (mod)
	{
		// lookup the address for the standard connection allocation function
		proc = GetProcAddress(mod, functionName);
	}
	else
	{
		std::string errstr;
		SystemUtil::getErrorString(GetLastError(), errstr);

		std::ostringstream oss;
		oss << "Unable to load " << libname << " DLL or one of its dependencies due to this error: " << errstr;

		throw GmsecException(FACTORY_ERROR, LIBRARY_LOAD_FAILURE, oss.str().c_str());
	}

#else  // UNIX/Linux/BSD flavors

	// Determine if the library name represents an absolute path to the library.  A tell-tale
	// manner to determine if an absolute path has been specified is to examine if a forward-slash
	// character appears at the beginning of the library name.
	bool absolutePath = (libname[0] == '/');

	if (!absolutePath)
	{
		// An absolute path to a library name was NOT provided.
		// Thus determine which system we're on, and formulate the library name
		// using the 'lib' prefix and the appropriate filename extension.
		std::string libExtension;

		#if defined __APPLE__
			libExtension = ".dylib";
		#elif defined __hpux
			libExtension = ".sl";
		#else
			libExtension = ".so";
		#endif

		libname = std::string("lib") + libname + libExtension;
	}

	void* proc = NULL;

	// attempt to link in library
	void* mod = dlopen(libname.c_str(), RTLD_NOW);

	// If we successfully loaded the library
	if (mod)
	{
		// lookup the address for the standard connection allocation function
		proc = dlsym(mod, functionName);

		if (proc == NULL)
		{
			dlclose(mod);

			std::ostringstream oss;
			oss << "Unable to locate function " << functionName << " within " << libname.c_str();

			throw GmsecException(FACTORY_ERROR, LIBRARY_LOAD_FAILURE, oss.str().c_str());
		}
	}
	else
	{
		// generate error in the case of load library failure
		const char* errstr = dlerror();

		std::ostringstream oss;
		oss << "Unable to load " << libname << " library or one of its dependencies due to this error: " << (errstr ? errstr : "<unknown dlopen error>");

		throw GmsecException(FACTORY_ERROR, LIBRARY_LOAD_FAILURE, oss.str().c_str());
	}

#endif

	return proc;
}
