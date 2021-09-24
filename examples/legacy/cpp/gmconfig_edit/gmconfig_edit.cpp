/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */





/** 
 * @file gmconfig_edit.cpp
 * 
 */

#include "gmconfig_edit.h"
#include "../example.h"

#include <gmsec_cpp.h>

#include <algorithm>
#include <sstream>
#include <iostream>
#include <cctype>

// For exit()
#ifdef __sun
	#include <stdlib.h>
#else
	#include <cstdlib>
#endif


bool GmsecConfigEdit::newConfiguration()
{
	if (m_configFile)
	{
		delete m_configFile;
	}

	m_configFile = new gmsec::ConfigFile;

	m_unsavedChanges = false;

	return (m_configFile ? true : false);
}


bool GmsecConfigEdit::loadConfiguration()
{
	bool success = false;

	std::string filename = getInput<std::string>("Enter the configuration file to load: ");

	if (!m_configFile)
	{
		m_configFile = new gmsec::ConfigFile;
	}

	success = example::checkNoThrow("", m_configFile->Load(filename.c_str()));

	if (success)
	{
		displayConfigFile();
	}

	m_unsavedChanges = false;

	return success;
}


bool GmsecConfigEdit::addSubscription()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string name  = getInput<std::string>("Enter the name of the subscription entry: ");
		std::string topic = getInput<std::string>("Enter the subscription topic: ");

		success = example::checkNoThrow("", m_configFile->AddSubscription(name.c_str(), topic.c_str()));

		if (success)
		{
			displayConfigFile();
			m_unsavedChanges = true;
		}
	}
	return success;
}


bool GmsecConfigEdit::removeSubscription()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string name = getInput<std::string>("Enter the name of the subscription entry to remove: ");

		success = example::checkNoThrow("", m_configFile->RemoveSubscription(name.c_str()));

		if (success)
		{
			displayConfigFile();
			m_unsavedChanges = true;
		}
	}
	return success;
}


bool GmsecConfigEdit::findSubscription()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string name = getInput<std::string>("Enter the name of the subscription entry to find: ");

		gmsec::ConfigFileIterator iter;

		m_configFile->Reset(iter);

		while (iter.HasNextSubscriptionName() && !success)
		{
			gmsec::util::String gmsecStr;

			iter.NextSubscriptionName(gmsecStr);

			if (name == gmsecStr.c_str())
			{
				success = true;
			}
		}

		if (success)
		{
			std::cout << "Subscription entry with name '" << name << "' exists in active configuration." << std::endl;
		}
		else
		{
			std::cout << "Subscription entry with name '" << name << "' not found in active configuration." << std::endl;
		}
	}
	
	return success;
}


bool GmsecConfigEdit::addConfiguration()
{
	bool success = false;

	if (haveConfigFile())
	{
		const std::string name = getInput<std::string>("Enter the name of the configuration entry to add: ");
		const size_t numParams = getInput<size_t>("How many parameter fields will this config entry have: ");

		gmsec::Config config;

		for (size_t n = 0; n < numParams; ++n)
		{
			std::ostringstream paramNamePrompt;
			std::ostringstream paramValuePrompt;

			paramNamePrompt << "\nParameter " << (n+1) << " name: ";
			paramValuePrompt << "Parameter " << (n+1) << " value: ";

			const std::string paramName  = getInput<std::string>(paramNamePrompt.str().c_str());
			const std::string paramValue = getInput<std::string>(paramValuePrompt.str().c_str());

			config.AddValue(paramName.c_str(), paramValue.c_str());
		}

		success = example::checkNoThrow("", m_configFile->AddConfig(name.c_str(), config));

		if (success)
		{
			displayConfigFile();
			m_unsavedChanges = true;
		}
	}
	return success;
}


bool GmsecConfigEdit::removeConfiguration()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string name = getInput<std::string>("Enter the name of the configuration entry to remove: ");

		success = example::checkNoThrow("", m_configFile->RemoveConfig(name.c_str()));

		if (success)
		{
			displayConfigFile();
			m_unsavedChanges = true;
		}
	}
	return success;
}


bool GmsecConfigEdit::findConfiguration()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string name = getInput<std::string>("Enter the name of the configuration entry to find: ");

		gmsec::ConfigFileIterator iter;

		m_configFile->Reset(iter);

		while (iter.HasNextConfigName() && !success)
		{
			gmsec::util::String gmsecStr;

			iter.NextConfigName(gmsecStr);

			if (name == gmsecStr.c_str())
			{
				success = true;
			}
		}

		if (success)
		{
			std::cout << "Configuration entry with name '" << name << "' exists in active configuration." << std::endl;
		}
		else
		{
			std::cout << "Configuration entry with name '" << name << "' not found in active configuration." << std::endl;
		}
	}
	
	return success;
}


