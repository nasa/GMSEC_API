/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#include <bolt/Shared.h>
#include <bolt/Log.h>
#include <bolt/util.h>

#include <gmsec4/util/TimeUtil.h>

using namespace gmsec::api::util;


namespace bolt {


struct ActionInfo
{
	ActionInfo(double interval) : last_s(TimeUtil::getCurrentTime_s()), interval_s(interval)
	{
	}

	bool tryNow() {
		bool flag = false;
		double now_s = TimeUtil::getCurrentTime_s();
		if (now_s - last_s >= interval_s) {
			flag = true;
			last_s = now_s;
		}
		return flag;
	}

private:
	double last_s;
	double interval_s;
};

/**
* The Manager holds the Condition of the Connection.
* If the connection is broken, the Manager takes care of reconnection.
*/

void runManager(counted_ptr<Shared> shared)
{
	const int timeout_ms = 5000;

	Condition &condition(shared->getCondition());
	string tag = shared->tag("runManager");

GMSEC_DEBUG << tag.c_str() << "acquiring mutex";
	AutoMutex hold(condition.getMutex());
	shared->setState(STATE_MANAGED);
	condition.broadcast(SIGNAL_STATE);
GMSEC_DEBUG << tag.c_str() << "broadcast state";

	State state = STATE_UNKNOWN;

	ActionInfo reconnect(5);
	ActionInfo ping(1);

	while ((state = shared->getState()) != STATE_FINISHED)
	{
		int reason = condition.wait(timeout_ms);
GMSEC_DEBUG << tag.c_str() << "condition= " << reason;

		state = shared->getState();
GMSEC_DEBUG << tag.c_str() << "state=" << util::toString(state).c_str();

		if (state == STATE_RECONNECTING)
		{
			if (reconnect.tryNow())
				shared->connect();
		}
		else if (state == STATE_CONNECTED)
		{
			if (ping.tryNow())
				shared->ping();
		}
	}

GMSEC_VERBOSE << tag.c_str() << "runManager finished";
}

} // namespace bolt


