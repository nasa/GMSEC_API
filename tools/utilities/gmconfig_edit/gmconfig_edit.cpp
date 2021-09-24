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

#include <gmsec4/Config.h>
#include <gmsec4/ConfigFile.h>

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


GmsecConfigEdit::GmsecConfigEdit(const gmsec::api::Config& config)
	: Utility(config),
	  m_configFile(new gmsec::api::ConfigFile()),
	  m_menuItems(),
	  m_unsavedChanges(false)
{
	initialize();

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


bool GmsecConfigEdit::newConfiguration()
{
	if (m_configFile)
	{
		delete m_configFile;
	}

	m_configFile = new gmsec::api::ConfigFile();

	m_unsavedChanges = false;

	return (m_configFile ? true : false);
}


bool GmsecConfigEdit::loadConfiguration()
{
	bool success = false;

	std::string filename = getInput<std::string>("Enter the configuration file to load: ");

	if (!m_configFile)
	{
		m_configFile = new gmsec::api::ConfigFile();
	}

	try
	{
		m_configFile->load(filename.c_str());
		success = true;
	}
	catch (gmsec::api::Exception& e)
	{
		std::cout << e.what() << std::endl;
	}

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

		try
		{
			m_configFile->addSubscription(name.c_str(), topic.c_str());
			success = true;
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
		}

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

		try
		{
			success = m_configFile->removeSubscription(name.c_str());

			if (success)
			{
				displayConfigFile();
				m_unsavedChanges = true;
			}
			else
			{
				std::cout << "Subscription entry with name '" << name << "' not found." << std::endl;
			}
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
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

		try {
			(void) m_configFile->lookupSubscriptionEntry(name.c_str());
			success = true;
			std::cout << "Subscription entry with name '" << name << "' exists in active configuration." << std::endl;
		}
		catch (...) {
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

		gmsec::api::Config config;

		for (size_t n = 0; n < numParams; ++n)
		{
			std::ostringstream paramNamePrompt;
			std::ostringstream paramValuePrompt;

			paramNamePrompt << "\nParameter " << (n+1) << " name: ";
			paramValuePrompt << "Parameter " << (n+1) << " value: ";

			const std::string paramName  = getInput<std::string>(paramNamePrompt.str().c_str());
			const std::string paramValue = getInput<std::string>(paramValuePrompt.str().c_str());

			config.addValue(paramName.c_str(), paramValue.c_str());
		}

		try
		{
			m_configFile->addConfig(name.c_str(), config);
			success = true;
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
		}

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

		try
		{
			success = m_configFile->removeConfig(name.c_str());

			if (success)
			{
				displayConfigFile();
				m_unsavedChanges = true;
			}
			else
			{
				std::cout << "Config entry with name '" << name << "' not found." << std::endl;
			}
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
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

		try
		{
			gmsec::api::Config config = m_configFile->lookupConfig(name.c_str());
			success = true;
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
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
		const std::string name  = getInput<std::string>("Enter the name of the message entry to add: ");
		const std::string topic = getInput<std::string>("Enter the message topic: ");
		const std::string type  = getInput<std::string>("Enter the message type (PUBLISH, REQUEST or REPLY): ");

		gmsec::api::Message::MessageKind msgKind;
		if (type == "PUBLISH")
		{
			msgKind = gmsec::api::Message::PUBLISH;
		}
		else if (type == "REQUEST")
		{
			msgKind = gmsec::api::Message::REQUEST;
		}
		else if (type == "REPLY")
		{
			msgKind = gmsec::api::Message::REPLY;
		}
		else
		{
			std::cout << "Unknown message kind: " << type << ".  Will default to using PUBLISH." << std::endl;
			msgKind = gmsec::api::Message::PUBLISH;
		}

		// Create a simple message with no fields.
		gmsec::api::Message msg(topic.c_str(), msgKind);

		// Add fields...
		const size_t numFields = getInput<size_t>("How many fields will this message have? ");

		for (size_t n = 0; n < numFields; ++n)
		{
			std::ostringstream fieldTypePrompt;
			std::ostringstream fieldNamePrompt;
			std::ostringstream fieldValuePrompt;

			fieldTypePrompt << "\nField " << (n+1) << " type (e.g. CHAR, I8, I16, I32, I64, U8, U16, U32, U64, F32, F64, STRING, BOOL, BIN): ";
			fieldNamePrompt << "Field " << (n+1) << " name: ";
			fieldValuePrompt << "Field " << (n+1) << " value: ";

			const std::string fieldType = getInput<std::string>(fieldTypePrompt.str().c_str());

			if (fieldType == "CHAR")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_CHAR  value     = getInput<GMSEC_CHAR>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "I8")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_I8    value     = getInput<GMSEC_I8>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "I16")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_I16   value     = getInput<GMSEC_I16>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "I32")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_I32   value     = getInput<GMSEC_I32>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "I64")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_I64   value     = getInput<GMSEC_I64>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "U8")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_U8    value     = getInput<GMSEC_U8>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "U16")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_U16   value     = getInput<GMSEC_U16>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "U32")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_U32   value     = getInput<GMSEC_U32>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "U64")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_U64   value     = getInput<GMSEC_U64>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "F32")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_F32   value     = getInput<GMSEC_F32>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "F64")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const GMSEC_F64   value     = getInput<GMSEC_F64>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value);
			}
			else if (fieldType == "STRING")
			{
				const std::string fieldName = getInput<std::string>(fieldNamePrompt.str().c_str());
				const std::string value     = getInput<std::string>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), value.c_str());
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
						msg.addField(fieldName.c_str(), value == "TRUE");
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
				std::string       value     = getInput<std::string>(fieldValuePrompt.str().c_str());

				msg.addField(fieldName.c_str(), (GMSEC_BIN) value.c_str(), value.size());
			}
			else
			{
				std::cout << "Error: Illegal field type..." << std::endl;
				n -= 1;
			}
		}

		try
		{
			m_configFile->addMessage(name.c_str(), msg);
			success = true;
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
		}

		if (success)
		{
			displayConfigFile();
			m_unsavedChanges = true;
		}
	}

	return success;
}


