/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file gmsec_schema_ids.cpp
 * 
 * A C++ utility for listing Schema IDs (and aliases) that can be used to
 * construct a MistMessage object.
 */

#include <gmsec4_cpp.h>
#include <gmsec4/internal/mist/SchemaIdMapping.h>

#include <iomanip>
#include <iostream>
#include <string>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


std::string lookupAlias(const std::string& schemaID, unsigned int msgSpecVersion);


int main(int argc, char* argv[])
{
	typedef gmsec::api::util::DataList<MessageSpecification*> MessageSpecifications;

	Config config(argc, argv);

	try
	{
		Specification spec(config);

		std::cout << "\nList of Message Schema IDs available for the "
		          << spec.getVersion() / 100 
		          << " message specification:\n"
		          << std::endl;

		const int         id_width    = 20;
		const int         alias_width = 30;
		const int         num_flds    = 2;
		const std::string sep         = "|";
		const std::string spc         = " ";
		const int         tot_width   = id_width + alias_width + ((sep.size() + spc.size()) * num_flds);
		const std::string line        = sep + std::string(tot_width-1, '-') + '|';

		std::cout << std::left << line << "\n" << sep
		          << spc << std::setw(id_width) << "Msg Schema ID" << sep
		          << spc << std::setw(alias_width) << "Alias(es)" << sep
		          << "\n" << line << "\n";


		const MessageSpecifications& msgSpecs = spec.getMessageSpecifications();

		for (MessageSpecifications::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
		{
			const MessageSpecification* msgSpec = *it;

			std::string alias = lookupAlias(msgSpec->getSchemaID(), spec.getVersion());

			std::cout << std::left << sep
			          << spc << std::setw(id_width) << msgSpec->getSchemaID() << sep
			          << spc << std::setw(alias_width) << (alias.empty() ? "" : alias) << sep
			          << "\n";
		}

		std::cout << line << "\n\n" << std::endl;
	}
	catch (const Exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;;
		return 1;
	}
}


std::string lookupAlias(const std::string& schemaID, unsigned int msgSpecVersion)
{
	std::string alias;

	SchemaIdMapping map;

	for (SchemaIdMapping::SchemaMap::const_iterator it = map.m_schemaMap.begin(); it != map.m_schemaMap.end(); ++it)
	{
		if (msgSpecVersion >= GMSEC_ISD_2019_00 && schemaID == it->second.schemaId && schemaID != it->first)
		{
			if (alias.empty())
			{
				alias = it->first;
			}
			else
			{
				alias += " , " + it->first;
			}
		}
		else if (schemaID == it->second.legacySchemaId && schemaID != it->first)
		{
			if (alias.empty())
			{
				alias = it->first;
			}
			else
			{
				alias += " , " + it->first;
			}
		}
	}

	return alias;
}