bool GmsecConfigEdit::addMessage()
{
	bool success = false;

	if (haveConfigFile())
	{
		gmsec::Connection* conn = 0;
		gmsec::Config      config;

		config.AddValue("connectionType", "gmsec_bolt");

		gmsec::Status status = gmsec::ConnectionFactory::Create(&config, conn);

		if (status.IsError())
		{
			return false;
		}

		const std::string name  = getInput<std::string>("Enter the name of the message entry to add: ");
		const std::string topic = getInput<std::string>("Enter the message topic: ");
		const std::string type  = getInput<std::string>("Enter the message type (PUBLISH, REQUEST or REPLY): ");

		gmsec::Message* msg = 0;

		GMSEC_MSG_KIND msgKind = GMSEC_MSG_PUBLISH;
		if (type == "REQUEST")
		{
			msgKind = GMSEC_MSG_REQUEST;
		}
		else if (type == "REPLY")
		{
			msgKind = GMSEC_MSG_REPLY;
		}

		// Create a simple message with no fields.
		if (example::checkNoThrow("", conn->CreateMessage(topic.c_str(), msgKind, msg)))
		{
			const size_t numFields = getInput<size_t>("How many fields will this message have? ");

			for (size_t n = 0; n < numFields; ++n)
			{
				std::ostringstream fieldTypePrompt;
				std::ostringstream fieldNamePrompt;
				std::ostringstream fieldValuePrompt;

				fieldTypePrompt << "\nField " << (n+1) << " type (e.g. CHAR, I16, I32, U16, U32, I64, F32, F64, STRING, BOOL, BIN): ";
				fieldNamePrompt << "Field " << (n+1) << " name: ";
				fieldValuePrompt << "Field " << (n+1) << " value: ";

				const std::string fieldType = getInput<std::string>(fieldTypePrompt.str().c_str());

				if (fieldType == "CHAR")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					const GMSEC_CHAR value = getInput<GMSEC_CHAR>(fieldValuePrompt.str().c_str());
					gmsec::Field field(fieldName.c_str(), value);
					msg->AddField(field);
				}
				else if (fieldType == "I16")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					const GMSEC_I16 value = getInput<GMSEC_I16>(fieldValuePrompt.str().c_str());
					gmsec::Field field(fieldName.c_str(), value);
					msg->AddField(field);
				}
				else if (fieldType == "U16")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					const GMSEC_U16 value = getInput<GMSEC_U16>(fieldValuePrompt.str().c_str());
					gmsec::Field field(fieldName.c_str(), value);
					msg->AddField(field);
				}
				else if (fieldType == "I32")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					const GMSEC_I32 value = getInput<GMSEC_I32>(fieldValuePrompt.str().c_str());
					gmsec::Field field(fieldName.c_str(), value);
					msg->AddField(field);
				}
				else if (fieldType == "U32")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					const GMSEC_U32 value = getInput<GMSEC_U32>(fieldValuePrompt.str().c_str());
					gmsec::Field field(fieldName.c_str(), value);
					msg->AddField(field);
				}
				else if (fieldType == "I64")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					const GMSEC_I64 value = getInput<GMSEC_I64>(fieldValuePrompt.str().c_str());
					gmsec::Field field(fieldName.c_str(), value);
					msg->AddField(field);
				}
				else if (fieldType == "F32")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					const GMSEC_F32 value = getInput<GMSEC_F32>(fieldValuePrompt.str().c_str());
					gmsec::Field field(fieldName.c_str(), value);
					msg->AddField(field);
				}
				else if (fieldType == "F64")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					const GMSEC_F64 value = getInput<GMSEC_F64>(fieldValuePrompt.str().c_str());
					gmsec::Field field(fieldName.c_str(), value);
					msg->AddField(field);
				}
				else if (fieldType == "STRING")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					const std::string value = getInput<std::string>(fieldValuePrompt.str().c_str());
					gmsec::Field field(fieldName.c_str(), value.c_str());
					msg->AddField(field);
				}
				else if (fieldType == "BOOL")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					bool done = false;
					while (!done)
					{
						std::string value = getInput<std::string>(fieldValuePrompt.str().c_str());
						std::transform(value.begin(), value.end(), value.begin(), toupper);
						if (value == "TRUE" || value == "FALSE")
						{
							gmsec::Field field(fieldName.c_str(), (GMSEC_BOOL)(value == "TRUE"));
							msg->AddField(field);
							done = true;
						}
						else
						{
							std::cout << "Error: Illegal field value for BOOL; must be TRUE or FALSE...\n" << std::endl;
						}
					}
				}
				else if (fieldType == "BIN")
				{
					const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
					std::string value = getInput<std::string>(fieldValuePrompt.str().c_str());
					gmsec::Field field(fieldName.c_str(), (GMSEC_BIN) value.c_str(), (GMSEC_U32) value.size());
					msg->AddField(field);
				}
				else
				{
					std::cout << "Error: Illegal field type..." << std::endl;
					n -= 1;
				}
			}

			success = example::checkNoThrow("", m_configFile->AddMessage(name.c_str(), *msg));

			if (success)
			{
				displayConfigFile();
				m_unsavedChanges = true;
			}

			conn->DestroyMessage(msg);
		}

		gmsec::ConnectionFactory::Destroy(conn);
	}
	return success;
}


