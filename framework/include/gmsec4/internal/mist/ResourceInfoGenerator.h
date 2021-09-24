/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file ResourceInfoGenerator.h
 *
 *  @brief The ResourceInfoGenerator encapsulates common API functions for
 *  gathering system resource information and adding them to a GMSEC message
 */

#ifndef GMSEC_MIST_INTERNAL_RESOURCEINFOGENERATOR_H
#define GMSEC_MIST_INTERNAL_RESOURCEINFOGENERATOR_H

#include <gmsec4_defs.h>

#include <string>
#include <vector>
#include <deque>


namespace gmsec
{
namespace api
{
namespace mist
{
	namespace message
	{
		// Forward declaration(s)
		class MistMessage;
	}

namespace internal
{

class ResourceInfoGenerator
{
public:
	static void addMainMemoryStats(message::MistMessage& msg, unsigned int specVersion, size_t movingAverageSamples = 1);

	static void addDiskStats(message::MistMessage& msg, unsigned int specVersion, size_t movingAverageSamples = 1);

	static void addCPUStats(message::MistMessage& msg, unsigned int specVersion, size_t movingAverageSamples = 1);

	static void addNetworkStats(message::MistMessage& msg, size_t samplePeriod, size_t movingAverageSamples = 1);

	static std::string getOSVersion();

private:
	struct CPUTickStruct
	{
		CPUTickStruct()
			: user_mode_ticks(0),
			  nice_mode_ticks(0),
			  system_mode_ticks(0),
			  idle_mode_ticks(0)
		{}

		long unsigned int user_mode_ticks;
		long unsigned int nice_mode_ticks;
		long unsigned int system_mode_ticks;
		long unsigned int idle_mode_ticks;
	};

	struct MainMemoryStats
	{
		MainMemoryStats()
			: memory_percent_utilized(0),
			  total_physical_memory(0),
			  available_physical_memory(0),
			  total_virtual_memory(0),
			  available_virtual_memory(0)
		{}

		GMSEC_F32 memory_percent_utilized;
		GMSEC_I64 total_physical_memory;
		GMSEC_I64 available_physical_memory;
		GMSEC_I64 total_virtual_memory;
		GMSEC_I64 available_virtual_memory;
	};

	struct StatsForOneDisk
	{
		StatsForOneDisk()
			: disk_size(0),
			  disk_util(0)
		{}

		std::string disk_name;
        GMSEC_F32   disk_size;
        GMSEC_F32   disk_util;
	};

	struct DiskStats
	{
		DiskStats()
			: num_of_disks(0)
		{}

		GMSEC_I16                    num_of_disks;
		std::vector<StatsForOneDisk> stats;
	};

	struct CPUStats
	{
		CPUStats()
			: num_cpus(0),
			  cpu_total_util(0)
		{}

		GMSEC_I16              num_cpus;
		GMSEC_F32              cpu_total_util;
		std::vector<GMSEC_F32> cpu_utils;
		std::vector<GMSEC_U32> util_ticks;
		std::vector<GMSEC_U32> total_ticks;
	};

	struct StatsForOneNet
	{
		StatsForOneNet()
			: bytesRX(0),
			  bytesTX(0),
			  errorsRX(0),
			  errorsTX(0),
			  bandwidth(0)
		{}

		std::string if_name;
		std::string ip_addr;
		std::string eui_addr;
		GMSEC_U64   bytesRX;
		GMSEC_U64   bytesTX;
		GMSEC_U32   errorsRX;
		GMSEC_U32   errorsTX;
		GMSEC_U32   bandwidth;
	};

	struct NetworkStats
	{
		NetworkStats()
			: num_net_ports(0)
		{}

		GMSEC_I16                   num_net_ports;
		std::vector<StatsForOneNet> stats;
	};


	static std::deque<MainMemoryStats> m_mainMemoryStatsQueue;
	static std::deque<DiskStats>       m_diskStatsQueue;
	static std::deque<CPUStats>        m_cpuStatsQueue;
	static std::deque<NetworkStats>    m_networkStatsQueue;
};

}  // end namespace internal
}  // end namespace mist
}  // end namespace api
}  // end namespace gmsec

#endif
