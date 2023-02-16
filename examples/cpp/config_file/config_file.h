/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/**
 * @file config_file.h
 *
 *  @brief A Demo for the ConfigFile class
 */

#ifndef GMSEC_API5_EXAMPLE_CONFIG_FILE_H
#define GMSEC_API5_EXAMPLE_CONFIG_FILE_H

#include <string>
#include <vector>
#include <iostream>

#include <gmsec5_cpp.h>

#include <gmsec5/ConfigFile.h>

class config_file
{
public:
	/**
	 * @fn config_file()
	 *
	 * @brief Constructor, provide the file name of interest. Open the test file. May throw a GMSECException.
	 */
	config_file(const std::string &configFilename);


	/**
	 * @fn ~config_file()
	 * @brief Destructor.
	 */
	~config_file();


	/**
	 * Print the named subscription entry contents or print that it does not exist. May throw a GMSECException.
	 * @param subscriptionName a String
	 */
	void showSubscription(const std::string &subscriptionName);


	/**
	 * Print the named config contents or print that it does not exist. May throw a GMSECException.
	 * @param configName a String
	 */
	void showConfig(const std::string &configName);


	/**
	 * Print the named message contents or print that it does not exist. May throw a GMSECException.
	 * @param configName a String
	 */
	void showMessage(const std::string &messageName);

private:
		gmsec::api5::ConfigFile configFile;
		std::string             configFileName;
};

#endif
