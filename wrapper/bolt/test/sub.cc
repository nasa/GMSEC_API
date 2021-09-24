
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <iostream>
#include <bolt/Connection.h>
#include <bolt/Property.h>
#include <bolt/Log.h>

using namespace std;
using namespace bolt;



static void checkResult(const char *tag, const Result &result, bool throwIfError = true)
{
	cout << tag << " => " << result << endl;

	if (throwIfError && result.isError())
		throw result;
}


void run()
{
	Log::SetDefaultStream(&cout);
	Log::SetReportingLevel(bolt::logDEBUG);

	Server server;
	server.host = "127.0.0.1";
	server.port = 9100;

	Connection conn(server);

	cout << "connection at " << &conn << "\n";

	Result result = conn.connect();
	checkResult("connect", result);

	result = conn.subscribe("A.>");
	checkResult("subscribe(A.>)", result);

	result = conn.subscribe("X.Y.Z");
	checkResult("subscribe(X.Y.Z)", result);

	SharedPacket packet;
	const int timeout_ms = 10000;
	const bool ignoreErrors = true;
	while (ignoreErrors || result.isValid())
	{
		result = conn.get(packet, timeout_ms);
		checkResult("get(packet&)", result, false);
		if (result.isValid())
			packet->put(cout);
	}
}


int main (int argc, char **argv)
{
	int code = 0;

	try
	{
		run();
	}
	catch (const Result &e)
	{
		code = 1;
		cout << "caught " << e << endl;
	}

	return code;
}

