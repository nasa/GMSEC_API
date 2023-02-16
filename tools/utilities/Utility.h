/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Utility.h
 */

#ifndef GMSEC_CPP_UTILITY_H
#define GMSEC_CPP_UTILITY_H

#include <gmsec5_cpp.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


namespace gmsec
{
namespace api5
{
namespace util
{

static const size_t NUM_STANDARD_FIELDS = 8;

typedef struct StandardFields
{
	const char* fieldName;
	const char* fieldValue;
} StandardFields;

static const StandardFields sf[NUM_STANDARD_FIELDS] =
{
	{ "DOMAIN1", "DOM1" },
	{ "DOMAIN2", "DOM2" },
	{ "CONSTELLATION-ID", "CNS1" },
	{ "MISSION-ID", "MSSN" },
	{ "SAT-ID-PHYSICAL", "SAT1" },
	{ "SAT-ID-LOGICAL", "SAT1" },
	{ "FACILITY", "MY-FACILITY" },
	{ "COMPONENT", "MY-COMPONENT" }
};


class Utility
{
public:
	Utility(const Config& c)
		: config(c)
	{
	}


	virtual ~Utility()
	{
	}


	inline const Config& getConfig() const
	{
		return config;
	}


	void usage(const std::string& programName)
	{
		std::cerr << "\nError:\tMissing configuration option(s):"
		          << "\n\n\tFor configuration option information, refer to the API User's Guide."
		          << "\n"
		          << "\nUsage:\t" << programName << " <config option>=<value>"
		          << "\n"
		          << "\n"
		          << "\tRequired configuration option(s) must include one of the following groups:"
		          << "\n"
		          << "\n\t1. A choice of middleware:"
		          << "\n"
		          << "\n\t\t* mw-id=<middleware>"
		          << "\n"
		          << "\n\t2. A config option and optional cfg-name:"
		          << "\n"
		          << "\n\t\t* config=<file with one or more CONFIG entries>"
		          << "\n"
		          << "\n\t\t* cfg-name=<name of the configuration within the config file>"
		          << "\n"
		          << "\n";

		if (programName.compare("gmrpl") == 0 || programName.compare("gmsub") == 0)
		{
			std::cerr << "\t3. A cfg-file and cfg-name:"
			          << "\n"
			          << "\n\t\t* cfg-file=<file with additional configuration option(s)>"
			          << "\n"
			          << "\n\t\t* cfg-name=<name of the configuration within the cfg-file>";
		}
		else
		{
			std::cerr << "\t3. A cfg-file and cfg-name and/or msg-name:"
			          << "\n"
			          << "\n\t\t* cfg-file=<file with additional configuration option(s)>"
			          << "\n"
			          << "\n\t\t* cfg-name=<name of the configuration within the cfg-file>"
			          << "\n"
			          << "\n\t\t* msg-name=<name of the message within the cfg-file>";
		}

		std::cerr << "\n"
		          << "\n"
		          << "\tThe following configuration options are optional:"
		          << "\n"
		          << "\n\t\t* server=<hostname or IP address as recognized by middleware>"
		          << "\n"
		          << "\n\t\t* loglevel=<level>"
		          << "\n"
		          << "\n\t\t* msg-log-format=<xml | json>"
		          << std::endl;
	}


	void setStandardFields(MessageFactory& factory, const char* component)
	{
		StdUniquePtr<StringField> standardFields[NUM_STANDARD_FIELDS];
		List<Field*>              list;

		for (size_t i = 0; i < NUM_STANDARD_FIELDS; ++i)
		{
			if ((i+1) < NUM_STANDARD_FIELDS)
			{
				standardFields[i].reset( new StringField( sf[i].fieldName, sf[i].fieldValue, true ) );
			}
			else
			{
				standardFields[i].reset( new StringField( sf[i].fieldName, (component == NULL ? sf[i].fieldValue : component), true ) );
			}

			list.add(standardFields[i].get());
		}

		factory.setStandardFields(list);
	}


	std::string get(const std::string& key)
	{
		const char* value = config.getValue(key.c_str());

		return (value ? value : "");
	}


	std::string get(const std::string& key, const std::string& xdefault)
	{
		std::string value = get(key);

		return (value.empty() ? xdefault : value);
	}


	int get(const std::string& key, int xdefault)
	{
		return config.getIntegerValue(key.c_str(), xdefault);
	}


	double get(const std::string& key, double xdefault)
	{
		return config.getDoubleValue(key.c_str(), xdefault);
	}


