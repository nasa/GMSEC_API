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

class throughput_pub
{
public:
	throughput_pub(Config& config);
	~throughput_pub();
	int getNumPublished();
	bool run();

private:
	Config&     config;
	Connection* connection;
	StdThread*  reportingThread;
	int         iteration;
};

void runReporter(StdSharedPtr<status_reporter> reporter);

#endif
