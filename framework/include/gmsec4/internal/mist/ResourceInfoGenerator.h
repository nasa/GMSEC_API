#ifndef GMSEC_INTERNAL_RESOURCEINFOGENERATOR_H
#define GMSEC_INTERNAL_RESOURCEINFOGENERATOR_H

#include <gmsec4/Message.h>

#include <gmsec4_defs.h>

#include <string>
#include <vector>
#include <deque>

using namespace std;



namespace gmsec
{
namespace api
{
	
namespace mist
{

namespace internal
{

class GMSEC_API ResourceInfoGenerator
{
public:
	static void addMainMemoryStats(Message &msg, unsigned int version, size_t movingAverageSamples = 1);
	static void addDiskStats(Message &msg, unsigned int version, size_t movingAverageSamples = 1);
	static std::string getOSVersion();
	static void addCPUStats(Message &msg, unsigned int version, size_t movingAverageSamples = 1);
	static void addNetworkStats(Message &msg, unsigned int version, size_t movingAverageSamples = 1);

private:
	static void pipeStreamToInputStream(std::istringstream& iss, FILE* pStream);
#ifdef __hpux
	static const char* normalizeMacAddressStr(const char* macAddrStr);
#endif

	struct CPUTickStruct
	{
		long unsigned int user_mode_ticks;
		long unsigned int nice_mode_ticks;
		long unsigned int system_mode_ticks;
		long unsigned int idle_mode_ticks;
	};

	struct MainMemoryStats
	{
		GMSEC_F32 memory_percent_utilized;
		GMSEC_I64 total_physical_memory;
		GMSEC_I64 available_physical_memory;
		GMSEC_I64 total_virtual_memory;
		GMSEC_I64 available_virtual_memory;
	};

	struct StatsForOneDisk
	{
		std::string disk_name;
	        GMSEC_F32   disk_size;
	        GMSEC_F32   disk_util;
	};

	struct DiskStats
	{
		GMSEC_I16                    num_of_disks;
		std::vector<StatsForOneDisk> stats;
	};

	struct CPUStats
	{
		GMSEC_I16                    num_cpus;
		GMSEC_F32                    cpu_total_util;
		std::vector<GMSEC_F32>       cpu_utils;
		std::vector<GMSEC_U32>       util_ticks;
		std::vector<GMSEC_U32>       total_ticks;
	};

	struct StatsForOneNet
	{
		std::string net_port_name;
		std::string net_port_ip_addr;
		std::string net_port_eu_addr;
		std::string net_port_desc;
	};

	struct NetworkStats
	{
		GMSEC_I16                    num_net_ports;
		std::vector<StatsForOneNet>  stats;
	};


	static std::deque<MainMemoryStats> m_mainMemoryStatsQueue;
	static std::deque<DiskStats>       m_diskStatsQueue;
	static std::deque<CPUStats>        m_cpuStatsQueue;
	static std::deque<NetworkStats>    m_networkStatsQueue;

};

}
}
}
}
#endif
