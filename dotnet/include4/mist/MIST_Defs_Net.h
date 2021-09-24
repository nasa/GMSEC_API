/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MIST_DEFS_NET_H
#define GMSEC_API_MIST_DEFS_NET_H


/// <summary>
/// This file contains public MIST-related enumerations and constants.
/// </summary>


namespace GMSEC
{
namespace API
{
namespace MIST
{


/// <summary>Definitions for the available GMSEC Interface Specification Document versions.</summary>
public ref class GMSEC_ISD
{
public:
	/// <summary>2014 version of the GMSEC Interface Specification Document (ISD).</summary>
	static const unsigned int GMSEC_ISD_2014_00 = 201400;
	
	/// <summary>2016 version of the GMSEC Interface Specification Document (ISD).</summary>
	static const unsigned int GMSEC_ISD_2016_00 = 201600;

	/// <summary>2018 version of the Command and Control Message Specification (C2MS).</summary>
	static const unsigned int GMSEC_ISD_2018_00 = 201800;

	/// <summary>2019 version of the Command and Control Message Specification (C2MS).</summary>
	static const unsigned int GMSEC_ISD_2019_00 = 201900;

	/// <summary>Current version.</summary>
	static const unsigned int GMSEC_ISD_CURRENT = GMSEC_ISD_2019_00;
};


/// <summary>Possible values for the a GMSEC Message's Response to a request.</summary>
public enum class ResponseStatus
{
	ACKNOWLEDGEMENT = 1,        ///< Basic acknowledgement
	WORKING_KEEP_ALIVE = 2,     ///< Still working on operation
	SUCCESSFUL_COMPLETION = 3,  ///< Operation successfully completed
	FAILED_COMPLETION = 4,      ///< Operation failed to complete
	INVALID_REQUEST = 5,        ///< The request for an operation was invalid
	FINAL_MESSAGE = 6           ///< This message is the last in a set of responses
};


} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
