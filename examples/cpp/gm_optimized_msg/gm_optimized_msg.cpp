/*
 * Copyright 2007-2016 United States Government as represented by the
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

using namespace gmsec::api;


class gm_opt_msg
{
public:
	gm_opt_msg(const char* filename);

	~gm_opt_msg();

	bool run();

private:
	void createMsgUsingConfigFile();
	void createMsgUsingConfigObject();

	Message buildMessage(const Config& config, size_t numFields);
	void displayMessage(const Message& msg, const char* header = 0);

	std::string cfgFilename;
	ConfigFile  cfgFile;
};


gm_opt_msg::gm_opt_msg(const char* filename)
	: cfgFilename(filename),
	  cfgFile()
{
}


gm_opt_msg::~gm_opt_msg()
{
}


bool gm_opt_msg::run()
{
	bool success = true;

	try
	{
		//o Load and parse configuration file
		cfgFile.load(cfgFilename.c_str());

		//o Retrieve config from file
		Config cfg = cfgFile.lookupConfig("config");

		example::initialize(cfg);

		createMsgUsingConfigFile();

		createMsgUsingConfigObject();
	}
	catch (Exception& e)
	{
		GMSEC_ERROR << e.what();
		success = false;
	}

	return success;
}


void gm_opt_msg::createMsgUsingConfigFile()
{
    for (int i = 0; i < 3; ++i)
    {
        //o Form message name (e.g. msg1) to reference.
        std::stringstream msgName;
        msgName << "msg" << (i+1);

        // Reference configuration file for message
        Message msg = cfgFile.lookupMessage(msgName.str().c_str());

        //o Display message
        const char* hdr = 0;
        switch (i+1)
        {
            case 1: hdr = "Expecting TREE map (at size limit of 8 fields):"; break;
            case 2: hdr = "Using TREE but expecting HASH map (exceeds size limit of 8 fields):"; break;
            case 3: hdr = "Expecting HASH map (size limit is ignored):"; break;
        }

        displayMessage(msg, hdr);
    }
}


void gm_opt_msg::createMsgUsingConfigObject()
{
    Config cfg1, cfg2, cfg3, empty;

    cfg1.addValue("gmsec-msgfld-store-type", "tree");
    cfg1.addValue("gmsec-msgfld-store-size", "10");      // max storage size before rollover (default is 50)

    cfg2.addValue("gmsec-msgfld-store-type", "hash");

    cfg3.addValue("gmsec-msgfld-store-type", "tree");    // storage type
    cfg3.addValue("gmsec-msgfld-store-size", "0");       // never rollover

    Message msg1 = buildMessage(cfg1,   9);    // uses bin-tree storage map (under size limit)
    Message msg2 = buildMessage(cfg1,  10);    // uses bin-tree storage map (at size limit)
    Message msg3 = buildMessage(cfg1,  11);    // uses hash storage map (exceeds size limit)
    Message msg4 = buildMessage(cfg2,   5);    // default to using hash storage map (size limit ignored)
    Message msg5 = buildMessage(cfg3,  55);    // always uses bin-tree storage map
    Message msg6 = buildMessage(empty, 50);   // uses bin-tree storage map (default size limit not exceeded)
    Message msg7 = buildMessage(empty, 51);   // uses hash storage map (exceeds default size limit)

    displayMessage(msg1, "Message using TREE storage map (under limit):");
    displayMessage(msg2, "Message using TREE storage map (at max limit):");
    displayMessage(msg3, "Message was using TREE storage map, but rolls over to HASH map (limit exceeded):");
    displayMessage(msg4, "Message using HASH storage map (limit ignored):");
    displayMessage(msg5, "Message using TREE storage map (limit ignored):");
    displayMessage(msg6, "Message using TREE storage map (at limit):");
    displayMessage(msg7, "Message using HASH storage map (limit exceeded):");
}


Message gm_opt_msg::buildMessage(const Config& config, size_t numFields)
{
	Message msg("GMSEC.SYSTEST.TEST.REQ.DIR.LONG", Message::PUBLISH, config);

    if (numFields > 0)
    {
        // Add Fields
        for (size_t i = 0; i < numFields; ++i)
        {
            std::ostringstream oss;
            oss << "FIELD-" << (i < 10 ? "00" : (i < 100 ? "0" : "")) << i;

            msg.addField(oss.str().c_str(), (GMSEC_U64) i);
        }
    }

    return msg;
}


void gm_opt_msg::displayMessage(const Message& msg, const char* header)
{
    const char* xml = msg.toXML();

    if (header)
    {
        GMSEC_INFO  << header << "\n" << xml;
    }
    else
    {
        GMSEC_INFO  << xml;
    }
}


int main(int argc, char** argv)
{
	if (argc != 2)
	{
		std::cerr << "usage: " << argv[0] << " <config-file>" << std::endl;
		return -1;
	}

	if (gm_opt_msg(argv[1]).run())
	{
		GMSEC_INFO << "Completed successfully!";
	}
	else
	{
		GMSEC_ERROR << "Failed!";
		return -1;
	}
}
