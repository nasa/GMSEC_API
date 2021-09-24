/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file ResourceInfoGenerator.cpp
 *
 *  @brief The ResourceInfoGenerator encapsulates common API functions for 
 *  gathering system resource information and adding them to a GMSEC message
 */

#include <gmsec4/internal/mist/ResourceInfoGenerator.h>

#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/mist/message/MistMessage.h>


#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>

#include <iomanip>
#include <sstream>
#include <cmath>
#include <cstdlib>
#include <cerrno>

#ifdef WIN32
#include <windows.h>
#include "pdh.h"
#include "pdhmsg.h"
#include "TCHAR.h"
#include <iphlpapi.h>

#include <locale>
#include <codecvt>

#elif defined(__linux__)
#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <sys/socket.h>
#include <sys/vfs.h>
#include <sys/statvfs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <string.h>

#elif defined(__APPLE__)
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/utsname.h>
#include <sys/sysctl.h>
#endif

#define CHAR_BUF_LEN 256
#define CPU_WINDOWS_TIMEOUT 17

using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::message;
using namespace gmsec::api::mist::internal;

using namespace std;


std::deque<ResourceInfoGenerator::MainMemoryStats> ResourceInfoGenerator::m_mainMemoryStatsQueue;
std::deque<ResourceInfoGenerator::DiskStats>       ResourceInfoGenerator::m_diskStatsQueue;
std::deque<ResourceInfoGenerator::CPUStats>        ResourceInfoGenerator::m_cpuStatsQueue;
std::deque<ResourceInfoGenerator::NetworkStats>    ResourceInfoGenerator::m_networkStatsQueue;


#ifdef WIN32
static bool checkStatus(const char* operation, PDH_STATUS status)
{
	if (status == ERROR_SUCCESS)
		return true;

	const char* error = NULL;

	switch (status)
	{
	case PDH_CSTATUS_BAD_COUNTERNAME:
		error = "Bad Counter Name";
		break;
	case PDH_CSTATUS_NO_COUNTER:
		error = "No Counter";
		break;
	case PDH_CSTATUS_NO_COUNTERNAME:
		error = "No Counter Name";
		break;
	case PDH_CSTATUS_NO_MACHINE:
		error = "No machine";
		break;
	case PDH_CSTATUS_NO_OBJECT:
		error = "No object";
		break;
	case PDH_FUNCTION_NOT_FOUND:
		error = "Not found";
		break;
	case PDH_INVALID_ARGUMENT:
		error = "Invalid argument";
		break;
	case PDH_INVALID_HANDLE:
		error = "Invalid handle";
		break;
	case PDH_MEMORY_ALLOCATION_FAILURE:
		error = "Memory allocation failure";
		break;
	case PDH_NO_DATA:
		error = "No data available";
		break;
	case PDH_INVALID_DATA:
		error = "Invalid data";
		break;
	case PDH_CSTATUS_ITEM_NOT_VALIDATED:
		error = "Item not validated";
		break;
	default:
		error = "Unknown error";
		break;
	}

	std::ostringstream oss;
	oss << operation << ": " << error;

	GMSEC_WARNING << oss.str().c_str();

	return false;
}


static std::string wideStringToString(const std::wstring& wstr)
{
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes(wstr);
}

#endif


#if defined(__linux__)

template <typename T>
static T getNetworkValue(const std::string& interface, const std::string& metric, T defaultValue)
{
	T data = defaultValue;

	std::ostringstream cmd;

	cmd << "/bin/cat /sys/class/net/" << interface << "/" << metric << " 2> /dev/null";

	FILE* output = popen(cmd.str().c_str(), "r");

	if (output)
	{
		char buffer[64] = {0};
		bool error = false;

		if (fgets(buffer, sizeof(buffer) - 1, output))
		{
			try {
				data = StringUtil::getValue<T>(buffer);
			}
			catch (...) {
				error = true;
			}
		}

		pclose(output);

		if (error)
		{
			std::ostringstream oss;
			oss << "Failed to process data for " << metric << " for " << interface;
			throw Exception(MIST_ERROR, RESOURCE_INFO_NET_ERROR, oss.str().c_str());
		}
	}
	else
	{
		// Not all interfaces (typically virtual ones) will have the 'metric' we seek.
		// If metric is not available, then just return the default value.
	}

	return data;
}

#endif


#if defined(__APPLE__)

static void pipeStreamToInputStream(std::istringstream& iss, FILE* pStream)
{
	std::ostringstream oss;

	char   buffer[CHAR_BUF_LEN];
	bool   done = false;
	size_t tmp_num_chars = 0;

	while (!done)
	{
		if ((tmp_num_chars = fread(buffer, 1, CHAR_BUF_LEN-1, pStream)) > 0)
		{
			buffer[tmp_num_chars] = '\0';  // terminate string
			oss << buffer;
		}
		else
		{
			done = true;
		}
	}

	iss.str(oss.str());
}


