
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
#include <bolt/Stringify.h>

using namespace std;
using namespace bolt;



static void checkResult(const char *tag, const Result &result)
{
	// cout << tag << " => " << result << endl;

	if (result.isError())
		throw result;
}


void run()
{
	Log::SetDefaultStream(&cout);
	Log::SetReportingLevel(logDEBUG);

	Server server;
	server.host = "127.0.0.1";
	server.port = 9100;

	Connection conn(server);

	cout << "connection at " << &conn << "\n";

	Result result = conn.connect();
	checkResult("connect", result);

	const char *replyTo = "REQ";
	result = conn.subscribe(replyTo);
	checkResult("subscribe", result);

	SharedPacket packet(new Packet(PACKET_REQUEST));
	Meta meta;
	meta.add(Property::createID("M-0"));
	meta.add(Property::createTopic("A.B"));
	meta.add(Property::createReplyTo(replyTo));
	packet->setMeta(meta);
	unsigned char uc[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	ByteBuffer body(uc, 7, false);
	packet->setBody(body);

	result = conn.put(packet);
#if 1
	checkResult("put", result);
#else
	if (result.isValid()) {
		cout << "published ";
		packet->put(cout);
	}
	else
		cout << "warning: " << result << '\n';
#endif

	// sleep(10);

	result = conn.get(packet, 5000);
	checkResult("get", result);
	if (result.isValid()) {
		cout << "received ";
		packet->put(cout);
	}
	else
		cout << "warning: " << result << '\n';
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

