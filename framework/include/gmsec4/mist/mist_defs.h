/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file mist_defs.h
 * 
 *  @brief This file contains public MIST-related enumerations and constants.
 */

#ifndef GMSEC_API_MIST_DEFS_H
#define GMSEC_API_MIST_DEFS_H

namespace gmsec
{
namespace api
{
namespace mist
{

	/**
	 * @desc 2014 version of the GMSEC Interface Specification Document (ISD)
	 */
	static const unsigned int GMSEC_ISD_2014_00 = 201400;

	/**
	 * @desc 2016 version of the GMSEC Interface Specification Document (ISD)
	 */
	static const unsigned int GMSEC_ISD_2016_00 = 201600;

	/**
	 * @desc Current version of the GMSEC Interface Specification Document (ISD)
	*/
	static const unsigned int GMSEC_ISD_CURRENT = GMSEC_ISD_2016_00;

	/**
     * @enum ResponseStatus
     * @brief Possible values for the a GMSEC Message's Response to a request.
     */
	struct ResponseStatus
	{
		enum Response
		{
			ACKNOWLEDGEMENT = 1,		///< Basic acknowledgement
			WORKING_KEEP_ALIVE = 2,		///< Still working on operation
			SUCCESSFUL_COMPLETION = 3,	///< Operation successfully completed
			FAILED_COMPLETION = 4,		///< Operation failed to complete
			INVALID_REQUEST = 5,		///< The request for an operation was invalid
			FINAL_MESSAGE = 6			///< This message is the last in a set of responses
		};
	};

}
}
}

#endif
