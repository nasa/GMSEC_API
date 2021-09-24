/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MIST_SCHEMA_ID_ITERATOR_NET_H
#define GMSEC_API_MIST_SCHEMA_ID_ITERATOR_NET_H


// C++ API native
#include <gmsec4/mist/SchemaIDIterator.h>


namespace GMSEC
{
namespace API
{
namespace MIST
{


/// <summary>
/// This class supports the iteration of Specification's loaded schema templates.
/// </summary>
///
/// <seealso cref="Specification::getSchemaIDIterator"/>
///
public ref class SchemaIDIterator
{
public:
	~SchemaIDIterator();


	/// <summary>
	/// Provides information as to whether there are additional Schema IDs that can be
	/// referenced using Next().
	/// </summary>
	///
	/// <seealso cref="Next"/>
	/// <seealso cref="Reset"/>
    bool HasNext();


	/// <summary>Returns a reference to the next available Schema ID.</summary>
	///
	/// <exception cref="GMSEC_Exception>
	/// An exception is thrown if the iterator has reached the end (i.e. there are no more IDs).
	/// </exception>
    System::String^ Next();


	/// <summary>
	/// Resets the interator to the beginning of the Schema list that is maintained by Specification.
	/// </summary>
	void Reset();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!SchemaIDIterator();


internal:
	/// <summary>Internal constructor</summary>
	SchemaIDIterator(gmsec::api::mist::SchemaIDIterator* iter);


	/// <summary>Reset unmanged implementation to null</summary>
	void ResetUnmanagedImplementation();

private:
	gmsec::api::mist::SchemaIDIterator* m_impl;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
