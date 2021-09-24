/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API_MIST_SCHEMA_ID_MAPPING_H
#define GMSEC_API_MIST_SCHEMA_ID_MAPPING_H


#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/util/Log.h>

#include <map>
#include <string>


namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

struct SchemaId
{
	SchemaId() {}

	SchemaId(const std::string& id1, const std::string& id2)
		: schemaId(id1), legacySchemaId(id2)
	{
	}

	SchemaId(const SchemaId& other)
		: schemaId(other.schemaId), legacySchemaId(other.legacySchemaId)
	{
	}

	std::string schemaId;
	std::string legacySchemaId;
};

struct SchemaIdMapping
{
	typedef std::map<std::string, SchemaId> SchemaMap;

	SchemaMap m_schemaMap;

	SchemaIdMapping()
		: m_schemaMap()
	{
		m_schemaMap["AMVAL"]              = SchemaId("MSG.AMVAL", "MSG.AMVAL");
		m_schemaMap["CMDECHO"]            = SchemaId("MSG.CMDECHO", "MSG.CMDECHO");
		m_schemaMap["LOG"]                = SchemaId("MSG.LOG", "MSG.LOG");
		m_schemaMap["MVAL"]               = SchemaId("MSG.MVAL", "MSG.MVAL");

		m_schemaMap["CFG"]                = SchemaId("MSG.CFG", "MSG.C2CX.CFG");
		m_schemaMap["MSG.CFG"]            = SchemaId("MSG.CFG", "MSG.C2CX.CFG");
		m_schemaMap["MSG.C2CX.CFG"]       = SchemaId("MSG.CFG", "MSG.C2CX.CFG");

		m_schemaMap["CNTL"]               = SchemaId("MSG.CNTL", "MSG.C2CX.CNTL");
		m_schemaMap["MSG.CNTL"]           = SchemaId("MSG.CNTL", "MSG.C2CX.CNTL");
		m_schemaMap["MSG.C2CX.CNTL"]      = SchemaId("MSG.CNTL", "MSG.C2CX.CNTL");

		m_schemaMap["DEV"]                = SchemaId("MSG.DEV", "MSG.C2CX.DEV");
		m_schemaMap["MSG.DEV"]            = SchemaId("MSG.DEV", "MSG.C2CX.DEV");
		m_schemaMap["MSG.C2CX.DEV"]       = SchemaId("MSG.DEV", "MSG.C2CX.DEV");

		m_schemaMap["HB"]                 = SchemaId("MSG.HB", "MSG.C2CX.HB");
		m_schemaMap["MSG.HB"]             = SchemaId("MSG.HB", "MSG.C2CX.HB");
		m_schemaMap["MSG.C2CX.HB"]        = SchemaId("MSG.HB", "MSG.C2CX.HB");

		m_schemaMap["RSRC"]               = SchemaId("MSG.RSRC", "MSG.C2CX.RSRC");
		m_schemaMap["MSG.RSRC"]           = SchemaId("MSG.RSRC", "MSG.C2CX.RSRC");
		m_schemaMap["MSG.C2CX.RSRC"]      = SchemaId("MSG.RSRC", "MSG.C2CX.RSRC");

		m_schemaMap["TLMFRAME"]           = SchemaId("MSG.TLMFRAME", "MSG.TLM.CCSDSFRAME");
		m_schemaMap["MSG.TLMFRAME"]       = SchemaId("MSG.TLMFRAME", "MSG.TLM.CCSDSFRAME");
		m_schemaMap["MSG.TLM.CCSDSFRAME"] = SchemaId("MSG.TLMFRAME", "MSG.TLM.CCSDSFRAME");

		m_schemaMap["TLMPKT"]             = SchemaId("MSG.TLMPKT", "MSG.TLM.CCSDSPKT");
		m_schemaMap["MSG.TLMPKT"]         = SchemaId("MSG.TLMPKT", "MSG.TLM.CCSDSPKT");
		m_schemaMap["MSG.TLM.CCSDSPKT"]   = SchemaId("MSG.TLMPKT", "MSG.TLM.CCSDSPKT");

		m_schemaMap["TLMPROC"]            = SchemaId("MSG.TLMPROC", "MSG.TLM.PROCESSED");
		m_schemaMap["MSG.TLMPROC"]        = SchemaId("MSG.TLMPROC", "MSG.TLM.PROCESSED");
		m_schemaMap["MSG.TLM.PROCESSED"]  = SchemaId("MSG.TLMPROC", "MSG.TLM.PROCESSED");

		m_schemaMap["TLMTDM"]             = SchemaId("MSG.TLMTDM", "MSG.TLM.TDM");
		m_schemaMap["MSG.TLMTDM"]         = SchemaId("MSG.TLMTDM", "MSG.TLM.TDM");
		m_schemaMap["MSG.TLM.TDM"]        = SchemaId("MSG.TLMTDM", "MSG.TLM.TDM");

		/* TODO: Wait for OMG C2MS 1.1 to see if NDM messages are flattened.
		m_schemaMap["AEM"]                = SchemaId("MSG.NDM.AEM", "MSG.NDM.AEM");
		m_schemaMap["APM"]                = SchemaId("MSG.NDM.APM", "MSG.NDM.APM");
		m_schemaMap["OEM"]                = SchemaId("MSG.NDM.OEM", "MSG.NDM.OEM");
		m_schemaMap["OMM"]                = SchemaId("MSG.NDM.OMM", "MSG.NDM.OMM");
		m_schemaMap["OPM"]                = SchemaId("MSG.NDM.OPM", "MSG.NDM.OPM");
		m_schemaMap["TDM"]                = SchemaId("MSG.NDM.TDM", "MSG.NDM.TDM");
		*/
	}

	/**
	 * @fn std::string getSchemaId(const std::string& id, unsigned int msgSpecVersion) const
	 * @brief Lookup message schema ID using alias, or actual schema ID. If nothing found, then just return given ID.
	 * @param id - schema ID (alias or actual)
	 * @param msgSpecVersion - version of the message specification
	 * @return The schema ID needed to reference the message template, or the given schema ID.
	 */
	std::string getSchemaId(const std::string& id, unsigned int msgSpecVersion) const
	{
		std::string schemaId = id;  // could be alias

		SchemaMap::const_iterator it = m_schemaMap.find(id);

		if (it != m_schemaMap.end())
		{
			schemaId = (msgSpecVersion >= GMSEC_ISD_2019_00 ? it->second.schemaId : it->second.legacySchemaId);
		}

		return schemaId;
	}
};

}  // end namespace internal
}  // end namespace mist
}  // end namespace api
}  // end namespace gmsec

#endif
