#ifndef STATUS_REPORTER_H
#define STATUS_REPORTER_H

#include <gmsec4_defs.h>

class throughput_sub;

class status_reporter
{
public:
	status_reporter(int monitorRate, throughput_sub* publisher);
	~status_reporter();
	bool run();
private:
	int m_monitorRateMs;
	throughput_sub* m_receiver;
};

#endif
