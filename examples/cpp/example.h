/*
 * Copyright 2007-2017 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file example.h
 * 
 * A C++ class containing a namespace, which collects several
 * functions used throughout the C++ GMSEC examples.
 * 
 */

#ifndef GMSEC_CPP4_EXAMPLE_H
#define GMSEC_CPP4_EXAMPLE_H

#include <gmsec4_cpp.h>

#include <gmsec4/util/TimeUtil.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <time.h>


namespace gmsec
{
namespace api
{
namespace example
{


inline void printUsage(const std::string &programName)
{
	std::cout << "\nusage: " << programName 
	          << " connectiontype=<middleware> [ <parameter>=<value> ]\n"
              << "\n\tNote that the parameter 'connectiontype' is required. "
	          << "\n\tThe rest of other parameters are optional.\n"
	          << "\n\tserver=<server name> "
	          << "(required if middleware is not bolt/MB locally)";
	
	if (programName.find("threading") == std::string::npos)
	{
		std::cout << "\n\tsubject=<subject name>";
	}
	if (programName.find("gmsub") != std::string::npos || programName.find("gmpub") != std::string::npos)
	{
		if (programName.find("gmsub_disp") == std::string::npos) 
		{
			std::cout << "\n\titerations=<iterations>";
		}

		if (programName.find("gmsub") != std::string::npos)
		{
			std::cout << "\n\tsubject.<N>=<sub-subject name>"
			          << "\n\t\tNote: N must be 1 or greater"
			          << "\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B";
			if (programName.find("gmsub_disp") == std::string::npos)
			{
				std::cout << "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
				          << "\n\tprog_timeout_s=<timeout period (seconds)>"
				          << "\n\t\tNote: msg_timeout_ms must be defined to use prog_timeout_s";
			}
		}
		else
		{
			std::cout << "\n\tinterval_ms=<interval (milliseconds)>";
		}
	}
	if (programName.find("throughput_pub") != std::string::npos)
	{
		std::cout << "\n\tdata-size=<number of bytes to package into messages>";
	}
	if (programName.find("throughput") != std::string::npos)
	{
		std::cout << "\n\tmonitor-rate=<period of time (ms) to report monitored throughput>";
	}
	if (programName.find("gmreq") != std::string::npos)
	{
		std::cout << "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"; 
	}
	else if (programName.compare("gmsub_disp_rr") == 0)
	{
		std::cout << "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
		          << "\n\tprog_timeout_s=<timeout period (seconds)>";
	}
	else if (programName.find("gmrpl") != std::string::npos)
	{
		std::cout << "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"
		          << "\n\tprog_timeout_s=<timeout period (seconds)>"
		          << "\n\t\tNote: msg_timeout_ms must be defined to use prog_timeout_s"; 
	}

	if (programName.find("gmmist") != std::string::npos)
	{
		std::cout << "\n\tGMSEC-MSG-CONTENT-VALIDATE=(true | false)"
		          << "\n\tGMSEC-SPECIFICATION-VERSION=(201400), this parameter should only be"
		          << "\n\t\tused if the user wishes to lock validation into a particular"
		          << "\n\t\tversion of the GMSEC ISD, which is not recommended. Otherwise,"
		          << "\n\t\tthe GMSEC API will validate against the most recent"
		          << "\n\t\tspecification available.";
	}

	std::cout << "\n\tloglevel=<log level>";

	if (programName.find("threading") != std::string::npos ||
        programName.find("gmmist_services") != std::string::npos ||
	    programName.find("gmmist_validation") != std::string::npos ||
	    programName.find("gmpub") != std::string::npos ||
	    programName.find("gmreq") != std::string::npos ||
	    programName.find("gmrpl") != std::string::npos ||
	    programName.find("gmsub") != std::string::npos ||
	    programName.find("logging") != std::string::npos)
	{
		std::cout << std::endl << "\tcfgfile=<config_filepath.xml>";
	}

	if (programName.find("wsmq_async") != std::string::npos)
	{
		std::cout << "\n\tmw-async-publish=<true | false>"
		<< "\n\tmw-async-status-check=<true | false>"
		<< "\n\tmw-async-status-check-message-interval=<number of publish operations before checking asynchronous put status>";
	}

	std::cout << "\n\nFor more information, see API User's Guide" << std::endl;
}


inline std::string get(const gmsec::api::Config& config, const std::string& key)
{
	const char* value = config.getValue(key.c_str());

	return (value ? value : "");
}


inline std::string get(const gmsec::api::Config& config, const std::string& key, const std::string& xdefault)
{
	std::string value = get(config, key);

	return (value.empty() ? xdefault : value);
}


inline int get(const gmsec::api::Config& config, const std::string& key, int xdefault)
{
	return config.getIntegerValue(key.c_str(), xdefault);
}


inline double get(const gmsec::api::Config& config, const std::string& key, double xdefault)
{
	return config.getDoubleValue(key.c_str(), xdefault);
}


inline void initialize(const gmsec::api::Config& config)
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


inline void determineSubjects(const gmsec::api::Config& config, std::vector<std::string>& subjects)
{
	std::string value = get(config, "SUBJECT");

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
			std::string tmp = get(config, subject);
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
		subjects.push_back("GMSEC.>");
	}
	else
	{
		subjects.push_back("GMSEC.TERMINATE");
	}
}


inline void addToConfigFromFile(gmsec::api::Config& config)
{
	std::string cfgFilename = get(config, "CFGFILE"); // This assumes, now, that there is
	                                                  // only _one_ CFGFILE=filename.xml arg
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

	gmsec::api::util::TimeUtil::millisleep(milliseconds);
}


inline bool isOptionInvalid(const gmsec::api::Config& config, int nbr_of_args)
{
	size_t conn_type_length = get(config, "connectionType").length();
	size_t mw_id_length = get(config, "mw-id").length();

	if (nbr_of_args <= 1 || (conn_type_length == 0 && mw_id_length == 0))
	{
		return true;
	}

	return false;
}


inline bool isOptionInvalid(const gmsec::api::Config& config, int nbr_of_args, const std::string& programName)
{
	size_t conn_type_length = get(config, "connectionType").length();
	size_t mw_id_length = get(config, "mw-id").length();

	if (nbr_of_args <= 1 || (conn_type_length == 0 && mw_id_length == 0))
	{
		return true;
	}

	if (programName.find("gmrpl") != std::string::npos ||
		programName.compare("gmsub") == 0 || 
		programName.compare("gmsub_cb") == 0)
	{
		if (get(config, "MSG_TIMEOUT_MS", GMSEC_WAIT_FOREVER) == GMSEC_WAIT_FOREVER &&
			get(config, "PROG_TIMEOUT_S", GMSEC_WAIT_FOREVER) != GMSEC_WAIT_FOREVER)
		{
			return true;
		}
	}

	return false;
}

} // namespace example
} // namespace api
} // namespace gmsec

#endif // GMSEC_CPP4_EXAMPLE_H
