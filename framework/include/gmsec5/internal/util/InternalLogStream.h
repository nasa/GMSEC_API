/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_LOG_STREAM_H
#define GMSEC_API5_INTERNAL_LOG_STREAM_H

#ifdef __APPLE__
#include <gmsec5/util/Mutex.h>
#endif
#include <gmsec5/util/wdllexp.h>

#include <sstream>
#include <string>


namespace gmsec
{
namespace api5
{
namespace internal
{

class GMSEC_API InternalLogStream
{
public:
	InternalLogStream();

	~InternalLogStream();

	const char* getText() const;

	template <typename T>
	InternalLogStream& operator<<(T& t)
	{
#ifdef __APPLE__
		gmsec::api5::util::AutoMutex lock(m_mutex);
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
	gmsec::api5::util::Mutex m_mutex;
#endif
};

}  // end namespace internal
}  // end namespace api5
}  // end namespace gmsec

#endif