	inline void initialize()
	{
		Config      tmpConfig = config;
		const char* logLevel  = tmpConfig.getValue(LOG_LEVEL);
		const char* logFile   = tmpConfig.getValue(LOG_FILE);

		if (!logLevel)
		{
			tmpConfig.addValue(LOG_LEVEL, "INFO");
		}
		if (!logFile)
		{
			tmpConfig.addValue(LOG_FILE, "STDERR");
		}
	}


	inline std::string toString(int i)
	{
		std::ostringstream os;
		os << i;
		return os.str();
	}


	inline void determineSubjects(unsigned int specVersion, Specification::SchemaLevel schemaLevel, std::vector<std::string>& subjects)
	{
		std::string value = get("SUBJECT");

		if (!value.empty())
		{
			subjects.push_back(value);
		}
		else
		{
			// Subjects are to be input using SUBJECT.1="" SUBJECT.2="" etc.
			for (int i = 1; true; ++i)
			{
				std::string subject = std::string("SUBJECT.") + toString(i);
				std::string tmp = get(subject);
				if (!tmp.empty())
				{
					subjects.push_back(tmp);
				}
				else
				{
					break;
				}
			}
		}

		if (subjects.size() == 0)
		{
			if (specVersion >= GMSEC_MSG_SPEC_2019_00)
			{
				switch (schemaLevel)
				{
				case Specification::SchemaLevel::LEVEL_0:
					subjects.push_back("C2MS.>");
					break;
				case Specification::SchemaLevel::LEVEL_1:
					subjects.push_back("C2MS-PIPE.>");
					break;
				case Specification::SchemaLevel::LEVEL_2:
				default:
					subjects.push_back("GMSEC.>");
					break;
				}
			}
			else if (specVersion == GMSEC_MSG_SPEC_2018_00)
			{
				switch (schemaLevel)
				{
				case Specification::SchemaLevel::LEVEL_0:
					subjects.push_back("C2MS.>");
					break;
				case Specification::SchemaLevel::LEVEL_1:
				default:
					subjects.push_back("GMSEC.>");
					break;
				}
			}
			else
			{
				subjects.push_back("GMSEC.>");
			}
		}
	}


	inline void determineExcludedSubjects(std::vector<std::string>& excludedSubjects)
	{
		std::string value = get("EXCLUDE-SUBJECT");

		if (!value.empty())
		{
			excludedSubjects.push_back(value);
		}
		else
		{
			// Excluded subjects are to be input using EXCLUDE-SUBJECT.1="" EXCLUDE-SUBJECT.2="" etc.
			for (int i = 1; true; ++i)
			{
				std::string subject = std::string("EXCLUDE-SUBJECT.") + toString(i);
				std::string tmp = get(subject);
				if (!tmp.empty())
				{
					excludedSubjects.push_back(tmp);
				}
				else
				{
					break;
				}
			}
		}
	}


	inline void addMultipleConfigsFromFile()
	{
		std::string cfgFilename = get("config");
		std::string cfgName     = get("cfg-name");

		if (!cfgFilename.empty())
		{
			std::ifstream ifs(cfgFilename.c_str());
			std::string   contents;

			if (ifs)
			{
				std::string data;
				std::string line;
				bool        keepLine = cfgName.empty();

				while (std::getline(ifs, line))
				{
					if (line.find("NAME=\"" + cfgName +"\"") != std::string::npos)
					{
						keepLine = true;

					}

					if (keepLine)
					{
						data += line + "\n";
					}

					if (keepLine && line.find("</CONFIG>") != std::string::npos)
					{
						break;
					}
				}

				gmsec::api5::Config configFromFile(data.c_str(), DataType::XML_DATA);
				config.merge(configFromFile);
			}
			else
			{
				throw GmsecException(CONFIG_ERROR, OTHER_ERROR_CODE, "Config File could not be found or could not be opened");
			}
		}
	}


	inline void addToConfigFromFile()
	{
		addMultipleConfigsFromFile();

		std::string cfgFilename  = get("cfg-file");
		std::string cfgEntryName = get("cfg-name");

		if (!cfgFilename.empty() && !cfgEntryName.empty())
		{
			try
			{
				gmsec::api5::ConfigFile configFile;
				configFile.load(cfgFilename.c_str());
				config.merge(configFile.lookupConfig(cfgEntryName.c_str()));
			}
			catch (const gmsec::api5::GmsecException& e)
			{
				GMSEC_WARNING << "Ignoring the contents of "
				              << cfgEntryName.c_str() << " in "
				              << cfgFilename.c_str() << ": " << e.what();
			}
		}
	}