static void getNetworkInfo(const std::string& interface, std::string& ip_addr, std::string& eui_addr)
{
	int mib[6];
	mib[0] = CTL_NET;
	mib[1] = AF_ROUTE;
	mib[2] = 0;
	mib[3] = AF_LINK;
	mib[4] = NET_RT_IFLIST;

	if ((mib[5] = if_nametoindex(interface.c_str())) == 0)
	{
		std::ostringstream oss;
		oss << "Failed to acquire name index for interface: " << interface;
		throw Exception(MIST_ERROR, RESOURCE_INFO_NET_ERROR, oss.str().c_str());
	}

	// Acquire the length of the MIB
	size_t mib_len;

	if (sysctl(mib, sizeof(mib)/sizeof(int), NULL, &mib_len, NULL, 0) < 0)
	{
		std::ostringstream oss;
		oss << "Failed to acquire the MIB length for interface: " << interface;
		throw Exception(MIST_ERROR, RESOURCE_INFO_NET_ERROR, oss.str().c_str());
	}

	char tmp[CHAR_BUF_LEN] = {0};

	if (mib_len > 0)
	{
		std::vector<char> buf(mib_len);

		// Acquire the data from the MIB
		if (sysctl(mib, sizeof(mib)/sizeof(int), buf.data(), &mib_len, NULL, 0) < 0)
		{
			std::ostringstream oss;
			oss << "Failed to acquire the MIB data for interface: " << interface;
			throw Exception(MIST_ERROR, RESOURCE_INFO_NET_ERROR, oss.str().c_str());
		}

		struct if_msghdr*   msg_hdr   = reinterpret_cast<struct if_msghdr*>(buf.data());
		struct sockaddr_dl* sock_addr = reinterpret_cast<struct sockaddr_dl*>(msg_hdr + 1);
		unsigned char*      addr      = reinterpret_cast<unsigned char*>(LLADDR(sock_addr));

		// Get the MAC (eui) address
		StringUtil::stringFormat(tmp, sizeof(tmp) - 1, "%02x:%02x:%02x:%02x:%02x:%02x",
		                         addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
		eui_addr = tmp;
	}
	else
	{
		GMSEC_WARNING << "Failed to obtain the EUI-ADDR for interfce: " << interface.c_str();
		eui_addr = "[unknown]";
	}

	// Get the IP address
	StringUtil::stringFormat(tmp, sizeof(tmp) - 1, "/sbin/ifconfig %s | /usr/bin/grep inet | /usr/bin/grep -v inet6 | /usr/bin/cut -d' ' -f2", interface.c_str());

	FILE* fp = popen(tmp, "r");
	std::istringstream iss;
	pipeStreamToInputStream(iss, fp);
	pclose(fp);

	if (iss.getline(tmp, sizeof(tmp) - 1, '\n'))
	{
		ip_addr = tmp;
	}
	else
	{
		GMSEC_WARNING << "Failed to obtain the IP-ADDR for interfce: " << interface.c_str();
		ip_addr = "[unknown]";
	}
}

#endif


//TODO:
//PROC.ZOMBIES, U32
//PROC.TOTAL, U32


void ResourceInfoGenerator::addMainMemoryStats(MistMessage& msg, unsigned int specVersion, size_t movingAverageSamples)
{
	MainMemoryStats stats_this_iteration;

	stats_this_iteration.memory_percent_utilized   = 0;
	stats_this_iteration.total_physical_memory     = 0;
	stats_this_iteration.available_physical_memory = 0;
	stats_this_iteration.total_virtual_memory      = 0;
	stats_this_iteration.available_virtual_memory  = 0;

#ifdef WIN32

	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	if (GlobalMemoryStatusEx(&statex) != 0)
	{
		stats_this_iteration.memory_percent_utilized   = static_cast<GMSEC_F32>(statex.dwMemoryLoad);
		stats_this_iteration.total_physical_memory     = statex.ullTotalPhys;
		stats_this_iteration.available_physical_memory = statex.ullAvailPhys;
		stats_this_iteration.total_virtual_memory      = statex.ullTotalVirtual;
		stats_this_iteration.available_virtual_memory  = statex.ullAvailVirtual;
	}
	else
	{
		GMSEC_WARNING << "GlobalMemoryStatusEx() failed.";
		throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR, "Failed to query virtual memory stats");
	}

#elif defined(__linux__)

	struct sysinfo sys_info;

	if (sysinfo(&sys_info) == 0) // success
	{
		GMSEC_I64 total_physical_memory     = static_cast<GMSEC_I64>(sys_info.totalram) * static_cast<GMSEC_I64>(sys_info.mem_unit);
		GMSEC_I64 available_physical_memory = static_cast<GMSEC_I64>(sys_info.freeram) * static_cast<GMSEC_I64>(sys_info.mem_unit);
		GMSEC_I64 total_virtual_memory      = static_cast<GMSEC_I64>(sys_info.totalswap) * static_cast<GMSEC_I64>(sys_info.mem_unit);
		GMSEC_I64 available_virtual_memory  = static_cast<GMSEC_I64>(sys_info.freeswap) * static_cast<GMSEC_I64>(sys_info.mem_unit);
		GMSEC_F32 memory_percent_utilized   = static_cast<GMSEC_F32>(sys_info.totalram - sys_info.freeram) / static_cast<GMSEC_F32>(sys_info.totalram);

		if (!std::isfinite(memory_percent_utilized))
		{
			memory_percent_utilized = 1; // assume all memory consumed
		}

		stats_this_iteration.memory_percent_utilized   = memory_percent_utilized * 100.0;
		stats_this_iteration.total_physical_memory     = total_physical_memory;
		stats_this_iteration.available_physical_memory = available_physical_memory;
		stats_this_iteration.total_virtual_memory      = total_virtual_memory;
		stats_this_iteration.available_virtual_memory  = available_virtual_memory;
	}
	else // failure
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR, "Failed to query main memory stats");
	}

#elif defined(__APPLE__)

	bool haveStats = false;
	int  tries     = 40;

	while (!haveStats && tries-- > 0)
	{
		system("/usr/bin/top -l 1 > /tmp/tmpout.top");

		TimeUtil::millisleep(100);

		struct stat top_stats;
		stat("/tmp/tmpout.top", &top_stats);

		haveStats = (top_stats.st_size > 0);
	}

	if (!haveStats)
	{
		GMSEC_ERROR << "Failed to acquire MEMORY stats";
		return;
	}


	char              buffer[CHAR_BUF_LEN];
	bool              phys_done             = false;
	bool              virt_done             = false;
	int               num_args              = 0;
	unsigned long int phys_mem_used         = 0;
	unsigned long int wired_mem_used        = 0;
	unsigned long int available_phys_mem    = 0;
	unsigned long int virtual_mem           = 0;
	char              units1, units2, units3;

	ifstream          ifs("/tmp/tmpout.top", ios::in);

	while (ifs.getline(buffer, sizeof(buffer), '\n') && (!phys_done || !virt_done))
	{
		if (StringUtil::stringCompareCount(buffer, "PhysMem:", 8) == 0)
		{
			num_args = sscanf(buffer, "PhysMem: %lu%c used (%lu%c wired), %lu%c unused.",
			                  &phys_mem_used, &units1,
			                  &wired_mem_used, &units2,
			                  &available_phys_mem, &units3);

			if (num_args == 6)
			{
				if (units1 == 'G')
				{
					phys_mem_used *= 1000;
				}
				if (units3 == 'G')
				{
					available_phys_mem *= 1000;
				}
				phys_done = true;
			}
		}
		else if (StringUtil::stringCompareCount(buffer, "VM:", 3) == 0)
		{
			num_args = sscanf(buffer, "VM: %lu%c vsize", &virtual_mem, &units1);

			if (num_args == 2)
			{
				if (units1 == 'G')
				{
					virtual_mem *= 1000;
				}
				virt_done = true;
			}
		}
	}

	if (!phys_done || !virt_done)
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR, "Failed to query memory stats");
	}
	else
	{
		unlink("/tmp/tmpout.top");
	}

	GMSEC_I64 total_virtual_memory      = static_cast<GMSEC_I64>(virtual_mem) * 1073741824; // virtual_mem is in GBytes
	GMSEC_I64 total_physical_memory     = (static_cast<GMSEC_I64>(phys_mem_used) + static_cast<GMSEC_I64>(available_phys_mem)) * 1048576; // phys_mem is in MBytes
	GMSEC_I64 available_physical_memory = static_cast<GMSEC_I64>(available_phys_mem) * 1048576; // phys mem is in MBytes
	GMSEC_F32 memory_percent_utilized   = static_cast<GMSEC_F32>(phys_mem_used) / static_cast<GMSEC_F32>((phys_mem_used + available_phys_mem));

	if (!std::isfinite(memory_percent_utilized))
	{
		memory_percent_utilized = 1; // assume all memory consumed
	}

	stats_this_iteration.memory_percent_utilized   = memory_percent_utilized * 100.0;
	stats_this_iteration.total_physical_memory     = total_physical_memory;
	stats_this_iteration.available_physical_memory = available_physical_memory;
	stats_this_iteration.total_virtual_memory      = total_virtual_memory;

