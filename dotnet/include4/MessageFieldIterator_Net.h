/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MESSAGE_FIELD_ITERATOR_NET_H
#define GMSEC_API_MESSAGE_FIELD_ITERATOR_NET_H


// C++ API native
#include <gmsec4/MessageFieldIterator.h>


namespace GMSEC
{
namespace API
{

// Forward declarations
ref class Field;


/// <summary>
/// The MessageFieldIterator class can be used to iterate over the Fields contained within a Message.
///
/// <seealso cref="Message"/>
/// </summary>

public ref class MessageFieldIterator
{
public:
	enum class Selector
	{
		ALL_FIELDS,
		HEADER_FIELDS,
		NON_HEADER_FIELDS
	};


	~MessageFieldIterator();


	bool HasNext();


	Field^ Next();


	void Reset();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!MessageFieldIterator();


internal:
	/// <summary>Internal constructor</summary>
	MessageFieldIterator(gmsec::api::MessageFieldIterator* iter);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


private:
	gmsec::api::MessageFieldIterator* m_impl;
};

} // end namespace API
} // end namespace GMSEC

#endif
