/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






/** 
 * @file gm_opt_msg.cpp
 * 
 * A C++ example demonstration of creating GMSEC Messages using two available
 * container types: Binary Tree Map (STL map) or Hash Map (unordered map).
 * 
 */

#include "../example.h"

#include <string>
#include <sstream>
#include <iostream>

using namespace gmsec;
using namespace gmsec::util;
using namespace example;


class gm_opt_msg
{
public:
	gm_opt_msg(ConfigFile& c);

	~gm_opt_msg();

	Status Run();

private:
	void setup();
	void teardown();

	void createMsgUsingConfigFile();
	void createMsgUsingConfigObject();

	Message* buildMessage(const int numFields, Config* config = 0);
	void destroyMessage(Message* msg);
	void displayMessage(Message* msg, const char* header = 0);

	ConfigFile& cf;
	Connection* connection;
};


gm_opt_msg::gm_opt_msg(ConfigFile& c)
    : cf(c),
      connection(0)
{
}


gm_opt_msg::~gm_opt_msg()
{
    teardown();    // just to be cautious
}


Status
gm_opt_msg::Run()
{
    Status result;

    setup();
    createMsgUsingConfigFile();
    createMsgUsingConfigObject();
    teardown();

    return result;
}


void
gm_opt_msg::setup()
{
    Status result;

    /* Load and Parse */
    result = cf.Load();
    check("ConfigLoad", result);

    /* Retrieve config from file */
    Config cfg;
    cf.LookupConfig("config", &cfg);

	initialize(cfg);

    /* Create the Connection */
    result = ConnectionFactory::Create(&cfg, connection);
    check("Create Connection", result);

    /* Connect */
    result = connection->Connect();
    check("Connect", result);
}


void
gm_opt_msg::teardown()
{
    if (connection)
    {
        if (connection->IsConnected())
        {
            checkNoThrow("Disconnect", connection->Disconnect());
        }
        checkNoThrow("Destroying Connection", ConnectionFactory::Destroy(connection));
    }

    connection = 0;

    checkNoThrow("ShutdownAllMiddlewares", ConnectionFactory::ShutdownAllMiddlewares());
}


void
gm_opt_msg::createMsgUsingConfigFile()
{
    for (int i = 0; i < 3; ++i)
    {
        /* Create message from config entry */
        Message* msg = buildMessage(0);

        // form msg name (e.g. msg1) to reference.
        std::stringstream msgName;
        msgName << "msg" << (i+1);

        // reference configuration for msg from file
        Status result = cf.LookupMessage(msgName.str().c_str(), msg);
        check("LookupMessage", result);

        /* Dump Message */
        const char* hdr = 0;
        switch (i+1)
        {
            case 1: hdr = "Expecting TREE map (at size limit of 8 fields):"; break;
            case 2: hdr = "Using TREE but expecting HASH map (exceeds size limit of 8 fields):"; break;
            case 3: hdr = "Expecting HASH map (size limit is ignored):"; break;
        }

        displayMessage(msg, hdr);

        destroyMessage(msg);
    }
}


void
gm_opt_msg::createMsgUsingConfigObject()
{
    Config cfg1, cfg2, cfg3;

    cfg1.AddValue("gmsec-msgfld-store-type", "tree");
    cfg1.AddValue("gmsec-msgfld-store-size", "10");      // max storage size before rollover (default is 50)

    cfg2.AddValue("gmsec-msgfld-store-type", "hash");

    cfg3.AddValue("gmsec-msgfld-store-type", "tree");    // storage type
    cfg3.AddValue("gmsec-msgfld-store-size", "0");       // never rollover

    Message* msg1 = buildMessage( 9, &cfg1);    // uses bin-tree storage map (under size limit)
    Message* msg2 = buildMessage(10, &cfg1);    // uses bin-tree storage map (at size limit)
    Message* msg3 = buildMessage(11, &cfg1);    // uses hash storage map (exceeds size limit)
    Message* msg4 = buildMessage( 5, &cfg2);    // default to using hash storage map (size limit ignored)
    Message* msg5 = buildMessage(55, &cfg3);    // always uses bin-tree storage map
    Message* msg6 = buildMessage(50);           // uses bin-tree storage map (default size limit not exceeded)
    Message* msg7 = buildMessage(51);           // uses hash storage map (exceeds default size limit)

    displayMessage(msg1, "Message using TREE storage map (under limit):");
    displayMessage(msg2, "Message using TREE storage map (at max limit):");
    displayMessage(msg3, "Message was using TREE storage map, but rolls over to HASH map (limit exceeded):");
    displayMessage(msg4, "Message using HASH storage map (limit ignored):");
    displayMessage(msg5, "Message using TREE storage map (limit ignored):");
    displayMessage(msg6, "Message using TREE storage map (at limit):");
    displayMessage(msg7, "Message using HASH storage map (limit exceeded):");

    destroyMessage(msg1);
    destroyMessage(msg2);
    destroyMessage(msg3);
    destroyMessage(msg4);
    destroyMessage(msg5);
    destroyMessage(msg6);
    destroyMessage(msg7);
}


Message*
gm_opt_msg::buildMessage(const int numFields, Config* config)
{
    Message* request = 0;
    Status   result;

    if (numFields > 0)
    {
        result = connection->CreateMessage("GMSEC.SYSTEST.TEST.REQ.DIR.LONG", GMSEC_MSG_REQUEST, request, config);
        check("CreateMessage(4 arg)", result);

        // Add Fields
        for (GMSEC_I32 i = 0; i < numFields; ++i)
        {
            Field fld;
            std::ostringstream oss;

            oss << "FIELD-" << (i < 10 ? "00" : (i < 100 ? "0" : "")) << i;

            fld.SetName(oss.str().c_str());
            fld.SetValue(i);

            result = request->AddField(fld);
            check("AddField", result);
        }
    }
    else
    {
        result = connection->CreateMessage(request);
        check("CreateMessage(1 arg)", result);
    }

    return request;
}


void
gm_opt_msg::destroyMessage(Message* msg)
{
    Status result = connection->DestroyMessage(msg);
    check("DestroyMessage", result);
}


void
gm_opt_msg::displayMessage(Message* msg, const char* header)
{
    const char* xml = 0;
    Status result = msg->ToXML(xml);
    check("ToXML", result);

    if (header)
    {
        LOG_INFO  << header << "\n" << xml;
    }
    else
    {
        LOG_INFO  << xml;
    }
}


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <config-file>" << std::endl;
        return -1;
    }

    try
    {
        ConfigFile cf(argv[1]);

        gm_opt_msg(cf).Run();
    }
    catch (std::exception& e)
    {
        LOG_ERROR << "Failed: " << e.what();
        return -1;
    }
}
