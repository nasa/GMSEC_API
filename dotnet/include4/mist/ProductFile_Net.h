/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_PRODUCT_FILE_NET_H
#define GMSEC_API_PRODUCT_FILE_NET_H


// C++ API native
#include <gmsec4/mist/ProductFile.h>


namespace GMSEC
{
namespace API
{
namespace MIST
{


/// <summary>
/// This class is a lightweight container for holding information on a Product File,
/// and used to generate GMSEC Product File messages by the ConnectionManager class. 
/// </summary>


public ref class ProductFile
{
public:
	/// <summary>Constructor for initializing the ProductFile object with a URI to describe a commonly accessible resource.</summary>
	///
	/// <param name="name">The name of the device parameter</param>
	/// <param name="description">The description of the file</param>
	/// <param name="version">The version of the file</param>
	/// <param name="format">The description of the file's format</param>
	/// <param name="uri">The URI for the file</param>
	ProductFile(System::String^ name, System::String^ description, System::String^ version,
		        System::String^ format, System::String^ uri);


	/// <summary>Constructor for initializing the ProductFile object with the binary contents of a file.</summary>
	///
	/// <param name="name">The name of the device parameter</param>
	/// <param name="description">The description of the file</param>
	/// <param name="version">The version of the file</param>
	/// <param name="format">The description of the file's format</param>
	/// <param name="data">The binary contents of the file</param>
	ProductFile(System::String^ name, System::String^ description, System::String^ version,
		        System::String^ format, array<System::Byte>^ data);


	/// <summary>Copy-constructor that can be used to make a deep-copy of another product file object.</summary>
	///
	/// <param name="other">The product file object to copy</param>
	ProductFile(ProductFile^ other);


	/// <summary>Destructor</summary>
	~ProductFile();


	/// <summary>Returns the name of the product file.</summary>
	System::String^ GetName();


	/// <summary>Returns the description of the product file.</summary>
	System::String^ GetDescription();


	/// <summary>Returns the version of the product file.</summary>
	System::String^ GetVersion();


	/// <summary>Returns the format of the product file.</summary>
	System::String^ GetFormat();


	/// <summary>Returns whether a URI has been set for the product file.</summary>
	bool UriAvailable();


	/// <summary>Returns the URI, if any, that has been set for the product file.</summary>
	System::String^ GetURI();


	/// <summary>Returns whether the contents for the product file are available.</summary>
	bool ContentsAvailable();


	/// <summary>Returns the contents, if available, for the product file.</summary>
	array<System::Byte>^ GetContents();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!ProductFile();


internal:
	/// <summary>This constructor creates the association of this object with an existing unmanaged object</summary>
	///
	/// <param name="param">unmanaged implementation to initialize with</param>
	ProductFile(gmsec::api::mist::ProductFile* productFile, bool owned);


	/// <summary>Get unmanaged implementation version</summary>
	///
	/// <returns>Unmanaged implementation</returns>
	gmsec::api::mist::ProductFile* GetUnmanagedImplementation();


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


	/// <summary>Delete old impl if present, and set unmanaged implementation to new value</summary>
	void SetUnmanagedImplementation(gmsec::api::mist::ProductFile* productFile, bool owned);


private:
	gmsec::api::mist::ProductFile* m_impl;
	bool                           m_owned;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