#else
	throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR, "Failed to query main memory stats:  unknown operating system");
#endif

	//
	// This section takes a queue of memory stats and performs
	// a moving average, based on past calls to this function where
	// the data was recorded in a queue of fixed size (where
	// one old sample is disposed of and a new sample is added, 
	// per call to this function).
	//
	while (m_mainMemoryStatsQueue.size() >= movingAverageSamples)
	{
		m_mainMemoryStatsQueue.pop_front();
	}

	m_mainMemoryStatsQueue.push_back(stats_this_iteration);

	GMSEC_F32 average_memory_percent_utilized   = 0;
	GMSEC_I64 average_total_physical_memory     = 0;
	GMSEC_I64 average_available_physical_memory = 0;
	GMSEC_I64 average_total_virtual_memory      = 0;
	GMSEC_I64 average_available_virtual_memory  = 0;

	for (std::deque<MainMemoryStats>::iterator it = m_mainMemoryStatsQueue.begin(); it != m_mainMemoryStatsQueue.end(); ++it)
	{
		average_memory_percent_utilized   += it->memory_percent_utilized;
		average_total_physical_memory     += it->total_physical_memory;
		average_available_physical_memory += it->available_physical_memory;
		average_total_virtual_memory      += it->total_virtual_memory;
		average_available_virtual_memory  += it->available_virtual_memory;
	}

	average_memory_percent_utilized   /= static_cast<GMSEC_F32>(m_mainMemoryStatsQueue.size());
	average_total_physical_memory     /= m_mainMemoryStatsQueue.size(); // The total physical memory shouldn't change (the average should be the same as a one-time sample), but keep it consistent with the other logic
	average_available_physical_memory /= m_mainMemoryStatsQueue.size();
	average_total_virtual_memory      /= m_mainMemoryStatsQueue.size(); // This total virtual memory shouldn't change (the average should be the same as a one-time sample), but keep it consistent with the other logic
	average_available_virtual_memory  /= m_mainMemoryStatsQueue.size();

	if (specVersion >= GMSEC_ISD_2016_00)
	{
		msg.addField("MEM.UTIL", average_memory_percent_utilized);
		msg.addField("MEM.PHYSICAL.TOTAL", static_cast<GMSEC_U64>(average_total_physical_memory));
		msg.addField("MEM.PHYSICAL.AVAIL", static_cast<GMSEC_U64>(average_available_physical_memory));
		msg.addField("MEM.VIRTUAL.TOTAL", static_cast<GMSEC_U64>(average_total_virtual_memory));
		msg.addField("MEM.VIRTUAL.AVAIL", static_cast<GMSEC_U64>(average_available_virtual_memory));
	}

	//TODO:
	//MEM.SWAP-UTIL, F32
}


