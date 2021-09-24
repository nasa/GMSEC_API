/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





#ifndef GMHELP_H
#define GMHELP_H

#include <set>
#include <map>
#include <string>

struct ConfigOption
{
	std::string name;
	std::string briefDescription;
	std::string detailedDescription;
	std::string example;

	bool operator<(const ConfigOption& opt) const
	{
        return name < opt.name;
	}
};

typedef std::set<ConfigOption> ConfigOptions;


struct MiddlewareOptions
{
	ConfigOptions client;
	ConfigOptions server;
};


typedef std::map<std::string, MiddlewareOptions> HelpMap;


class MiddlewareHelp
{
public:
	MiddlewareHelp();
	int  processUserRequest(int argc, char** argv) const;

private:
	void showUsage(std::string progName) const;

	void showHelp(const std::string& mw, const std::string& jms_mw) const;
	void showDetailedHelp(const std::string& mw, const std::string& jms_mw, const std::string& parameter) const;

	void displayOptions(const std::string& mw_name, const std::string& jms_mw, const MiddlewareOptions& mw_opts) const;
	void displayOption(const std::string& mw_name, const ConfigOption& cfg_opt, bool showDetails) const;

	HelpMap m_helpMap;
};

#endif
