/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file mist_defs.h
 *  @brief Contains common definitions and enumerations used by MIST.
 */


#ifndef GMSEC_API_C_MIST_DEFS_H
#define GMSEC_API_C_MIST_DEFS_H


/**
 * @desc Definition for the 2014 version of the GMSEC Interface Specification Document (ISD)
 */
#define C_GMSEC_ISD_2014_00 201400

/**
 * @desc Definition for the 2016 version of the GMSEC Interface Specification Document (ISD)
 */
#define C_GMSEC_ISD_2016_00 201600

/**
 * @desc Definition for the 2018 version of the GMSEC Interface Specification Document (ISD)
 */
#define C_GMSEC_ISD_2018_00 201800

/**
 * @desc Definition for the 2019 version of the Command and Control Message Specificaiton (C2MS)
 */
#define C_GMSEC_ISD_2019_00 201900

/**
 * @desc Definition for the default version
 */
#define C_GMSEC_ISD_CURRENT C_GMSEC_ISD_2019_00


/**
 * @enum GMSEC_SchemaLevel
 *
 * @sa specificationGetSchemaLevel
 */
typedef enum
{
	GMSEC_SCHEMA_LEVEL_0,    /**< C2MS */
	GMSEC_SCHEMA_LEVEL_1,    /**< C2MS Extensions, or optionally user-defined */
	GMSEC_SCHEMA_LEVEL_2,    /**< NASA/GMSEC Addendum, or optionally user-defined */
	GMSEC_SCHEMA_LEVEL_3,    /**< User-defined */
	GMSEC_SCHEMA_LEVEL_4,    /**< User-defined */
	GMSEC_SCHEMA_LEVEL_5,    /**< User-defined */
	GMSEC_SCHEMA_LEVEL_6     /**< User-defined */
} GMSEC_SchemaLevel;


/**
 * @enum GMSEC_ResponseStatus
 *
 * @sa productFileMessageCreate
 * @sa productFileMessageGetResponseStatus
 */
typedef enum
{
	GMSEC_ACKNOWLEDGEMENT = 1,         /**< Acknowledgement */
	GMSEC_WORKING_KEEP_ALIVE = 2,      /**< Working keep alive */
	GMSEC_SUCCESSFUL_COMPLETION = 3,   /**< Successful completion */
	GMSEC_FAILED_COMPLETION = 4,       /**< Failed completion */
	GMSEC_INVALID_REQUEST = 5,         /**< Invalid request */
	GMSEC_FINAL_MESSAGE = 6            /**< Final message */
} GMSEC_ResponseStatus;


#endif
