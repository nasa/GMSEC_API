/*
 * Copyright 2007-2021 United States Government as represented by the
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

#ifndef gmsec_cxx_example_h
#define gmsec_cxx_example_h

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/select.h>
#endif

#include <time.h>
#include <gmsec_cpp.h>
#include <gmsec/util/sysutil.h>

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "gmsec/Config.h"

namespace example {

	const int EX_PROG_TIMEOUT_S = GMSEC_WAIT_FOREVER;
	const int EX_MSG_TIMEOUT_MS = GMSEC_WAIT_FOREVER;

class gmsec_exception: public std::exception {

public:
	
	~gmsec_exception() throw () {
		error = "";
	}

	gmsec_exception(const std::string &s) {
		error = s;
	}

	virtual const char* what() const throw () {
		return error.c_str();
	}

private:

	std::string error;

};


inline void printUsage(const std::string &programName) {
	std::cout << "\nusage: " << programName 
		<< " connectiontype=<middleware> [ <parameter>=<value> ]\n";
    std::cout << "\n\tNote that the parameter 'connectiontype' is required. "
		<< "\n\tThe rest of other parameters are optional.\n";

	std::cout << "\n\tserver=<server name> "
		<< "(required if middleware is not bolt/MB locally)";
	
	if (programName.find("threading") == std::string::npos){
		std::cout << "\n\tsubject=<subject name>";
	}
	if (programName.find("gmsub") != std::string::npos || 
			programName.compare("gmpub") == 0) {

		if (programName.find("gmsub_disp") == std::string::npos) 
			std::cout << "\n\titerations=<iterations>";

		if (programName.find("gmsub") != std::string::npos) {
			std::cout << "\n\tsubject.<N>=<sub-subject name>"
				<< "\n\t\tNote: N must be 1 or greater"
				<< "\n\t\tFor example, subject.1=GMSEC.A subject.2=GMSEC.B";
			if (programName.find("gmsub_disp") == std::string::npos){
				std::cout << "\n\tmsg_timeout_ms=<timeout period (milliseconds)>";
				std::cout << "\n\tprog_timeout_s=<timeout period (seconds)>"
						  << "\n\t\tNote: msg_timeout_ms must be defined to use prog_timeout_s";
			}
		}
		else {
			std::cout << "\n\tinterval_ms=<interval (milliseconds)>";
		}
	}

	if(programName.find("gmreq") != std::string::npos){
		std::cout << "\n\tmsg_timeout_ms=<timeout period (milliseconds)>"; 
	}
	else if (programName.compare("gmsub_disp_rr") == 0) {
		std::cout << "\n\tmsg_timeout_ms=<timeout period (milliseconds)>";
		std::cout << "\n\tprog_timeout_s=<timeout period (seconds)>";
	}
	else if (programName.find("gmrpl") != std::string::npos) {
		std::cout << "\n\tmsg_timeout_ms=<timeout period (milliseconds)>";
		std::cout << "\n\tprog_timeout_s=<timeout period (seconds)>"
		          << "\n\t\tNote: msg_timeout_ms must be defined to use prog_timeout_s"; 
	}

	if (programName.find("gmmist") != std::string::npos){
		std::cout << "\n\tGMSEC-MSG-CONTENT-VALIDATE=(true | false)";
		std::cout << "\n\tGMSEC-SPECIFICATION-VERSION=(201400), this parameter should only be";
		std::cout << "\n\t\tused if the user wishes to lock validation into a particular";
		std::cout << "\n\t\tversion of the GMSEC ISD, which is not recommended. Otherwise,";
		std::cout << "\n\t\tthe GMSEC API will validate against the most recent";
		std::cout << "\n\t\tspecification available.";
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
	std::cout << "\n\nFor more information, see API User's Guide\n";
}

inline bool checkNoThrow(const std::string &tag, const gmsec::Status &status) {

	if (!status.isError())
		return true;

	LOG_WARNING << tag << ": " << status.Get();

	return false;

}

inline bool check(const std::string &tag, const gmsec::Status &status) {

	if (!checkNoThrow(tag, status))
		throw gmsec_exception(tag + ": " + status.Get());

	return true;

}

inline std::string get(const gmsec::Config &config, const std::string &key) {

	const char * value = 0;
	gmsec::Status status = config.GetValue(key.c_str(), value);

	if (!status.isError() && value != NULL)
		return value;

	return "";

}

inline std::string get(const gmsec::Config &config, const std::string &key, const std::string &xdefault) {

	std::string value = get(config, key);

	if (value.empty())
		value = xdefault;

	return value;

}

inline int get(const gmsec::Config &config, const std::string &key, int xdefault) {

	int value = xdefault;

	std::string s = get(config, key);

	if (!s.empty()) {

		int tmp = 0;
		std::stringstream ss(s);
		ss >> tmp;

		if (!ss.fail()) {
			value = tmp;
		}

	}

	return value;

}

inline double get(const gmsec::Config &config, const std::string &key, double xdefault) {

	double value = xdefault;
	std::string s = get(config, key);

	if (!s.empty()) {

		double tmp = 0;
		std::stringstream ss(s);
		ss >> tmp;

		if (!ss.fail()) {
			value = tmp;
		}

	}

	return value;

}

inline void initialize(const gmsec::Config &config) {

	gmsec::Config temp = config;
	const char * tmp;

	if (temp.GetValue("LOGLEVEL", tmp).isError())
		temp.AddValue("LOGLEVEL", "INFO");

	if (temp.GetValue("LOGFILE", tmp).isError())
		temp.AddValue("LOGFILE", "STDERR");

}


inline std::string toString(int i) {

	std::ostringstream os;
	os << i;

	return os.str();
}


inline void determineSubjects(const gmsec::Config &config, std::vector<std::string> &subjects)
{
	std::string value = get(config, "SUBJECT");
	if (value.length() != 0) {
		subjects.push_back(value);
	}
	else {
		/* Subjects are to be input using SUBJECT.1="" SUBJECT.2="" etc */
		for (int i = 1; true; ++i) {
			std::string subject = std::string("SUBJECT.") + toString(i);
			std::string tmp = get(config, subject);
			if (tmp.length() != 0)
				subjects.push_back(tmp);
			else
				break;
		}
	}

	if (subjects.size() == 0)
		subjects.push_back("GMSEC.>");
	else
		subjects.push_back("GMSEC.TERMINATE");
}