void ResourceInfoGenerator::addDiskStats(MistMessage& msg, unsigned int specVersion, size_t movingAverageSamples)
{
	//This function responsible for adding:
	//NUM-OF-DISKS
	//DISK.N.NAME
	//DISK.N.SIZE
	//DISK.N.UTIL

	DiskStats stats_this_iteration;

	stats_this_iteration.num_of_disks = 0;

#ifdef WIN32

	int       drType = NULL;
	char      drAvail[CHAR_BUF_LEN];//Max drive str size
	char*     temp = drAvail;
	GMSEC_I16 driveCount = 0;

	GetLogicalDriveStrings(CHAR_BUF_LEN, drAvail);

	while (*temp != NULL)
	{ // Split the buffer by null
		drType=GetDriveType(temp);

		if (drType == DRIVE_FIXED)
		{
			++driveCount;

			StatsForOneDisk stats_for_one_disk;

			stats_for_one_disk.disk_name = temp;

			unsigned __int64 i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;

			BOOL fResult = GetDiskFreeSpaceEx(temp, (PULARGE_INTEGER)&i64FreeBytesToCaller, (PULARGE_INTEGER)&i64TotalBytes, (PULARGE_INTEGER)&i64FreeBytes);

			if (fResult)
			{
				GMSEC_F32 size = static_cast<GMSEC_F32>(i64TotalBytes) / (1024*1024);
				GMSEC_F32 free = static_cast<GMSEC_F32>(i64FreeBytes) / (1024*1024);
				GMSEC_F32 util;

				if (size > 0)
				{
					util = ((size - free) / size) * 100.0f;
				}
				else
				{
					util = 100.0;
				}

				stats_for_one_disk.disk_size = size;
				stats_for_one_disk.disk_util = util;
			}

			stats_this_iteration.stats.push_back(stats_for_one_disk);
		}

        temp += lstrlen(temp) +1; // incriment the buffer
    }

	stats_this_iteration.num_of_disks = driveCount;

#elif defined(__linux__)

	//
	// Read from Linux pseudo-file /proc/mounts to obtain mounted disks
	// info.
	//
	// The file fields are:
	//
	// 		 1 - device path
	// 		 2 - mount directory
	// 		 3 - filesystem type
	// 		 4 - mount options
	// 		 5 - filesystem dump info
	// 		 6 - used by fsck
	//

	char           buffer[CHAR_BUF_LEN];
	std::ifstream  ifs("/proc/mounts", std::ios::in);
	GMSEC_I16      num_of_disks = 0;

	while (ifs.getline(buffer, sizeof(buffer), '\n'))
	{
		char*       token       = strtok(buffer, " \t");
		int         token_count = 0;
		std::string device_path;
		std::string mount_directory;

		while (token != NULL)
		{
			++token_count;

			if (token_count == 1)
			{
				device_path = token;
			}
			else if (token_count == 2)
			{
				mount_directory = token;

				if (StringUtil::stringCompareCount(device_path.c_str(), "/dev", 4) == 0)
				{
					struct statfs stat_buf;
	
					if (statfs(mount_directory.c_str(), &stat_buf) == 0) // success
					{
						StatsForOneDisk stats_for_one_disk;

						++num_of_disks;

						stats_for_one_disk.disk_name = device_path;

						GMSEC_I64 disk_size_in_bytes = stat_buf.f_blocks * stat_buf.f_bsize;
	
						stats_for_one_disk.disk_size = static_cast<GMSEC_F32>(disk_size_in_bytes) / static_cast<GMSEC_F32>(1024.0*1024.0);

						GMSEC_F32 disk_free_percentage = static_cast<GMSEC_F32>(stat_buf.f_bfree) / static_cast<GMSEC_F32>(stat_buf.f_blocks);

						if (!std::isfinite(disk_free_percentage))
						{
							disk_free_percentage = 0.0f;   // assume disk is full and/or is not writable
						}

						stats_for_one_disk.disk_util = ((1.0f - disk_free_percentage) * 100.0);

						stats_this_iteration.stats.push_back(stats_for_one_disk);
					}
					else
					{
						std::ostringstream err_str;
						err_str << "Failed to query disk stats on " << device_path << " [" << strerror(errno) << "]";
						GMSEC_WARNING << err_str.str().c_str();
					}
				}
			}

			token = strtok(NULL, " \t");
		}
	}

	ifs.close();

	stats_this_iteration.num_of_disks = num_of_disks;

#elif defined(__APPLE__)

	//
	// Execute df to get mounted disks info.
	//
	// The file fields are:
	//
	// 		 1 - device path to mount
	// 		 2 - 512-blocks
	// 		 3 - Used
	// 		 4 - Available
	// 		 5 - Capacity
	// 		 6 - iused
	// 		 7 - ifree
	// 		 8 - %iused
	// 		 9 - Mounted on
	//

	char       buffer[CHAR_BUF_LEN];
	GMSEC_I16  num_of_disks = 0;
	FILE*      pipe_output_fp = popen("/bin/df", "r");

	std::istringstream iss;
	pipeStreamToInputStream(iss, pipe_output_fp);
	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer), '\n'))
	{
		char*       token       = strtok(buffer, " \t");
		int         token_count = 0;
		std::string device_path;
		std::string mount_directory;

		while (token != NULL)
		{
			++token_count;

			if (token_count == 1)
			{
				device_path = token;
			}
			else if (token_count == 9)
			{
				mount_directory = token;

				if (StringUtil::stringCompareCount(device_path.c_str(), "/dev/", 5) == 0 && strstr(mount_directory.c_str(), "/") != NULL)
				{
					struct statvfs stat_buf;

					if (statvfs(mount_directory.c_str(), &stat_buf) == 0) // success
					{
						StatsForOneDisk stats_for_one_disk;

						++num_of_disks;

						stats_for_one_disk.disk_name = device_path;

						GMSEC_I64 disk_size_in_bytes = static_cast<GMSEC_I64>(stat_buf.f_blocks) * static_cast<GMSEC_I64>(stat_buf.f_frsize);

						stats_for_one_disk.disk_size = static_cast<GMSEC_F32>(disk_size_in_bytes) / static_cast<GMSEC_F32>(1024.0*1024.0);

						GMSEC_F64 disk_free_percentage = static_cast<GMSEC_F64>(stat_buf.f_bfree) / static_cast<GMSEC_F64>(stat_buf.f_blocks);

						GMSEC_F64 disk_util_percentage = 1.0 - disk_free_percentage;

						stats_for_one_disk.disk_util = static_cast<GMSEC_F32>(disk_util_percentage * 100.0);

						stats_this_iteration.stats.push_back(stats_for_one_disk);
					}
					else
					{
						std::ostringstream err_str;
						err_str << "Failed to query disk stats on " << device_path << " [" << strerror(errno) << "]";
						GMSEC_WARNING << err_str.str().c_str();
					}
				}
			}

			token = strtok(NULL, " \t");
		}
	}

	stats_this_iteration.num_of_disks = num_of_disks;

#else
	throw Exception(MIST_ERROR, RESOURCE_DISK_MEMORY_ERROR, "Failed to query disk stats: unknown operating system");
#endif

	//
	// This section takes a queue of disk stats and performs
	// a moving average, based on past calls to this function where
	// the data was recorded in a queue of fixed size (where
	// one old sample is disposed of and a new sample is added,
	// per call to this function).
	//

	while (m_diskStatsQueue.size() >= movingAverageSamples)
	{
		m_diskStatsQueue.pop_front();
	}

	m_diskStatsQueue.push_back(stats_this_iteration);

	for (int disk_count = 0; disk_count < stats_this_iteration.num_of_disks; ++disk_count)
	{
		GMSEC_F32 average_disk_size = 0.0;
		GMSEC_F32 average_disk_util = 0.0;

		for (std::deque<DiskStats>::iterator it = m_diskStatsQueue.begin(); it != m_diskStatsQueue.end(); ++it)
		{
			average_disk_size += it->stats[disk_count].disk_size;
			average_disk_util += it->stats[disk_count].disk_util;
		}

		average_disk_size /= static_cast<GMSEC_F32>(m_diskStatsQueue.size());
		average_disk_util /= static_cast<GMSEC_F32>(m_diskStatsQueue.size());

		char field_key[CHAR_BUF_LEN];
	
		StringUtil::stringFormat(field_key, sizeof(field_key), "DISK.%d.NAME", disk_count+1);
		msg.addField(field_key, stats_this_iteration.stats[disk_count].disk_name.c_str());

		StringUtil::stringFormat(field_key, sizeof(field_key), "DISK.%d.SIZE", disk_count+1);

		if (specVersion <= GMSEC_ISD_2014_00)
		{
			msg.addField(field_key, average_disk_size);
		}
		else
		{
			msg.addField(field_key, static_cast<GMSEC_U32>(average_disk_size));
		}

		StringUtil::stringFormat(field_key, sizeof(field_key), "DISK.%d.UTIL", disk_count+1);
		msg.addField(field_key, average_disk_util);
	}

	if (specVersion <= GMSEC_ISD_2014_00)
	{
		msg.addField("NUM-OF-DISKS", stats_this_iteration.num_of_disks);
	}
	else
	{
		msg.addField("NUM-OF-DISKS", static_cast<GMSEC_U16>(stats_this_iteration.num_of_disks));
	}
}