bool GmsecConfigEdit::removeMessage()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string name = getInput<std::string>("Enter the name of the message entry to remove: ");

		success = example::checkNoThrow("", m_configFile->RemoveMessage(name.c_str()));

		if (success)
		{
			displayConfigFile();
			m_unsavedChanges = true;
		}
	}
	return success;
}


bool GmsecConfigEdit::findMessage()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string name = getInput<std::string>("Enter the name of the message entry to find: ");

		gmsec::ConfigFileIterator iter;

		m_configFile->Reset(iter);

		while (iter.HasNextMessageName() && !success)
		{
			gmsec::util::String gmsecStr;

			iter.NextMessageName(gmsecStr);

			if (name == gmsecStr.c_str())
			{
				success = true;
			}
		}

		if (success)
		{
			std::cout << "Message entry with name '" << name << "' exists in active configuration." << std::endl;
		}
		else
		{
			std::cout << "Message entry with name '" << name << "' not found in active configuration." << std::endl;
		}
	}
	
	return success;
}


bool GmsecConfigEdit::addCustomXML()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string xml = getMultiLineInput("Enter custom XML content to add:");

		// Note: We need to replace any tab-characters with 4 white-space characters,
		// so as to mimic what would otherwise be done by TinyXML.
		size_t tab_pos = 0;
		while ((tab_pos = xml.find('\t')) != std::string::npos)
		{
			xml.replace(tab_pos, 1, "    ");
		}

		success = example::checkNoThrow("", m_configFile->AddCustomXML(xml.c_str()));

		if (success)
		{
			displayConfigFile();
			m_unsavedChanges = true;
		}
	}
	return success;
}


bool GmsecConfigEdit::removeCustomXML()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string xml = getMultiLineInput("Enter the custom XML string to remove: ");

		// Note: We need to replace any tab-characters with 4 white-space characters,
		// so as to mimic what would otherwise be done by TinyXML.
		size_t tab_pos = 0;
		while ((tab_pos = xml.find('\t')) != std::string::npos)
		{
			xml.replace(tab_pos, 1, "    ");
		}

		success = example::checkNoThrow("", m_configFile->RemoveCustomXML(xml.c_str()));

		if (success)
		{
			displayConfigFile();
			m_unsavedChanges = true;
		}
	}
	return success;
}


bool GmsecConfigEdit::findCustomXML()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string xml = getMultiLineInput("Enter the custom XML string to find: ");

		// Note: We need to replace any tab-characters with 4 white-space characters,
		// so as to mimic what would otherwise be done by TinyXML.
		size_t tab_pos = 0;
		while ((tab_pos = xml.find('\t')) != std::string::npos)
		{
			xml.replace(tab_pos, 1, "    ");
		}

		gmsec::ConfigFileIterator iter;

		m_configFile->Reset(iter);

		while (iter.HasNextCustomElement() && !success)
		{
			gmsec::util::String gmsecStr;

			iter.NextCustomElement(gmsecStr);

			if (xml == gmsecStr.c_str())
			{
				success = true;
			}
		}

		if (success)
		{
			std::cout << "Custom XML entry exists in active configuration." << std::endl;
		}
		else
		{
			std::cout << "Custom XML entry not found in active configuration." << std::endl;
		}
	}
	
	return success;
}


bool GmsecConfigEdit::destroyConfiguration()
{
	if (haveConfigFile())
	{
		delete m_configFile;
	}

	m_configFile = new gmsec::ConfigFile("");

	displayConfigFile();

	return true;
}


bool GmsecConfigEdit::saveConfiguration()
{
	bool success = false;

	if (haveConfigFile() && m_unsavedChanges)
	{
		success = example::checkNoThrow("", m_configFile->Save());

		if (success)
		{
			m_unsavedChanges = false;
		}
	}
	return success;
}


bool GmsecConfigEdit::saveConfigurationFile()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string filename = getInput<std::string>("Enter the file to save configuration: ");

		success = example::checkNoThrow("", m_configFile->Save(filename.c_str()));

		if (success)
		{
			m_unsavedChanges = false;
		}
	}
	return success;
}