bool GmsecConfigEdit::removeMessage()
{
	bool success = false;

	if (haveConfigFile())
	{
		std::string name = getInput<std::string>("Enter the name of the message entry to remove: ");

		try
		{
			success = m_configFile->removeMessage(name.c_str());

			if (success)
			{
				displayConfigFile();
				m_unsavedChanges = true;
			}
			else
			{
				std::cout << "Message entry with name '" << name << "' not found." << std::endl;
			}
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
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

		try
		{
			gmsec::api::Message msg = m_configFile->lookupMessage(name.c_str());
			success = true;
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
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

		try
		{
			m_configFile->addCustomXML(xml.c_str());
			success = true;
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
		}

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

		try
		{
			success = m_configFile->removeCustomXML(xml.c_str());

			if (success)
			{
				displayConfigFile();
				m_unsavedChanges = true;
			}
			else
			{
				std::cout << "Custom XML element not found." << std::endl;
			}
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
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

		const gmsec::api::ConfigFileIterator& iter = m_configFile->getIterator();

		while (iter.hasNextCustomElement() && !success)
		{
			std::string customElement = iter.nextCustomElement();

			success = (xml == customElement);
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

	m_configFile = new gmsec::api::ConfigFile();

	displayConfigFile();

	return true;
}


bool GmsecConfigEdit::saveConfiguration()
{
	bool success = false;

	if (haveConfigFile() && m_unsavedChanges)
	{
		try
		{
			m_configFile->save();
			success = true;
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
		}

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

		try
		{
			m_configFile->save(filename.c_str());
			success = true;
		}
		catch (gmsec::api::Exception& e)
		{
			std::cout << e.what() << std::endl;
		}

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

	if (haveConfigFile())
	{
		const char* xml = m_configFile->toXML();

		if (xml)
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


int main(int argc, char** argv)
{
	gmsec::api::Config config(argc, argv);

	GmsecConfigEdit gcf(config);

	std::cout << "\nWelcome to gmconfig_edit!";

	while (true)
	{
		gcf.processMenuSelection();
	}
}