void ResourceInfoGenerator::addCPUStats(MistMessage& msg, unsigned int specVersion, size_t movingAverageSamples)
{
	//This function is responsible for:
	//NUM-OF-CPUS, I16
	//CPU.n.UTIL, F32

	CPUStats stats_this_iteration;

	stats_this_iteration.num_cpus       = 0;
	stats_this_iteration.cpu_total_util = 0.0;

#ifdef WIN32

	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);

	stats_this_iteration.num_cpus = static_cast<GMSEC_I16>(systemInfo.dwNumberOfProcessors);

	unsigned int tries = 0;
	unsigned int wait_until = 1;

	PDH_STATUS status = ERROR_SUCCESS;

	while (tries < 3)
	{
		PDH_HQUERY            cpuQuery = NULL;

		PDH_HCOUNTER          totalCounter  = NULL;
		PDH_HCOUNTER*         indivCounters = new PDH_HCOUNTER[systemInfo.dwNumberOfProcessors];

		PDH_FMT_COUNTERVALUE  totalUtilization;
		PDH_FMT_COUNTERVALUE* indivUtilization = new PDH_FMT_COUNTERVALUE[systemInfo.dwNumberOfProcessors];

		// Open query
		//
		status = PdhOpenQuery(NULL, 0, &cpuQuery);
		
		if (checkStatus("PdhOpenQuery", status))
		{
			// Add counters for the Total CPU utilization and for the individual CPU utilizations
			//
			status = PdhAddCounter(cpuQuery, TEXT("\\Processor(_Total)\\% Processor Time"), 0, &totalCounter);

			if (checkStatus("\\Processor(_Total)\\% Processor Time", status))
			{
				for (unsigned int i = 0; i < systemInfo.dwNumberOfProcessors && status == ERROR_SUCCESS; ++i)
				{
					std::ostringstream oss;
					oss << "\\Processor(" << i << ")\\% Processor Time";

					status = PdhAddCounter(cpuQuery, TEXT(oss.str().c_str()), 0, &indivCounters[i]);
					checkStatus(oss.str().c_str(), status);
				}
			}

			if (status == ERROR_SUCCESS)
			{
				//MEH - on Windows 64 bit systems, the counter may not attach correctly without
				//      a wait. In this case, we will try successively longer waits.
				TimeUtil::millisleep(tries * 1000);


				// Collect the queried data
				//
				status = PdhCollectQueryData(cpuQuery);

				if (checkStatus("PdhCollectQueryData", status))
				{
					HANDLE event = CreateEvent(NULL, FALSE, FALSE, "MyEvent");

					status = PdhCollectQueryDataEx(cpuQuery, wait_until, event);

					if (checkStatus("PdhCollectQueryDataEx", status))
					{
						WaitForSingleObject(event, INFINITE);

						// Get the Total CPU utilization reading
						//
						status = PdhGetFormattedCounterValue(totalCounter, PDH_FMT_DOUBLE, NULL, &totalUtilization);

						if (checkStatus("PdhGetFormattedCounterValue Total", status))
						{
							stats_this_iteration.cpu_total_util = static_cast<GMSEC_F32>(totalUtilization.doubleValue);

							// Get the individual CPU utilization readings
							//
							for (unsigned int i = 0; i < systemInfo.dwNumberOfProcessors && status == ERROR_SUCCESS; ++i)
							{
								status = PdhGetFormattedCounterValue(indivCounters[i], PDH_FMT_DOUBLE, NULL, &indivUtilization[i]);

								if (checkStatus("PdhGetFormattedCounterValue Individual", status))
								{
									stats_this_iteration.cpu_utils.push_back(static_cast<GMSEC_F32>(indivUtilization[i].doubleValue));
								}
							}
						}
					}

					if (event != NULL)
					{
						CloseHandle(event);
					}
				}
			}
		}

		if (cpuQuery)
		{
			PdhCloseQuery(cpuQuery);
		}

		delete [] indivCounters;
		delete [] indivUtilization;

		if (status == ERROR_SUCCESS)
		{
			break;
		}
		else
		{
			stats_this_iteration.cpu_total_util = 0.0;
			stats_this_iteration.cpu_utils.clear();

			wait_until *= 2;  // increase delay
			++tries;          // try again
		}
	}

	if (tries == 3 && status != ERROR_SUCCESS)
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_CPU_ERROR, "Unable to collect system CPU resource information");
	}

#elif defined(__linux__)

	//
	// Read from Linux pseudo-file /proc/stat to obtain cpu loading
	// info.
	//
	// The file format is:
	//
	// /proc/stat
	//     kernel/system statistics.   Varies  with  architecture.   Common
	//     entries include:
	// 
	//     cpu  3357 0 4313 1362393
	//         The   amount  of  time,  measured  in  units  of  USER_HZ
	//         (1/100ths  of  a  second  on  most   architectures,   use
	//         sysconf(_SC_CLK_TCK) to obtain the right value), that the
	//         system spent in user mode, user mode  with  low  priority
	//         (nice),  system  mode,  and  the idle task, respectively.
	//         The last value should be USER_HZ times the  second  entry
	//         in the uptime pseudo-file.
	// 
	//         In Linux 2.6 this line includes three additional columns:
	//         iowait - time waiting for I/O to complete (since 2.5.41);
	//         irq  -  time  servicing  interrupts  (since 2.6.0-test4);
	//         softirq - time servicing softirqs (since 2.6.0-test4).
	// 
	//         Since Linux 2.6.11, there is an eighth  column,  steal  -
	//         stolen  time,  which is the time spent in other operating
	//         systems when running in a virtualized environment
	// 
	//         Since Linux 2.6.24, there is a ninth column, guest, which
	//         is the time spent running a virtual CPU for guest operat-
	//         ing systems under the control of the Linux kernel.
	//

	//
	// Sample user+system ticks and total ticks twice.  Get the difference
	// between the samples for each data point.
	//
	// Then divide delta_user_and_system_ticks/delta_total_ticks.
	//

	char          buffer[CHAR_BUF_LEN];
	GMSEC_I16     num_cpus = 0;
	CPUTickStruct cpu_tick_struct;
	std::ifstream ifs("/proc/stat", std::ios::in);

	while (ifs.getline(buffer, sizeof(buffer), '\n'))
	{
		char* current_str = strtok(buffer, " ");

		if (current_str != NULL)
		{
			if (StringUtil::stringCompareCount(current_str, "cpu", 3) == 0 && !StringUtil::stringEquals(current_str, "cpu")) // want "cpu0" and "cpu1" etc. but not just "cpu"
			{
				current_str = strtok(NULL, "\n");

				if (current_str == NULL) continue;

				int num_args = sscanf(current_str, "%lu %lu %lu %lu",
				                 &cpu_tick_struct.user_mode_ticks,
				                 &cpu_tick_struct.nice_mode_ticks,
				                 &cpu_tick_struct.system_mode_ticks,
				                 &cpu_tick_struct.idle_mode_ticks);

				if (num_args == 4)
				{
					++num_cpus;

					GMSEC_U32 util_ticks  = cpu_tick_struct.user_mode_ticks + cpu_tick_struct.system_mode_ticks;
					GMSEC_U32 total_ticks = cpu_tick_struct.user_mode_ticks + cpu_tick_struct.system_mode_ticks + cpu_tick_struct.idle_mode_ticks;

					stats_this_iteration.util_ticks.push_back(util_ticks);
					stats_this_iteration.total_ticks.push_back(total_ticks);
				}
			}
		}
	}

	ifs.close();

	stats_this_iteration.num_cpus = num_cpus;

