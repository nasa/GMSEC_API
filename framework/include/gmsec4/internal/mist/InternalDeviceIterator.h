/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_MIST_INTERNAL_DEVICE_ITERATOR_H
#define GMSEC_API_MIST_INTERNAL_DEVICE_ITERATOR_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
namespace mist
{
	class Device;

	namespace message
	{
		namespace internal
		{
			class InternalDeviceMessage;
		}
	}
	
namespace internal
{
class GMSEC_API InternalDeviceIterator
{
public:
	InternalDeviceIterator(message::internal::InternalDeviceMessage& msg);


    ~InternalDeviceIterator();


    bool CALL_TYPE hasNext();


    const Device& CALL_TYPE next();


	void CALL_TYPE reset();


private:
	// Declared, but not implemented
	InternalDeviceIterator(const InternalDeviceIterator&);
	InternalDeviceIterator& operator=(const InternalDeviceIterator&);


	bool CALL_TYPE makeReady();


	message::internal::InternalDeviceMessage& m_msg;
	bool                                      m_ready;
	const Device*                             m_nextDevice;
};

} // namespace internal
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
