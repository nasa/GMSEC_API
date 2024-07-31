/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

/*
 * config_file.h
 */

#ifndef GMSEC_API5_EXAMPLE_CONFIG_FILE_H
#define GMSEC_API5_EXAMPLE_CONFIG_FILE_H


/**
 * @fn config_file()
 *
 * @brief fake Constructor, provide the file name of interest.
 */
void configFileDemo(const char *configFilename);


/**
 * Open demo, also the test file.  If a GMSECException results because of this operation
 * the method returns false.
 *
 * @return 1 if no GMSECException is caught, otherwise < 1 for failure
 */
int open();


/**
 * Close the config demo
 */
void close();


/**
 * Print the named subscription entry contents or print that it does not exist, or there is an error.
 * @param subscriptionName a String
 */
void showSubscription(const char *subscriptionName);


/**
 * Print the named config contents or print that it does not exist, or there is an error.
 * @param configName a String
 */
void showConfig(const char *configName);


/**
 * Print the named message contents or print that it does not exist, or there is an error.
 * @param configName a String
 */
void showMessage(const char *messageName);


#endif