#elif defined(__APPLE__)

	char          buffer[CHAR_BUF_LEN];
	bool          done = false;
	int           num_args = 0;
	float         dummy = 0.0;
	float         user = 0.0;
	float         sys = 0.0;
	float         idle = 0.0;
	int           num_cpus = 0;
	bool          ready_to_fetch_cpu_loads = false;
	CPUTickStruct cpu_tick_struct;

	FILE* pipe_output_fp = popen("/usr/bin/top -l 1", "r");

	std::istringstream iss;

	pipeStreamToInputStream(iss, pipe_output_fp);

	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer), '\n') && !done)
	{
		if (StringUtil::stringCompareCount(buffer, "CPU usage:", 10) == 0)
		{
			//
			// Get rid of '%' chars in buffer; sscanf doesn't deal with
			// them gracefully.
			//
			unsigned int index = 0;

			while (buffer[index] != '\0' && index < sizeof(buffer)-1)
			{
				if (buffer[index] == '%')
				{
					buffer[index] = ' ';
				}

				++index;
			}

			num_args = sscanf(buffer, "CPU usage: %f user, %f sys, %f idle", &user, &sys, &idle);

			if (num_args == 3)
			{
				GMSEC_F32 cpu_util = ((user + sys) / (user + sys + idle)) * 100.0;

				if (!std::isfinite(cpu_util))
				{
					cpu_util = 0.0;  // if user + sys + idle is zero (unlikely), assume no CPU utilization
				}

				stats_this_iteration.cpu_total_util = cpu_util;

				done = true;
			}
			else
			{
				throw Exception(MIST_ERROR, RESOURCE_INFO_CPU_ERROR, "Failed to query CPU utilization");
			}
		}
	}

	size_t size = sizeof(num_cpus);
	if (sysctlbyname("hw.logicalcpu", &num_cpus, &size, NULL, 0) != 0)
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_CPU_ERROR, "Failed to query number of CPUs");
	}

	stats_this_iteration.num_cpus = static_cast<GMSEC_I16>(num_cpus);

	for (size_t synth_cpu_count = 0; synth_cpu_count < num_cpus; ++synth_cpu_count)
	{
		if (synth_cpu_count == 0)
		{
			stats_this_iteration.cpu_utils.push_back(stats_this_iteration.cpu_total_util);
		}
		else
		{
			stats_this_iteration.cpu_utils.push_back(0.0);
		}
	}

#else
	throw Exception(MIST_ERROR, RESOURCE_INFO_CPU_ERROR, "Failed to query CPU stats: unknown operating system");
#endif

	//
	// This section takes a queue of cpu stats and performs
	// a moving average, based on past calls to this function where
	// the data was recorded in a queue of fixed size (where
	// one old sample is disposed of and a new sample is added,
	// per call to this function).
	//

	while (m_cpuStatsQueue.size() >= movingAverageSamples)
	{
		m_cpuStatsQueue.pop_front();
	}

	m_cpuStatsQueue.push_back(stats_this_iteration);

	for (int cpu_count = 0; cpu_count < stats_this_iteration.num_cpus; ++cpu_count)
	{
		//
		// In Linux there are raw accumulating ticks, in other OS's
		// there are not.  That is why there are two different
		// implementations below; calculating using raw accumulating 
		// ticks gives a more reliable result overall.
		//
		GMSEC_F32 average_cpu_utils = 0.0;

#ifndef __linux__

		//
		// This is for non-Linux
		//
		for (std::deque<CPUStats>::iterator it = m_cpuStatsQueue.begin(); it != m_cpuStatsQueue.end(); ++it)
		{
			average_cpu_utils += it->cpu_utils[cpu_count];
		}

		average_cpu_utils /= static_cast<GMSEC_F32>(m_cpuStatsQueue.size());

#else

		//
		// This is for Linux
		//
		GMSEC_F32 util_ticks_delta  = 0.0;
		GMSEC_F32 total_ticks_delta = 0.0;

		if (m_cpuStatsQueue.size() > 0)
		{
			GMSEC_U32 start_util_ticks  = 0;
			GMSEC_U32 start_total_ticks = 0;

			if (m_cpuStatsQueue.size() > 1)
			{
		  		start_util_ticks  = m_cpuStatsQueue.front().util_ticks[cpu_count];
		  		start_total_ticks = m_cpuStatsQueue.front().total_ticks[cpu_count];
			}

			GMSEC_U32 end_util_ticks  = m_cpuStatsQueue.back().util_ticks[cpu_count];
			GMSEC_U32 end_total_ticks = m_cpuStatsQueue.back().total_ticks[cpu_count];

			util_ticks_delta  = end_util_ticks - start_util_ticks;
			total_ticks_delta = end_total_ticks - start_total_ticks;

			if (total_ticks_delta > 0)
			{
				average_cpu_utils = (static_cast<GMSEC_F32>(util_ticks_delta) / static_cast<GMSEC_F32>(total_ticks_delta)) * 100.0;
			}
		}

#endif

		char field_key[CHAR_BUF_LEN];

		StringUtil::stringFormat(field_key, sizeof(field_key), "CPU.%d.UTIL", cpu_count+1);
		msg.addField(field_key, average_cpu_utils);
	}// for cpu_count

	if (specVersion <= GMSEC_ISD_2014_00)
	{
		msg.addField("NUM-OF-CPUS", stats_this_iteration.num_cpus);
	}
	else
	{
		msg.addField("NUM-OF-CPUS", static_cast<GMSEC_U16>(stats_this_iteration.num_cpus));
	}

	//TODO:
	//CPU.n.MEM, U32
	//CPU.n.MEM-UTIL, F32
	//CPU.n.PAGE-FAULTS, U32
}


