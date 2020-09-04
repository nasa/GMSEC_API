#ifndef STATUS_REPORTER_H
#define STATUS_REPORTER_H

#include <gmsec4_defs.h>

class throughput_pub;

class status_reporter
{
public:
	status_reporter(int dataSize, int monitorRate, throughput_pub* publisher);
	~status_reporter();
	bool run();
private:
	int m_dataSize;
	int m_monitorRateMs;
	int m_lastNumPublished;
	throughput_pub* m_publisher;
};

#endif
