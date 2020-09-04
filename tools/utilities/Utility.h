/*
 * Copyright 2007-2020 United States Government as represented by the
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
		          << "\tRequired configuration option(s):"
		          << "\n"
		          << "\n\t\t* mw-id=<middleware>"
		          << "\n"
		          << "\n"
		          << "\tOptional configuration options:"
		          << "\n"
		          << "\n\t\t* server=<hostname or IP address as recognized by middleware>"
		          << "\n"
		          << "\n\t\t* loglevel=<level>"
		          << "\n"
		          << "\n\t\t* config=<file with additional configuration option(s)>"
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
					subjects.push_back("GMSEC.>");
					break;
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
					subjects.push_back("GMSEC.>");
					break;
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
				foundTerminate = (*it == "GMSEC.TERMINATE");
			}
			if (!foundTerminate)
			{
				if (schemaLevel == 1)
				{
					subjects.push_back("GMSEC.TERMINATE");
				}
				else
				{
					subjects.push_back("C2MS.TERMINATE");
				}
			}
		}
	}


	inline void addToConfigFromFile()
	{
		std::string cfgFilename = get("CONFIG"); // This assumes, now, that there is
		                                         // only _one_ CONFIG=filename.xml arg
		                                         // specified.

		if (!cfgFilename.empty())
		{
			std::ifstream fileStream(cfgFilename.c_str());
			std::string   contents;

			if (fileStream)
			{
				std::string line;

				while (std::getline(fileStream, line))
				{
					if (!line.empty())
					{
						contents += (line + "\n");
						GMSEC_DEBUG << line.c_str();
					}
				}

				fileStream.close();

				gmsec::api::Config configFromFile;

				configFromFile.fromXML(contents.c_str());

				//
				// name and value C strings are allocated,
				// and are supposed to be memory-handled in
				// general, in  GetFirst(), GetNext(),
				// and the destructor of the BaseConfig class.
				//
				const char* name  = NULL;
				const char* value = NULL;

				//
				// Read all the config file name, value pairs
				// and add them to the current base
				// configuration (which can have its
				// (name, value) pairs from both command-line
				// pairs and from the input current config 
				// file that was read).
				//
				bool foundEntry = configFromFile.getFirst(name,  value);

				while (foundEntry)
				{
					GMSEC_DEBUG << "Adding ("
					            << name << ", " << value
					            << ") from config file "
					            << cfgFilename.c_str();
					config.addValue(name, value);

					foundEntry = configFromFile.getNext(name, value);
				}
			}
			else
			{
				GMSEC_WARNING << "Non-valid-filepath config "
				              << "argument " << cfgFilename.c_str()
				              << " seen; ignoring it.";
			}
		}
	}


	inline void millisleep(int milliseconds)
	{
 		if (milliseconds < 1)
			milliseconds = 1;

		TimeUtil::millisleep(milliseconds);
	}


	bool isOptionInvalid(int numArgs)
	{
		std::string connType = get("connectionType");
		std::string mwID     = get("mw-id");

		return (numArgs <= 1 || (connType.empty() && mwID.empty()));
	}


	bool isOptionInvalid(int numArgs, const std::string& programName)
	{
		if (isOptionInvalid(numArgs))
		{
			return true;
		}

		if (programName.find("gmrpl") != std::string::npos ||
			programName.compare("gmsub") == 0 || 
			programName.compare("gmsub_cb") == 0)
		{
			if (get("MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER) == GMSEC_WAIT_FOREVER &&
				get("PROG_TIMEOUT_S", GMSEC_WAIT_FOREVER) != GMSEC_WAIT_FOREVER)
			{
				return true;
			}
		}

		return false;
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


	Message* readConfigFile(const std::string& cfgFileName, const std::string& msgName)
	{
		ConfigFile cfgFile;

		cfgFile.load(cfgFileName.c_str());

		return new Message(cfgFile.lookupMessage(msgName.c_str()));
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
} // namespace api
} // namespace gmsec

#endif // GMSEC_CPP4_EXAMPLE_H