void ResourceInfoGenerator::addNetworkStats(MistMessage& msg, size_t sampleInterval, size_t movingAverageSamples)
{
	//This function is responsible for:
	//NUM-OF-NET-PORTS
	//NET-PORT.n.NAME
	//NET-PORT.n.EUI-ADR
	//NET-PORT.n.IP-ADR
	//NET-PORT.n.TOTAL-BANDWIDTH
	//NET-PORT.n.UTIL
	//NET-PORT.n.BYTES-SENT
	//NET-PORT.n.BYTES-RECEIVED
	//NET-PORT.n.ERRORS

	NetworkStats stats_this_iteration;

#ifdef WIN32

	ULONG            ulOutBufLen  = sizeof(IP_ADAPTER_INFO);
	IP_ADAPTER_INFO* pAdapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>( malloc(ulOutBufLen) );

	if (!pAdapterInfo)
	{
		GMSEC_WARNING << "Error allocating memory needed for GetAdaptersInfo";
		return;
	}

	// Two calls to GetAdaptersInfo() needed; first to get the necessary buffer size.
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
	{
		free(pAdapterInfo);

		pAdapterInfo = reinterpret_cast<IP_ADAPTER_INFO*>( malloc(ulOutBufLen) );

		if (!pAdapterInfo)
		{
			GMSEC_WARNING << "Error allocating memory following call to GetAdaptersInfo";
			return;
		}
	}

	// The second call to get the actual data.
	DWORD dwRetVal;
	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != ERROR_SUCCESS)
	{
		free(pAdapterInfo);

		GMSEC_WARNING << "GetAdaptersInfo call failed with error: " << dwRetVal;
		return;
	}

	struct Adapter
	{
		std::string name;
		std::string ip_addr;
	};
	std::vector<Adapter> adapters;

	// Acquire short list of our known network adapters (and IP addresses)
	PIP_ADAPTER_INFO pAdapter = pAdapterInfo;

	while (pAdapter)
	{
		Adapter adapter = {  pAdapter->AdapterName, pAdapter->IpAddressList.IpAddress.String };

		adapters.push_back(adapter);

		pAdapter = pAdapter->Next;
	}

	free(pAdapterInfo);


	DWORD dwSize = sizeof(MIB_IFTABLE);

	/* variables used for GetIfTable and GetIfEntry */
	MIB_IFTABLE* pIfTable = reinterpret_cast<MIB_IFTABLE*>(malloc(dwSize));

	// Verify memory was allocated.
	if (pIfTable == NULL)
	{
		GMSEC_WARNING << "Error allocating memory needed to call GetIfTable";
		return;
	}

	// Make an initial call to GetIfTable to get the necessary size into dwSize
	if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER)
	{
		free(pIfTable);

		pIfTable = reinterpret_cast<MIB_IFTABLE*>(malloc(dwSize));

		if (pIfTable == NULL)
		{
			GMSEC_WARNING << "Error allocating memory following call to GetIfTable";
			return;
		}
	}

	// Make a second call to GetIfTable to get the actual data we want.
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) != NO_ERROR)
	{
		free(pIfTable);

		GMSEC_WARNING << "GetIfTable call failed with error: " << dwRetVal;
		return;
	}

	for (DWORD i = 0; i < pIfTable->dwNumEntries; ++i)
	{
		MIB_IFROW* pIfRow = reinterpret_cast<MIB_IFROW*>(& pIfTable->table[i]);

		std::string if_name = wideStringToString( pIfRow->wszName );
		std::string if_addr;

		// If we do not recognize the adapter, ignore it
		for (std::vector<Adapter>::const_iterator it = adapters.begin(); it != adapters.end(); ++it)
		{
			if (if_name.find(it->name) != std::string::npos)
			{
				if_addr = it->ip_addr;
				break;
			}
		}

		if (if_addr.empty() || if_addr == "0.0.0.0")
		{
			continue;
		}

		// We know this adapter; put together the MAC (physical) address.

		std::ostringstream macAddr;

		for (DWORD j = 0; j < pIfRow->dwPhysAddrLen; ++j)
		{
			TCHAR szCounterName[CHAR_BUF_LEN];

			sprintf_s(szCounterName, sizeof(szCounterName) - 1, "%.2X", static_cast<int>(pIfRow->bPhysAddr[j]));

			macAddr << szCounterName << (j == (pIfRow->dwPhysAddrLen - 1) ? "" : "-");
		}

		StatsForOneNet stats_for_one_net;

		stats_for_one_net.if_name   = if_name;
		stats_for_one_net.ip_addr   = if_addr;
		stats_for_one_net.eui_addr  = macAddr.str();
		stats_for_one_net.bytesRX   = pIfRow->dwInOctets;
		stats_for_one_net.bytesTX   = pIfRow->dwOutOctets;
		stats_for_one_net.errorsRX  = pIfRow->dwInErrors;
		stats_for_one_net.errorsTX  = pIfRow->dwOutErrors;
		stats_for_one_net.bandwidth = pIfRow->dwSpeed / 1024;   // convert from bits to Kbits

		stats_this_iteration.stats.push_back(stats_for_one_net);

		++stats_this_iteration.num_net_ports;
	}

	if (pIfTable != NULL)
	{
		free(pIfTable);
	}


#elif defined(__linux__)

	struct ifaddrs* ifap    = NULL;

	if (getifaddrs(&ifap) == 0) // success
	{
		struct ifaddrs* ifap_head = ifap;

		while (ifap != NULL)
		{
			if (reinterpret_cast<struct sockaddr_in*>(ifap->ifa_addr)->sin_family == AF_INET)
			{
				StatsForOneNet stats_for_one_net;

				stats_for_one_net.if_name   = ifap->ifa_name;
				stats_for_one_net.ip_addr   = inet_ntoa(reinterpret_cast<struct sockaddr_in*>(ifap->ifa_addr)->sin_addr);
				stats_for_one_net.eui_addr  = getNetworkValue<std::string>(ifap->ifa_name, "address", "");
				stats_for_one_net.bytesRX   = getNetworkValue<GMSEC_U64>(ifap->ifa_name, "statistics/rx_bytes", 0);
				stats_for_one_net.bytesTX   = getNetworkValue<GMSEC_U64>(ifap->ifa_name, "statistics/tx_bytes", 0);
				stats_for_one_net.errorsRX  = getNetworkValue<GMSEC_U32>(ifap->ifa_name, "statistics/rx_errors", 0);
				stats_for_one_net.errorsTX  = getNetworkValue<GMSEC_U32>(ifap->ifa_name, "statistics/tx_errors", 0);
				stats_for_one_net.bandwidth = getNetworkValue<GMSEC_U32>(ifap->ifa_name, "speed", 0) * 1024;  // convert Mbits to Kbits

				stats_this_iteration.stats.push_back(stats_for_one_net);

				++stats_this_iteration.num_net_ports;
			}

			ifap = ifap->ifa_next;
		}

		if (ifap_head != NULL)
		{
			freeifaddrs(ifap_head); // Please read man page for getifaddrs about dynamic allocation of ifap and the need to (ideally) free it after use
		}
	}
	else // failure on call to getifaddrs()
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_NET_ERROR, "Failed to query network stats:  failed call to getifaddrs()");
	}

