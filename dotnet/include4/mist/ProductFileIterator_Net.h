/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_PRODUCT_FILE_ITERATOR_NET_H
#define GMSEC_API_PRODUCT_FILE_ITERATOR_NET_H


// C++ API native
#include <gmsec4/mist/ProductFileIterator.h>


namespace GMSEC
{
namespace API
{
namespace MIST
{
// Forward declarations
ref class ProductFile;


/// <summary>
/// The ProductFileIterator class can be used to iterate over the ProductFile Object(s) contained within a ProductFileMessage.
///
/// <seealso cref="ProductFileMessage"/>
/// </summary>

public ref class ProductFileIterator
{
public:
	~ProductFileIterator();


	/// <summary>Provides indication as to whether there are additional ProductFile objects that can be referenced using Next().</summary>
	///
	/// <seealso cref="next"/>
	/// <seealso cref="reset"/>
	bool HasNext();


	/// <summary>Returns a reference to the next available ProductFile object.</summary>
	///
	/// <seealso cref="hasNext"/>
	/// <seealso cref="reset"/>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if the iterator has reached the end (i.e. there are no more ProductFiles.)</exception>
	ProductFile^ Next();


	/// <summary>Resets the iterator to the beginning of the ProductFile list that is maintained by the ProductFileMessage.</summary>
	void Reset();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ProductFileIterator();


internal:
	/// <summary>Internal constructor</summary>
	ProductFileIterator(gmsec::api::mist::ProductFileIterator* iter);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


private:
	gmsec::api::mist::ProductFileIterator* m_impl;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
