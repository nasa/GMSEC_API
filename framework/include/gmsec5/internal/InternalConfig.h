/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API5_INTERNAL_CONFIG_H
#define GMSEC_API5_INTERNAL_CONFIG_H

#include <gmsec5/internal/ci_less.h>
#include <gmsec5/internal/json-forwards.h>
#include <gmsec5/internal/tinyxml2.h>

#include <gmsec5_defs.h>
#include <gmsec5/Config.h>

#include <gmsec5/util/wdllexp.h>

#include <string>
#include <map>
#include <vector>


namespace gmsec
{
namespace api5
{
namespace internal
{

class GMSEC_API InternalConfig
{
public:
	InternalConfig();

	InternalConfig(int argc, char* argv[]);

	InternalConfig(const char* data, DataType type);

	InternalConfig(const InternalConfig& cfg);

	virtual ~InternalConfig();

	void CALL_TYPE addValue(const char* name, const char* value);

	bool CALL_TYPE clearValue(const char* name);

	const char* CALL_TYPE getValue(const char* name) const;
	const char* CALL_TYPE getValue(const char* name, const char* defaultValue) const;

	bool CALL_TYPE getBooleanValue(const char* name) const;
	bool CALL_TYPE getBooleanValue(const char* name, bool defaultValue) const;

	int CALL_TYPE getIntegerValue(const char* name) const;
	int CALL_TYPE getIntegerValue(const char* name, int defaultValue) const;

	double CALL_TYPE getDoubleValue(const char* name) const;
	double CALL_TYPE getDoubleValue(const char* name, double defaultValue) const;

	void CALL_TYPE clear();

	bool CALL_TYPE getFirst(const char*& name, const char*& value) const;

	bool CALL_TYPE getNext(const char*& name, const char*& value) const;


	void CALL_TYPE merge(const gmsec::api5::Config& other, bool overwriteExisting = true);

	const char* CALL_TYPE toXML() const;

	void CALL_TYPE fromXML(const char* xml);

	void CALL_TYPE fromXML(tinyxml2::XMLElement* element);

	static Config CALL_TYPE getFromFile(const char* filename, const char* configName);

	const char* CALL_TYPE toJSON(bool standalone = true) const;

	void CALL_TYPE fromJSON(const Json::Value& root);


private:
	// declared, but not implemented.
	InternalConfig& operator=(const InternalConfig&);

	void validateConfigName(const char* name) const;

	void getKeyValuePairs(const std::string& data, std::vector<std::string>& pairs);

	typedef std::map<std::string, std::string, ci_less> ConfigMap;
	typedef ConfigMap::const_iterator                   ConfigMapIter;

	ConfigMap             m_configs;
	std::string           m_configName;
	mutable ConfigMapIter m_configIter;
	mutable std::string   m_xml;
	mutable std::string   m_json;

};

} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
