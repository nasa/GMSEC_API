/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file DeviceIterator.h
 *
 * @brief Defines a class that supports the iteration over the Device objects stored in a DeviceMessage.
 */

#ifndef GMSEC_API_DEVICE_ITERATOR_H
#define GMSEC_API_DEVICE_ITERATOR_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace mist
{
	class Device;

	namespace internal
	{
		class InternalDeviceIterator;
	}

namespace message
{
	namespace internal
	{
		class InternalDeviceMessage;
	}
}



/** @class DeviceIterator
 * 
 * @brief The DeviceIterator supports iterating over the Device objects of a DeviceMessage.
 *
 * @note DeviceIterator is not thread safe.
 *
 * @sa DeviceMessage::getDeviceIterator()
*/
class GMSEC_API DeviceIterator
{
public:
	/*
	 * @fn ~DeviceIterator()
	 *
	 * @brief Destructor.
	 */
    ~DeviceIterator();


	/**
	 * @fn bool hasNext() const
	 *
	 * @brief Provides information as to whether there are additional Device objects that can be referenced using next().
	 *
	 * @return True if additional fields are available, false otherwise.
	 */
    bool CALL_TYPE hasNext() const;


	/**
	 * @fn const Device& next() const
	 *
	 * @brief Returns a reference to the next available Device object.
	 *
	 * @return A reference to a Device object.
	 *
	 * @throw Exception is thrown if the iterator has reached the end (i.e. there are no more Devices).
	 */
    const Device& CALL_TYPE next() const;


	/**
	 * @fn void reset()
	 *
	 * @desc Resets the interator to the beginning of the Device list that is maintained by the DeviceMessage.
	 */
	void CALL_TYPE reset();


private:
	friend class gmsec::api::mist::message::internal::InternalDeviceMessage;

	/**
	 * @fn DeviceIterator(const DeviceMessage& msg)
	 *
	 * @brief Default constructor.
	 *
	 * @param msg - the DeviceMessage object to use when iterating over the Device objects.
	 */
	DeviceIterator(mist::message::internal::InternalDeviceMessage& msg);


	// Declared, but not implemented
	DeviceIterator(const DeviceIterator&);

	//TODO - Figure out why Visual Studio thinks this method must be implicitly defined.
	//       Might be related to the (false) C4355 warning.
	//DeviceIterator& operator=(const DeviceIterator&);


	gmsec::api::mist::internal::InternalDeviceIterator* m_iter;
};

} // namespace mist
} // namespace api
} // namespace gmsec

#endif
