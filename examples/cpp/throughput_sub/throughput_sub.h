#ifndef GMPUB_THROUGHPUT_H
#define GMPUB_THROUGHPUT_H

#include <gmsec4_defs.h>

#include <gmsec4/util/StdSharedPtr.h>
#include <gmsec4/util/StdThread.h>

namespace gmsec
{
namespace api
{
// Forward Declarations
class Config;
class Connection;
}
}

class status_reporter;

using namespace gmsec::api;
using namespace gmsec::api::util;

class throughput_sub
{
public:
	throughput_sub(Config& config);
	~throughput_sub();
	int getNumMsgsDropped();
	size_t getNumBitsReceived();
	bool run();

private:
	Config&     config;
	Connection* connection;
	StdThread*  reportingThread;
	size_t      bytesReceived;
	int         droppedMsgCount;
	int         iteration;
	mutable     gmsec::api::util::Mutex mutex;
};

void runReporter(StdSharedPtr<status_reporter> reporter);

#endif
