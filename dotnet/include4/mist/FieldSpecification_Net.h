/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



#ifndef GMSEC_API_MIST_FIELDSPECIFICATION_NET_H
#define GMSEC_API_MIST_FIELDSPECIFICATION_NET_H

// C++ API native
#include <gmsec4/mist/FieldSpecification.h>


namespace GMSEC
{
namespace API
{
namespace MIST
{


/// <summary>
/// This class is a container for holding information on a Field Specification.
/// </summary>
///
/// <seealso cref="MessageSpecification"/>
/// <seealso cref="Specification"/>
///
public ref class FieldSpecification
{
public:
	/// <summary>Destructor</summary>
	~FieldSpecification();


	/// <summary>
	/// Accessor for the FieldSpecification name.
	/// </summary>
	System::String^ GetName();


	/// <summary>
	/// Accessor for the FieldSpecification type (e.g. STRING, I16, etc.).
	/// </summary>
	System::String^ GetFieldType();


	/// <summary>
	/// Accessor for the FieldSpecification mode (e.g. REQUIRED, OPTIONAL, etc.).
	/// </summary>
	System::String^ GetMode();


	/// <summary>
	/// Accessor for the FieldSpecification classification.
	/// </summary>
	System::String^ GetClassification();


	/// <summary>
	/// Accessor for the FieldSpecification default value, if any.
	/// </summary>
	System::String^ GetValue();


	/// <summary>
	/// Accessor for the FieldSpecification description.
	/// </summary>
	System::String^ GetDescription();


protected:
	/// <summary>Finalize, free memory and set objects to null</summary>
	!FieldSpecification();


internal:
	/// <summary>
	/// Constructor
	/// </summary>
	FieldSpecification(System::String^ name, System::String^ type, System::String^ mode, System::String^ clazz, System::String^ value, System::String^ desc);


private:
	System::String^ m_name;
	System::String^ m_type;
	System::String^ m_mode;
	System::String^ m_class;
	System::String^ m_value;
	System::String^ m_desc;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