bool GmsecConfigEdit::quit()
{
	if (m_unsavedChanges)
	{
		std::string doSave = getInput<std::string>("You have unsaved changes!  Are you sure you want to quit? [Y/N] ");

		if (doSave == "N" || doSave == "n" || doSave == "NO" || doSave == "no")
			return true;
	}

	std::cout << "\nBye!" << std::endl;
	exit(0);

	return true;
}


bool GmsecConfigEdit::displayConfigFile()
{
	std::cout << "\n"
	          << "--------------------------------------------------------------------------------------"
	          << "\n"
	          << std::endl;

	const char* xml = 0;
	if (haveConfigFile())
	{
		if (!m_configFile->ToXML(xml).IsError())
		{
	        std::cout << "Current Config File:\n\n" << xml << std::endl;
		}
		else
		{
			std::cout << "Config File is empty.\n" << std::endl;
		}
	}

	std::cout << "--------------------------------------------------------------------------------------"
	          << std::endl;

	return true;
}


bool GmsecConfigEdit::haveConfigFile() const
{
	if (m_configFile)
	{
		return true;
	}
	std::cout << "No Config File loaded.\n" << std::endl;
	return false;
}


GmsecConfigEdit::GmsecConfigEdit(int argc, char** argv)
	: m_configFile(new gmsec::ConfigFile("")),
	  m_menuItems(),
	  m_unsavedChanges(false)
{
	gmsec::Config config(argc, argv);

	example::initialize(config);

	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Create new configuration",        this, &GmsecConfigEdit::newConfiguration));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Load existing configuration",     this, &GmsecConfigEdit::loadConfiguration));

	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Add a subscription definition",   this, &GmsecConfigEdit::addSubscription));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Add a configuration definition",  this, &GmsecConfigEdit::addConfiguration));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Add a message definition",        this, &GmsecConfigEdit::addMessage));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Add a custom XML content",        this, &GmsecConfigEdit::addCustomXML));

	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Find subscription content",       this, &GmsecConfigEdit::findSubscription));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Find configuration content",      this, &GmsecConfigEdit::findConfiguration));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Find message content",            this, &GmsecConfigEdit::findMessage));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Find custom XML content",         this, &GmsecConfigEdit::findCustomXML));

	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Remove subscription definition",  this, &GmsecConfigEdit::removeSubscription));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Remove configuration definition", this, &GmsecConfigEdit::removeConfiguration));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Remove message definition",       this, &GmsecConfigEdit::removeMessage));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Remove custom XML content",       this, &GmsecConfigEdit::removeCustomXML));

	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Display current configuration",   this, &GmsecConfigEdit::displayConfigFile));

	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Destroy current configuration",   this, &GmsecConfigEdit::destroyConfiguration));

	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Save configuration to file",      this, &GmsecConfigEdit::saveConfiguration));
	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Save configuration to new file",  this, &GmsecConfigEdit::saveConfigurationFile));

	m_menuItems.push_back(MenuItem<GmsecConfigEdit>("Quit",                            this, &GmsecConfigEdit::quit));
}


GmsecConfigEdit::~GmsecConfigEdit()
{
	delete m_configFile;
}


std::string GmsecConfigEdit::menu() const
{
	static std::ostringstream menu;

	if (menu.str().empty())
	{
		for (size_t i = 0; i < m_menuItems.size(); ++i)
		{
			size_t index = i + 1;
			menu << (index < 10 ? " " : "") << index << ")  " << m_menuItems[i].description << "\n";
		}
	}

	return menu.str();
}


void GmsecConfigEdit::processMenuSelection()
{
	std::cout << "\n\nPlease choose one of the following options:\n\n";

	std::cout << menu() << std::endl;

	bool validSelection = false;
	size_t choice = 0;
	while (!validSelection)
	{
		choice = getInput<size_t>(">> ");

		validSelection = (choice > 0 && choice <= m_menuItems.size());

		if (!validSelection)
		{
			std::cout << "Invalid selection; please try again...\n" << std::endl;
		}
	}

	bool result = m_menuItems[choice-1].callback.execute();

	std::cout << (result ? "Ok" : "Fail") << ".\n" << std::endl;

	std::string enter;
	std::cout << "Press <enter> to continue...";
	std::getline(std::cin, enter);
}


int main(int argc, char** argv)
{
	try
	{
		GmsecConfigEdit gcf(argc, argv);

		std::cout << "\nWelcome to gmconfig_edit!";
		while (true)
		{
			gcf.processMenuSelection();
		}
	}
	catch (example::gmsec_exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
		return 1;
	}
}