inline void addToConfigFromFile(gmsec::Config& config)
{
	std::string cfgfilename = get(config, "CFGFILE"); // This assumes, now, that there is
	                                                  // only _one_ CFGFILE=filename.xml arg
	                                                  // specified.

	if (cfgfilename.length() != 0)
	{
		std::ifstream file_stream(cfgfilename.c_str());

		std::string   file_text_contents;

		if (file_stream.is_open())
		{
			std::string text_line;

			while (!file_stream.eof())
			{
				std::getline(file_stream, text_line);

				if (!file_stream.eof() &&
				    text_line != "")
				{
					file_text_contents += (text_line + "\n");
					LOG_DEBUG << text_line;
				}
			}

			file_stream.close();

			gmsec::Config config_from_file;

			config_from_file.FromXML(file_text_contents.c_str());

			//
			// name and value C strings are allocated,
			// and are supposed to be memory-handled in
			// general, in  GetFirst(), GetNext(),
			// and the destructor of the BaseConfig class.
			//
			const char* name = NULL;
			const char* value = NULL;

			//
			// Read all the config file name, value pairs
			// and add them to the current base
			// configuration (which can have its
			// (name, value) pairs from both command-line
			// pairs and from the input current config 
			// file that was read).
			//
			gmsec::Status status = config_from_file.GetFirst(name,  value);
			while (!status.IsError())
			{
				LOG_DEBUG << "Adding ("
				          << name << ", " << value
				          << ") from config file "
				          << cfgfilename;
				config.AddValue(name, value);

				status = config_from_file.GetNext(name, value);
			}
		}
		else
		{
			LOG_WARNING << "Non-valid-filepath config "
			            << "argument " << cfgfilename
			            << " seen; ignoring it.";
		}
	}// if there is a config file argument at all
}


inline void example_millisleep(int milliseconds)
{
 	if (milliseconds < 1)
		milliseconds = 1;

#ifdef WIN32
	Sleep(milliseconds);
#else
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = milliseconds * 1000;
	select(0, NULL, NULL, NULL, &tv);
#endif 
}

inline bool isOptionInvalid(const gmsec::Config &config, int nbr_of_args) {

  size_t conn_type_length = get(config, "connectionType").length();
  size_t mw_id_length = get(config, "mw-id").length();

  if (nbr_of_args <= 1 || (conn_type_length == 0 && mw_id_length == 0)) {
    return true;
  }

  return false;
}

inline bool isOptionInvalid(const gmsec::Config &config, int nbr_of_args, const std::string &programName) {

  size_t conn_type_length = get(config, "connectionType").length();
  size_t mw_id_length = get(config, "mw-id").length();

  if (nbr_of_args <= 1 || (conn_type_length == 0 && mw_id_length == 0)) {
    return true;
  }

  if (programName.find("gmrpl") != std::string::npos ||
	  programName.compare("gmsub") == 0 || 
	  programName.compare("gmsub_cb") == 0){
	if(get(config, "MSG_TIMEOUT_MS", EX_MSG_TIMEOUT_MS) == EX_MSG_TIMEOUT_MS &&
		get(config, "PROG_TIMEOUT_S", EX_PROG_TIMEOUT_S) != EX_PROG_TIMEOUT_S)
	{
		return true;
	}
  }

  return false;
}

} // namespace example

#endif /* gmsec_cxx_example_h */
