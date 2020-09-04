#include "../example.h"
#include "status_reporter.h"
#include "throughput_sub.h"

using namespace gmsec::api;
using namespace gmsec::api::util;


status_reporter::status_reporter(int monitorRate, throughput_sub* receiver)
	: m_monitorRateMs(monitorRate),
	m_receiver(receiver)
{
}

status_reporter::~status_reporter()
{
}

bool status_reporter::run()
{
	bool done = false;
	while(!done)
	{
		TimeUtil::millisleep(m_monitorRateMs);

		size_t numBitsReceived = m_receiver->getNumBitsReceived();

		GMSEC_INFO << "\nReceived " << numBitsReceived / 1024 << " kb per " << m_monitorRateMs << " ms";
	}

	return true;
}
