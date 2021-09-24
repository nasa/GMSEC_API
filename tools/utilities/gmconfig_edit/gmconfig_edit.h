/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** 
 * @file gmconfig_edit.h
 * 
 */

#ifndef GMSEC_CONFIG_EDIT_H
#define GMSEC_CONFIG_EDIT_H

#include "../Utility.h"

#include "callback.h"

#include <iostream>
#include <sstream>
#include <limits>
#include <string>
#include <vector>

// Forward Declaration(s)
namespace gmsec
{
namespace api
{
	class Config;
	class ConfigFile;
}
};


template <typename T>
struct MenuItem
{
	std::string description;
	Callback<T> callback;

	MenuItem(const char* desc, T* instance, bool (T::*funcPtr)())
		: description(desc)
	{
		callback.setCallback(instance, funcPtr);
	}
};


class GmsecConfigEdit : public gmsec::api::util::Utility
{
public:
	GmsecConfigEdit(const gmsec::api::Config& config);

	~GmsecConfigEdit();

	void processMenuSelection();

private:
	bool newConfiguration();
	bool loadConfiguration();

	bool addSubscription();
	bool removeSubscription();
	bool findSubscription();

	bool addConfiguration();
	bool removeConfiguration();
	bool findConfiguration();

	bool addMessage();
	bool removeMessage();
	bool findMessage();

	bool addCustomXML();
	bool removeCustomXML();
	bool findCustomXML();

	bool destroyConfiguration();

	bool saveConfiguration();
	bool saveConfigurationFile();

	bool quit();
	bool displayConfigFile();
	bool haveConfigFile() const;

	std::string menu() const;

	typedef std::vector<MenuItem<GmsecConfigEdit> > Menu;

	gmsec::api::ConfigFile* m_configFile;
	Menu                    m_menuItems;
	bool                    m_unsavedChanges;
};


template <typename T>
T getInput(const char* prompt)
{
	T    input;
	bool done = false;

	while (!done)
	{
		std::string tmp;
		std::cout << prompt;
		std::getline(std::cin, tmp);

		std::istringstream iss(tmp);
		iss >> input;
		done = !iss.fail();

		if (!done)
		{
			std::cout << "Invalid input; please try again...\n" << std::endl;
			std::cin.clear();
		}
	}

	return input;
}


template <>
std::string getInput<std::string>(const char* prompt)
{
	std::string input;

	std::cout << prompt;
	std::getline(std::cin, input);

	return input;
}


std::string getMultiLineInput(const char* prompt)
{
	std::string input;
	std::string newPrompt = std::string(prompt) + "\n<enter blank line to when done>\n\n";
	std::cout << newPrompt;

	bool done = false;

	while (!done)
	{
		std::string tmp;
		std::getline(std::cin, tmp);

		if (tmp.empty())
		{
			input = input.substr(0, input.length() - 1);  // remove trailing newline
			done = true;
		}
		else
		{
			input += tmp;
			input += std::string("\n");
		}
    }

	return input;
}

#endif
