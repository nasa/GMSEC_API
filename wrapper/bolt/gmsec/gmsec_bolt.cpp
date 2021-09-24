/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#include <gmsec_bolt.h>

#include <BoltConnection.h>

#include <gmsec4/internal/ConnectionInterface.h>

#include <gmsec4/Config.h>
#include <gmsec4/Status.h>

#include <gmsec4/util/Log.h>

#include <string>
#include <sstream>


using namespace gmsec::api;
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

	std::string serverStr("127.0.0.1");

	Config emptyConfig;

	if (config)
	{
		const char* value = config->getValue("server");

		if (value)
		{
			serverStr = value;
		}
	}
	else
	{
		config = &emptyConfig;
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

	std::string keep;
	bool doport = false;
	size_t end = str.length();

	for (size_t p = 0; result.isValid() && p < end; ++p) {

		char c = str[p];

		if (isspace(c))
			; // ignore whitespace anywhere in host / port value
		else if (c == ':') {
			if (!doport) {
				// save host string, on to port
				spec.host = keep;
				keep = "";
				doport = true;
			}
			else
				result.set(1, "doport and colon");
		}
		else if (doport) {
			if (isdigit(c))
				keep.push_back(c);
			else
				result.set(1, "doport and non-digit");
		}
		else {
			keep.push_back(c);
		}
	}

	if (result.isValid())
	{
		if (keep.length() > 0) {
			if (doport) {
				std::istringstream in(keep);
				in >> spec.port;
				if (!in || spec.port < 1 || in.peek() != EOF)
					result.set(1, "unable to get port from " + keep);
			}
			else {
				spec.host = keep;
				spec.port = bolt::DEFAULT_PORT;
			}
		}
		else {
			if (doport)
				result.set(1, "doport and empty port string");
			else
				spec.port = bolt::DEFAULT_PORT;
		}
	}

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
