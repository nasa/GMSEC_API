/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_DEVICE_ITERATOR_NET_H
#define GMSEC_API_DEVICE_ITERATOR_NET_H


// C++ API native
#include <gmsec4/mist/DeviceIterator.h>


namespace GMSEC
{
namespace API
{
namespace MIST
{
// Forward declarations
ref class Device;


/// <summary>
/// The DeviceIterator class can be used to iterate over the Device Object(s) contained within a DeviceMessage.
///
/// <seealso cref="DeviceMessage"/>
/// </summary>

public ref class DeviceIterator
{
public:
	~DeviceIterator();


	/// <summary>Provides indication as to whether there are additional Device objects that can be referenced using Next().</summary>
	///
	/// <seealso cref="next"/>
	/// <seealso cref="reset"/>
	bool HasNext();


	/// <summary>Returns a reference to the next available Device object.</summary>
	///
	/// <seealso cref="hasNext"/>
	/// <seealso cref="reset"/>
	///
	/// <exception cref="GMSEC_Exception">An exception is thrown if the iterator has reached the end (i.e. there are no more Devices.)</exception>
	Device^ Next();


	/// <summary>Resets the iterator to the beginning of the Device list that is maintained by the DeviceMessage.</summary>
	void Reset();


protected:
	/// <summary>Finalizer, free memory and set objects to null</summary>
	!DeviceIterator();


internal:
	/// <summary>Internal constructor</summary>
	DeviceIterator(gmsec::api::mist::DeviceIterator* iter);


	/// <summary>Reset unmanaged implementation to null</summary>
	void ResetUnmanagedImplementation();


private:
	gmsec::api::mist::DeviceIterator* m_impl;
};

} // end namespace MIST
} // end namespace API
} // end namespace GMSEC

#endif
