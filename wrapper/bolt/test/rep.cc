
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



static void checkResult(const char *tag, const Result &result, bool throwIfError = true)
{
	cout << tag << " => " << result << endl;

	if (throwIfError && result.isError())
		throw result;
}


Result sendReply(Connection &conn, SharedPacket &request)
{
	Result result;

	Meta *reqMeta = request->getMeta();

	string replyTo = reqMeta->getReplyTo();
	if (replyTo.empty())
	{
		result.set(2, "missing reply to address");
		LOG_WARNING << result;
		return result;
	}

	string corrID = reqMeta->getID();
	if (corrID.empty())
	{
		result.set(2, "missing request ID");
		LOG_WARNING << result;
		return result;
	}

	SharedPacket packet(new Packet(PACKET_REPLY));
	Meta meta;
	meta.add(Property::createID("M-" + util::toString(13)));
	meta.add(Property::createTopic(replyTo));
	meta.add(Property::createCorrID(corrID));
	packet->setMeta(meta);
	unsigned char uc[8] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	ByteBuffer body(uc, 3, false);
	packet->setBody(body);

	result = conn.put(packet);

	return result;
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
		{
			packet->put(cout);
			if (packet->getHeader()->getType() == PACKET_REQUEST)
			{
				result = sendReply(conn, packet);
				checkResult("sendReply", result);
			}
			else
				cout << "replier received non-request...\n";
		}
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

