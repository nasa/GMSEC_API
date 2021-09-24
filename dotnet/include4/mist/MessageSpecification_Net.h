/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#ifndef GMSEC_API_MIST_MESSAGESPECIFICATION_NET_H
#define GMSEC_API_MIST_MESSAGESPECIFICATION_NET_H

// C++ API native
#include <gmsec4/mist/MessageSpecification.h>


namespace GMSEC
{
namespace API
{
namespace MIST
{
	// Forward declaration(s)
	ref class FieldSpecification;


/// <summary>
/// This class is a container for holding information on a Message Specification.
/// </summary>
///
/// <seealso cref="FieldSpecification"/>
/// <seealso cref="Specification"/>
///
public ref class MessageSpecification
{
public:
	/// <summary>Destructor</summary>
	virtual ~MessageSpecification();


	/// <summary>
	/// Accessor for the MessageSpecification schema ID.
	/// </summary>
	System::String^ GetSchemaID();


	/// <summary>
	/// Accessor for acquiring the list of FieldSpecification objects associated with the MessageSpecification.
	/// </summary>
	System::Collections::Generic::List<FieldSpecification^>^ GetFieldSpecifications();


protected:
	/// <summary>Finalize, free memory and set objects to null</summary>
	!MessageSpecification();


internal:
	MessageSpecification(System::String^ schemaID, System::Collections::Generic::List<FieldSpecification^>^ fieldSpecs);


private:
	System::String^                                          m_schemaID;
	System::Collections::Generic::List<FieldSpecification^>^ m_fieldSpecs;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