	inline void millisleep(int milliseconds)
	{
 		if (milliseconds < 1)
			milliseconds = 1;

		TimeUtil::millisleep(milliseconds);
	}


	inline bool areOptionsValid(int numArgs)
	{
		if (numArgs < 2)
		{
			GMSEC_ERROR << "Missing argument(s)";
			return false;
		}

		std::string mwId    = get("mw-id");
		std::string config  = get("config");
		std::string cfgFile = get("cfg-file");
		std::string cfgName = get("cfg-name");
		std::string msgName = get("msg-name");

		if (mwId.empty() && config.empty() && cfgFile.empty())
		{
			GMSEC_ERROR << "Need to specify mw-id, config, or cfg-file";
			return false;
		}

		if (!mwId.empty() && !config.empty())
		{
			GMSEC_ERROR << "Cannot specify mw-id and config";
			return false;
		}

		if (!mwId.empty() && !cfgFile.empty() && msgName.empty())
		{
			GMSEC_ERROR << "Cannot specify mw-id and cfg-file, unless msg-name is specified";
			return false;
		}

		if (!config.empty() && !cfgFile.empty() && msgName.empty())
		{
			GMSEC_ERROR << "Cannot specify config and cfg-file";
			return false;
		}

		if (!cfgFile.empty() && cfgName.empty() && msgName.empty())
		{
			GMSEC_ERROR << "Usage of cfg-file requires cfg-name and/or msg-name";
			return false;
		}

		return true;
	}


	inline bool areOptionsValid(int numArgs, const std::string& programName)

	{
		if (!areOptionsValid(numArgs))
		{
			return false;
		}

		if (programName.compare("gmrpl") == 0 || programName.compare("gmsub") == 0 || programName.compare("gmsub_cb") == 0)


		{
			if (get("msg-timeout-ms", GMSEC_WAIT_FOREVER) == GMSEC_WAIT_FOREVER &&
				get("prog-timeout-s", GMSEC_WAIT_FOREVER) != GMSEC_WAIT_FOREVER)
			{
				GMSEC_ERROR << "prog-timeout-s should not be used when msg-timeout-ms is -1";
				return false;
			}
		}

		return true;
	}


	Message* readMessageFile(const std::string& msgFileName)
	{
		std::ifstream ifs(msgFileName.c_str(), std::ios::in);

		if (ifs)
		{
			std::string data;
			std::string line;

			while (std::getline(ifs, line))
			{
				data += line + "\n";
			}

			MessageFactory msgFactory( getConfig() );

			
			DataType type;
			if (data.find("<MESSAGE") != std::string::npos || data.find("<message") != std::string::npos)
			{
				type = DataType::XML_DATA;
			}
			else
			{
				type = DataType::JSON_DATA;
			}

			return new Message( msgFactory.fromData(data.c_str(), type) );
		}

		throw GmsecException(MSG_ERROR, OTHER_ERROR_CODE, "Message File could not be found or could not be opened");
	}


	Message* readConfigFile(const std::string& cfgFileName, const std::string& msgName)
	{
		ConfigFile cfgFile;

		cfgFile.load(cfgFileName.c_str());

		return new Message( cfgFile.lookupMessage(msgName.c_str()) );
	}


	void displayMessage(const std::string& text, const gmsec::api5::Message& message)
	{
		static const char* const FORMAT = "msg-log-format";

		if (std::string(getConfig().getValue(FORMAT, "xml")) == "json" ||
		    std::string(getConfig().getValue(FORMAT, "XML")) == "JSON")
		{
			GMSEC_INFO << text.c_str() << message.toJSON();
		}
		else
		{
			GMSEC_INFO << text.c_str() << message.toXML();
		}
	}


	std::vector<std::string> split(const std::string& str, const std::string& delimiter)
	{
		std::vector<std::string> ret;

		size_t begin = 0;
		size_t end = str.find(delimiter);

		while (end != std::string::npos)
		{
			ret.push_back(str.substr(begin, end - begin));

			begin = end + delimiter.length();
			end = str.find(delimiter, begin);
		}

		ret.push_back(str.substr(begin));

		return ret;
	}

private:
	Config config;
};

} // namespace utility
} // namespace api5
} // namespace gmsec

#endif
