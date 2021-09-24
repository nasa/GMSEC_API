/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API_INTERNAL_LOG_STREAM_H
#define GMSEC_API_INTERNAL_LOG_STREAM_H

#ifdef __APPLE__
#include <gmsec4/util/Mutex.h>
#endif

#include <gmsec4/util/wdllexp.h>

#include <sstream>
#include <string>


namespace gmsec
{
namespace api
{
namespace internal
{

class GMSEC_API InternalLogStream
{
public:
	InternalLogStream();

	~InternalLogStream();

	const char* CALL_TYPE getText() const;

	template <typename T>
	InternalLogStream& CALL_TYPE operator<<(T& t)
	{
#ifdef __APPLE__
		gmsec::api::util::AutoMutex lock(m_mutex);
#endif

		m_oss << t;

		return *this;
	}

private:
	// Defined, but not implemented.
	InternalLogStream(const InternalLogStream&);
	InternalLogStream& operator=(const InternalLogStream&);

	std::ostringstream  m_oss;
	mutable std::string m_data;
#ifdef __APPLE__
	gmsec::api::util::Mutex m_mutex;
#endif
};

}  // end namespace internal
}  // end namespace api
}  // end namespace gmsec

#endif
