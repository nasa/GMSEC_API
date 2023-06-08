/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ResourceInfoCollector.h
 *
 * @brief The ResourceInfoCollector encapsulates common API functions for
 * gathering system resource information and adding them to a GMSEC message
 */

#ifndef GMSEC_API5_INTERNAL_RESOURCE_INFO_COLLECTOR_H
#define GMSEC_API5_INTERNAL_RESOURCE_INFO_COLLECTOR_H

#include <gmsec5_defs.h>

#include <string>
#include <vector>
#include <deque>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Message;

namespace internal
{

class ResourceInfoCollector
{
public:
	static void reset();

	static void addMainMemoryStats(Message& msg, unsigned int specVersion, size_t movingAverageSamples = 1);

	static void addDiskStats(Message& msg, unsigned int specVersion, size_t movingAverageSamples = 1);

	static void addCPUStats(Message& msg, unsigned int specVersion, size_t movingAverageSamples = 1);

	static void addNetworkStats(Message& msg, size_t samplePeriod, size_t movingAverageSamples = 1);

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

		GMSEC_U16                   num_net_ports;
		std::vector<StatsForOneNet> stats;
	};


	static std::deque<MainMemoryStats> m_mainMemoryStatsQueue;
	static std::deque<DiskStats>       m_diskStatsQueue;
	static std::deque<CPUStats>        m_cpuStatsQueue;
	static std::deque<NetworkStats>    m_networkStatsQueue;
};

}  // end namespace internal
}  // end namespace api5
}  // end namespace gmsec

#endif
