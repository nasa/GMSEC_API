/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <bolt/Test.h>
#include <bolt/Shared.h>
#include <bolt/util.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

#include <gmsec4/internal/Rawbuf.h>


using namespace gmsec::api::util;


namespace bolt {


Test::Test()
	:
	prelocked(false),
	verbosity(0)
{
}


std::string Test::info()
{
	if (!description.length())
	{
		Rawbuf<256> buffer;
		buffer.stream() << "Test[" << this << ',' << util::getname(typeid(*this)) << ']';
		description = buffer.str();
	}

	return description;
}


void Test::describe(const string &s)
{
	description = s;
}


TestConnected::TestConnected(Shared *s)
	:
	shared(s)
{
}


Code TestConnected::apply()
{
	Code code = CODE_FALSE;

	State s = shared->getState();

	if (s == STATE_CONNECTED)
	{
		code = SUCCESS;
	}
	else if (s == STATE_FINISHED)
	{
		code = CODE_ABORT;
	}

	return code;
}


TestAcknowledged::TestAcknowledged(Shared *s, const string &i)
	:
	shared(s),
	id(i)
{
}


Code TestAcknowledged::apply()
{
	return shared->isAcknowledged(id);
}



namespace util {


const int DEBUG_AWAIT = 1;


Result await(Test &test, Condition &cv, i32 timeout_ms)
{
	Result result;

	if (DEBUG_AWAIT)
	{
		GMSEC_DEBUG << "util.await: " << test.info().c_str() << " timeout_ms=" << timeout_ms;
	}

	const int ms_per_s  = 1000;
	const int ns_per_ms = 1000 * ms_per_s;
	const int ns_per_s  = 1000 * ns_per_ms;

	GMSEC_TimeSpec until;
	TimeUtil::getCurrentTime_s(&until);

	if (timeout_ms >= ms_per_s)
	{
		until.seconds += timeout_ms / ms_per_s;
		timeout_ms %= ms_per_s;
	}

	until.nanoseconds += timeout_ms * ns_per_ms;

	if (until.nanoseconds >= ns_per_s)
	{
		int delta_s = until.nanoseconds / ns_per_s;
		until.nanoseconds %= ns_per_s;
		until.seconds += delta_s;
	}

	if (DEBUG_AWAIT)
	{
		GMSEC_DEBUG << "util.await: seconds=" << time_t(until.seconds) << " nanos=" << until.nanoseconds;
	}

	AutoMutex hold(cv.getMutex(), !test.isPrelocked());

	if (DEBUG_AWAIT)
	{
		GMSEC_DEBUG << "util.await: " << test.info().c_str() << " has mutex";
	}

	Code code = test.apply();

	if (DEBUG_AWAIT)
	{
		GMSEC_DEBUG << "util.await: " << test.info().c_str() << " code=" << util::toString(code).c_str();
	}

	while (code == CODE_FALSE)
	{
		int reason = cv.waitUntil(until);

		if (reason == Condition::TIMEOUT)
		{
			code = CODE_TIMEOUT;
		}
		else
		{
			code = test.apply();
		}
	}

	if (code != SUCCESS)
	{
		util::set(result, code);
	}

	if (result.isError())
	{
		GMSEC_VERBOSE << "util.await: " << test.info().c_str() << " result=" << result;
	}

	return result;
}

} // namespace util
} // namespace bolt