#elif defined(__APPLE__)

	char buffer[CHAR_BUF_LEN] = {0};

	FILE* pipe_output_fp = popen("/usr/sbin/netstat -in", "r");
	std::istringstream iss;
	pipeStreamToInputStream(iss, pipe_output_fp);
	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer) - 1, '\n'))
	{
		char interface_name[CHAR_BUF_LEN] = {0};
		char dummy[CHAR_BUF_LEN] = {0};
		char address[CHAR_BUF_LEN] = {0};

		int num_args = sscanf(buffer, "%s %s %s %s", interface_name, dummy, dummy, address);

		if (num_args == 4)
		{
			if (strstr(address, ".") != NULL)
			{
				StatsForOneNet stats_for_one_net;

				stats_for_one_net.if_name = interface_name;

				getNetworkInfo(interface_name, stats_for_one_net.ip_addr, stats_for_one_net.eui_addr);

				// DMW TODO - Need to acquire bytes RX/TX, errors RX/TX, and bandwidth.

				stats_this_iteration.stats.push_back(stats_for_one_net);

				++stats_this_iteration.num_net_ports;
			}
		}
	}

#else
	throw Exception(MIST_ERROR, RESOURCE_INFO_NET_ERROR, "Failed to query network stats:  unknown operating system");
#endif

	//
	// This section takes a queue of net stats and performs
	// a moving average, based on past calls to this function where
	// the data was recorded in a queue of fixed size (where
	// one old sample is disposed of and a new sample is added,
	// per call to this function).
	//

	while (m_networkStatsQueue.size() >= movingAverageSamples)
	{
		m_networkStatsQueue.pop_front();
	}

	m_networkStatsQueue.push_back(stats_this_iteration);

	for (int net_port_count = 0; net_port_count < stats_this_iteration.num_net_ports; ++net_port_count)
	{
		char field_key[CHAR_BUF_LEN];

		StringUtil::stringFormat(field_key, sizeof(field_key), "NET-PORT.%d.NAME", net_port_count+1);
		msg.addField(field_key, stats_this_iteration.stats[net_port_count].if_name.c_str());

		StringUtil::stringFormat(field_key, sizeof(field_key), "NET-PORT.%d.IP-ADR", net_port_count+1);
		msg.addField(field_key, stats_this_iteration.stats[net_port_count].ip_addr.c_str());

		if (!stats_this_iteration.stats[net_port_count].eui_addr.empty())
		{
			StringUtil::stringFormat(field_key, sizeof(field_key), "NET-PORT.%d.EUI-ADR", net_port_count+1);
			msg.addField(field_key, stats_this_iteration.stats[net_port_count].eui_addr.c_str());
		}

		// We only care about bandwidth, utilization of such, and bytes sent/received if the bandwidth is non-zero.
		if (stats_this_iteration.stats[net_port_count].bandwidth > 0)
		{
			StringUtil::stringFormat(field_key, sizeof(field_key), "NET-PORT.%d.BYTES-RECEIVED", net_port_count+1);
			msg.setValue(field_key, static_cast<GMSEC_I64>(stats_this_iteration.stats[net_port_count].bytesRX));

			StringUtil::stringFormat(field_key, sizeof(field_key), "NET-PORT.%d.BYTES-SENT", net_port_count+1);
			msg.setValue(field_key, static_cast<GMSEC_I64>(stats_this_iteration.stats[net_port_count].bytesTX));

			StringUtil::stringFormat(field_key, sizeof(field_key), "NET-PORT.%d.ERRORS", net_port_count+1);
			msg.setValue(field_key, static_cast<GMSEC_I64>(stats_this_iteration.stats[net_port_count].errorsRX + stats_this_iteration.stats[net_port_count].errorsTX));

			StringUtil::stringFormat(field_key, sizeof(field_key), "NET-PORT.%d.TOTAL-BANDWIDTH", net_port_count+1);
			msg.setValue(field_key, static_cast<GMSEC_I64>(stats_this_iteration.stats[net_port_count].bandwidth));

			// Compute utilization (for optimal results, we need at least two sample readings, of which only the last two are examined)
			// If less than two samples are available, then assume utilization to be 0%.
			GMSEC_F32 utilization = 0.0f;

			if (m_networkStatsQueue.size() >= 2)
			{
				static GMSEC_F32 BYTES_PER_KBIT = 128.0f;

				const NetworkStats& stats1 = m_networkStatsQueue[ m_networkStatsQueue.size() - 1 ];
				const NetworkStats& stats0 = m_networkStatsQueue[ m_networkStatsQueue.size() - 2 ];

				GMSEC_U64 delta_bytesRX = std::llabs( stats1.stats[net_port_count].bytesRX - stats0.stats[net_port_count].bytesRX );
				GMSEC_U64 delta_bytesTX = std::llabs( stats1.stats[net_port_count].bytesTX - stats0.stats[net_port_count].bytesTX );
				GMSEC_U64 total_bytes   = delta_bytesRX + delta_bytesTX;

				utilization = static_cast<GMSEC_F32>(total_bytes) / sampleInterval / (static_cast<GMSEC_F32>(stats1.stats[net_port_count].bandwidth) * BYTES_PER_KBIT) * 100.0f;

				if (!std::isfinite(utilization))
				{
					GMSEC_WARNING << "Network utilization percentage is not finite [" << utilization << "]; reporting as 0.0";
					utilization = 0.0f;
				}
			}

			StringUtil::stringFormat(field_key, sizeof(field_key), "NET-PORT.%d.UTIL", net_port_count+1);
			msg.addField(field_key, utilization);
		}
	}

	msg.setValue("NUM-OF-NET-PORTS", static_cast<GMSEC_I64>(stats_this_iteration.num_net_ports));
}


std::string ResourceInfoGenerator::getOSVersion()
{
#ifdef WIN32

	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    if (GetVersionEx(&osvi) != 0)
	{
		std::ostringstream ss;
		ss << "Windows version: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion;
		return ss.str();
	}
	else
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR, "Failed to query Windows version information");
	}

#elif defined(__linux__) || defined(__APPLE__)

	struct utsname utsname_buf;

	if (uname(&utsname_buf) >= 0) // success
	{
		std::ostringstream ss;
		ss << "OS version: " << utsname_buf.sysname << " "
		   << utsname_buf.release << " "
		   << utsname_buf.version << " "
		   << utsname_buf.machine;

		return ss.str();
	}
	else // failure in query
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_OS_VERSION_ERROR, "Failed to query OS version");
	}

#else
	return "Unknown Operating System";
#endif    
}
