/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file Utility.h
 * 
 * A C++ class containing a namespace, which collects several
 * functions used throughout the C++ GMSEC examples.
 * 
 */

#ifndef GMSEC_CPP_UTILITY_H
#define GMSEC_CPP_UTILITY_H

#include <gmsec4_cpp.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>


namespace gmsec
{
namespace api
{
namespace util
{


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
		          << "\n"
		          << "\t1. A choice of middleware:"
		          << "\n"
		          << "\n\t\t* mw-id=<middleware>"
		          << "\n"
		          << "\n"
		          << "\t2. A config option and optional cfg-name:"
		          << "\n"
		          << "\n\t\t* config=<file with one or more CONFIG entries>"
		          << "\n\t\t* cfg-name=<name of the configuration within the config file>"
		          << "\n"
		          << "\n";

		if (programName.compare("gmrpl") == 0 || programName.compare("gmsub") == 0 || programName.compare("gmsub_cb") == 0)
		{
			std::cerr << "\t3. A cfg-file and cfg-name:"
		              << "\n"
		              << "\n\t\t* cfg-file=<file with additional configuration option(s)>"
		              << "\n\t\t* cfg-name=<name of the configuration within the cfg-file>";
		}
		else
		{
			std::cerr << "\t3. A cfg-file and cfg-name and/or msg-name:"
		              << "\n"
		              << "\n\t\t* cfg-file=<file with additional configuration option(s)>"
		              << "\n\t\t* cfg-name=<name of the configuration within the cfg-file>"
		              << "\n\t\t* msg-name=<name of the message within the cfg-file>";
		}

		std::cerr << "\n"
		          << "\n"
		          << "\tThe following configuration options are optional:"
		          << "\n"
		          << "\n\t\t* server=<hostname or IP address as recognized by middleware>"
		          << "\n"
		          << "\n\t\t* loglevel=<level>"
		          << std::endl;
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
		gmsec::api::Config tmpConfig = config;
		const char*        logLevel  = tmpConfig.getValue("LOGLEVEL");
		const char*        logFile   = tmpConfig.getValue("LOGFILE");

		if (!logLevel)
		{
			tmpConfig.addValue("LOGLEVEL", "INFO");
		}
		if (!logFile)
		{
			tmpConfig.addValue("LOGFILE", "STDERR");
		}
	}


	inline std::string toString(int i)
	{
		std::ostringstream os;
		os << i;
		return os.str();
	}


	inline void determineSubjects(unsigned int specVersion, mist::Specification::SchemaLevel schemaLevel, std::vector<std::string>& subjects)
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
			if (specVersion >= mist::GMSEC_ISD_2019_00)
			{
				switch (schemaLevel)
				{
				case mist::Specification::LEVEL_0:
					subjects.push_back("C2MS.>");
					break;
				case mist::Specification::LEVEL_1:
					subjects.push_back("C2MS-11B.>");
					break;
				case mist::Specification::LEVEL_2:
				default:
					subjects.push_back("GMSEC.>");
					break;
				}
			}
			else if (specVersion == mist::GMSEC_ISD_2018_00)
			{
				switch (schemaLevel)
				{
				case mist::Specification::LEVEL_0:
					subjects.push_back("C2MS.>");
					break;
				case mist::Specification::LEVEL_1:
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
		else
		{
			// Include topic to GMSEC.TERMINATE, but only if the user has not provided it.

			bool foundTerminate = false;
			for (std::vector<std::string>::iterator it = subjects.begin(); it != subjects.end() && !foundTerminate; ++it)
			{
				foundTerminate = ((*it).find(".TERMINATE") != std::string::npos);
			}
			if (!foundTerminate)
			{
				if (specVersion >= mist::GMSEC_ISD_2019_00)
				{
					switch (schemaLevel)
					{
					case mist::Specification::LEVEL_0:
						subjects.push_back("C2MS.TERMINATE");
						break;
					case mist::Specification::LEVEL_1:
						subjects.push_back("C2MS-11B.TERMINATE");
						break;
					case mist::Specification::LEVEL_2:
					default:
						subjects.push_back("GMSEC.TERMINATE");
						break;
					}
				}
				else if (specVersion == mist::GMSEC_ISD_2018_00)
				{
					switch (schemaLevel)
					{
					case mist::Specification::LEVEL_0:
						subjects.push_back("C2MS.TERMINATE");
						break;
					case mist::Specification::LEVEL_1:
					default:
						subjects.push_back("GMSEC.TERMINATE");
						break;
					}
				}
				else
				{
					subjects.push_back("GMSEC.TERMINATE");
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
			std::ifstream ifs(cfgFilename.c_str(), std::ios::in);

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

				gmsec::api::Config configFromFile(data.c_str());
				config.merge(configFromFile);
			}
			else
			{
				throw Exception(CONFIG_ERROR, OTHER_ERROR_CODE, "Config File could not be found or could not be opened");
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
                gmsec::api::ConfigFile configFile;
                configFile.load(cfgFilename.c_str());
                config.merge(configFile.lookupConfig(cfgEntryName.c_str()));
            }
            catch (const gmsec::api::Exception& e)
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


	inline Message* readMessageFile(const std::string& msgFileName)
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

			// Now attempt to build the message (using XML or JSON)
			std::string hasSchemaLevel = get("GMSEC-SCHEMA-LEVEL", "");
			std::string hasSpecVersion = get("GMSEC-SPECIFICATION-VERSION", "");

			if (hasSchemaLevel.empty() && hasSpecVersion.empty())
			{
				// Build a regular message if no configuration is provided
				return new Message(data.c_str());
			}
			return new mist::message::MistMessage(mist::Specification(getConfig()), data.c_str());
		}

		throw Exception(MSG_ERROR, OTHER_ERROR_CODE, "Message File could not be found or could not be opened");
	}


	inline Message* readConfigFile(const std::string& cfgFileName, const std::string& msgName)
	{
		ConfigFile cfgFile;

		cfgFile.load(cfgFileName.c_str());

		return new Message(cfgFile.lookupMessage(msgName.c_str()));
	}


	inline std::vector<std::string> split(const std::string& str, const std::string& delimiter)
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
} // namespace api
} // namespace gmsec

#endif // GMSEC_CPP4_EXAMPLE_H
