/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec_bolt.h>

#include <BoltConnection.h>

#include <gmsec5/internal/ConnectionInterface.h>

#include <gmsec5/Config.h>
#include <gmsec5/Status.h>

#include <gmsec5/util/Log.h>

#include <string>
#include <sstream>


using namespace gmsec::api5;
using namespace gmsec_bolt;

using bolt::Server;
using bolt::Result;


static Result parseServers(std::list<Server>& servers, const std::string& str);


/**
* @note This is not thread-safe by design.  Something external must ensure that
* multiple threads do not call CreateConnection simultaneously...
*/
GMSEC_MW_API void createConnection(const Config* config, internal::ConnectionInterface** connIf, Status* status)
{
	*connIf = NULL;

	if (status == NULL)
	{
		GMSEC_ERROR << "Status is NULL";
		return;
	}

	Config emptyConfig;

	const char* defaultServer("localhost");   // default server name if none is provided
	std::string serverStr;

	if (config)
	{
		serverStr = config->getValue(GMSEC_MIDDLEWARE_SERVER, "");

		if (serverStr.empty())
		{
			serverStr = config->getValue(GMSEC_SERVER, defaultServer);
		}
	}
	else
	{
		config = &emptyConfig;
		serverStr = defaultServer;
	}

	std::list<Server> servers;
	Result result = parseServers(servers, serverStr);

	if (result.isError())
	{
		status->set(CONFIG_ERROR, INVALID_CONFIG_VALUE, result.getText().c_str());
	}
	else
	{
		try
		{
			BoltConnection* boltconn = 0;

			// undecided whether it is preferable to implement
			// more complex code once, or simpler code twice...
			if (servers.size() == 1)
			{
				boltconn = new SingleConnection(*config);
			}
			else
			{
				boltconn = new MultipleConnection(*config);
			}

			boltconn->setServers(servers);

			*connIf = boltconn;
		}
		catch (std::exception& e)
		{
			status->set(MIDDLEWARE_ERROR, OTHER_ERROR_CODE, e.what());
		}
	}
}


static Result parseOne(Server& spec, const std::string& str)
{
	Result result;

	// The algorithm below attempts to handle server configuration strings
	// similar to the following:
	//
	//		10.10.115.50                          IPv4 address, no port
	//		10.10.115.50:12345                    IPv4 address, with port
	//		::                                    IPv6 loopback, no port
	//		:::12345                              IPv6 loopback, with port
	//		::1                                   IPv6 loopback, no port
	//		::1:12344                             IPv6 loopback, with port
	//		[2001:db8:85a3:0:0:8a2e:370]          IPv6 address, no port
	//		[2001:db8:85a3:0:0:8a2e:370]:12345    IPv6 address, with port
	//		myserver                              IPv4 or IPv6, no port
	//		myserver:12345                        IPv4 or IPv6, with port
	//

	std::string addr;
	std::string port;

	size_t openBracket  = str.find("[");
	size_t closeBracket = str.find("]");
	size_t colon        = str.rfind(":");

	if (openBracket == std::string::npos && closeBracket == std::string::npos)
	{
		if (str == "::1" || str == "::")
		{
			// IPv6 loopback (no port)
			addr = str;
		}
		else if (colon == std::string::npos)
		{
			// IPv4 address (no port)
			addr = str;
		}
		else
		{
			// IPv4 address or IPv6 loopback, and port
			addr = str.substr(0, colon);
			port = str.substr(colon + 1);
		}
	}
	else
	{
		// Dealing with IPv6 address (and possibly port)

		if (openBracket == std::string::npos)
		{
			result.set(1, "IPv6 address is missing opening square bracket");
		}
		else if (closeBracket == std::string::npos)
		{
			result.set(1, "IPv6 address is missing closing square bracket");
		}
		else
		{
			if (str[colon - 1] == ']')
			{
				// IPv6 address, and port
				addr = str.substr(openBracket+1, closeBracket-1);
				port = str.substr(colon + 1);
			}
			else
			{
				// IPv6 address (no port)
				addr = str.substr(openBracket+1, closeBracket-1);
			}
		}
	}

	if (result.isValid() && addr.length() > 0)
	{
		spec.host = addr;
	}
	else
	{
		result.set(1, "unable to get server from " + str);
	}

	if (result.isValid() && port.length() > 0)
	{
		std::istringstream in(port);

		in >> spec.port;

		if (!in || spec.port < 1 || in.peek() != EOF)
		{
			result.set(1, "unable to get port from " + port);
		}
	}
	else
	{
		spec.port = bolt::DEFAULT_PORT;
	}

	GMSEC_VERBOSE << "host: " << spec.host.c_str() << ", port: " << spec.port;

	return result;
}


static Result parseOne(std::list<Server>& servers, const std::string& str)
{
	Server server;
	GMSEC_DEBUG << "parseOne(servers, " << str.c_str() << ')';
	Result result = parseOne(server, str);
	if (result.isValid())
	{
		servers.push_back(server);
	}
	else
	{
		GMSEC_WARNING << "invalid spec '" << str.c_str() << "' : " << result << '\n';
	}
	return result;
}


Result parseServers(std::list<Server>& servers, const std::string& str)
{
	Result result;

	size_t start = 0;
	size_t end;
	while ((end = str.find_first_of(',', start)) != std::string::npos)
	{
		result = parseOne(servers, str.substr(start, end - start));
		if (result.isError())
		{
			return result;
		}
		start = end + 1;
	}

	if (start != str.length())
	{
		result = parseOne(servers, str.substr(start, end - start));
	}

	if (result.isValid() && servers.size() < 1)
	{
		result.set(1, "unable to parse any servers");
	}

	return result;
}


#ifdef TEST_SPLIT_DRIVER


void test(const char* p)
{
	std::string str(p);

	std::list<Server> specs;

	GMSEC_INFO << "test(" << p << ")";

	Result result = parseServers(specs, str);

	GMSEC_INFO << "parseServers(" << p << ") => " << result << " yielded " << specs.size();

	for (std::list<Server>::iterator i = specs.begin(); i != specs.end(); ++i)
	{
		GMSEC_INFO << "\thost=" << i->host << " port=" << i->port;
	}
}


void test ()
{
	test(" alpha: 3202");
	test("first, second:9101");
	test("a:second");
}


int main(int argc, char** argv)
{
	if (argc == 1)
	{
		test();
	}
	else
	{
		for (int i = 1; i < argc; ++i)
		{
			test(argv[i]);
		}
	}
}

#endif
