/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file ResourceInfoGenerator.cpp
 *
 *  @brief The ResourceInfoGenerator encapsulates common API functions for 
 *  gathering system resource information and adding them to a GMSEC message
 *
**/

#include <gmsec4/internal/mist/ResourceInfoGenerator.h>

#include <gmsec4/mist/mist_defs.h>

#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>

#include <gmsec4/util/Log.h>
#include <gmsec4/util/TimeUtil.h>

#include <gmsec4/internal/StringUtil.h>
#include <gmsec4/internal/SystemUtil.h>

#include <deque>
#include <iomanip>
#include <sstream>
#include <string>
#include <cstdlib>

#ifdef WIN32
#include <windows.h>
#include "pdh.h"
#include "pdhmsg.h"
#include "TCHAR.h"
#include <iphlpapi.h>
#elif defined(__sun)
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/statvfs.h>
#include <sys/utsname.h>
#elif defined(__hpux)
#include <iostream>
#include <fstream>
#include <sys/statvfs.h>
#include <sys/utsname.h>
#include <unistd.h>
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
#include <sys/types.h>
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
using namespace gmsec::api::mist::internal;


std::deque<ResourceInfoGenerator::MainMemoryStats> ResourceInfoGenerator::m_mainMemoryStatsQueue;
std::deque<ResourceInfoGenerator::DiskStats>       ResourceInfoGenerator::m_diskStatsQueue;
std::deque<ResourceInfoGenerator::CPUStats>        ResourceInfoGenerator::m_cpuStatsQueue;
std::deque<ResourceInfoGenerator::NetworkStats>    ResourceInfoGenerator::m_networkStatsQueue;


void ResourceInfoGenerator::addMainMemoryStats(Message &msg, unsigned int version, size_t movingAverageSamples)
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
		stats_this_iteration.memory_percent_utilized   = (GMSEC_F32) statex.dwMemoryLoad;
		stats_this_iteration.total_physical_memory     = statex.ullTotalPhys;
		stats_this_iteration.available_physical_memory = statex.ullAvailPhys;
		stats_this_iteration.total_virtual_memory      = statex.ullTotalVirtual;
		stats_this_iteration.available_virtual_memory  = statex.ullAvailVirtual;
	}
	else
	{
		GMSEC_WARNING << "GlobalMemoryStatusEx() failed.";
		throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR,
		                "Failed to query virtual memory stats");
	}

#elif defined(__sun)

	long memory_page_size        = sysconf(_SC_PAGE_SIZE);
	long memory_physical_pages   = sysconf(_SC_PHYS_PAGES);
	long memory_pages_not_in_use = sysconf(_SC_AVPHYS_PAGES);

	GMSEC_F32 memory_percent_utilized =
	  1.0 - ((GMSEC_F32)(memory_pages_not_in_use)/(GMSEC_F32)memory_physical_pages);

	GMSEC_I64 total_physical_memory =
	  (GMSEC_I64)memory_page_size * (GMSEC_I64)memory_physical_pages;

	GMSEC_I64 available_physical_memory =
	  (GMSEC_I64)memory_page_size * (GMSEC_I64)memory_pages_not_in_use;

	FILE* pipe_output_fp = popen("/usr/bin/sh -c \"/usr/sbin/swap -s\"", "r");

	bool               done           = false;
	int                num_args       = 0;
	char               buffer[CHAR_BUF_LEN];
	unsigned long int  dummy          = 0;
	unsigned long int  available_vm   = 0;
	unsigned long int  used_vm        = 0;

	std::istringstream iss;

	pipeStreamToInputStream(iss, pipe_output_fp);

	pclose(pipe_output_fp);

	done = false;
	while (iss.getline(buffer, sizeof(buffer), '\n'))
	{
		num_args = sscanf(buffer, "total: %luk bytes allocated + %luk reserved = %luk used, %luk available",
		                  &dummy, &dummy,
		                  &used_vm, &available_vm);

		if (num_args == 4)
		{
			done = true;
		}
	}

	if (!done)
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR,
		                "Failed to query virtual memory stats");
	}

	GMSEC_I64 total_virtual_memory = ((GMSEC_I64)used_vm+(GMSEC_I64)available_vm)*1024;

	GMSEC_I64 available_virtual_memory = (GMSEC_I64)available_vm*1024;

	stats_this_iteration.memory_percent_utilized   = memory_percent_utilized*100.0;
	stats_this_iteration.total_physical_memory     = total_physical_memory;
	stats_this_iteration.available_physical_memory = available_physical_memory;
	stats_this_iteration.total_virtual_memory      = total_virtual_memory;
	stats_this_iteration.available_virtual_memory  = available_virtual_memory;

#elif defined(__hpux)

	system("top -f /tmp/tmpout.top");

	bool              done     = false;
	int               num_args = 0;
	char              buffer[CHAR_BUF_LEN];
	unsigned long int real_mem;
	unsigned long int available_real_mem;
	unsigned long int virtual_mem;
	unsigned long int available_virtual_mem;
	unsigned long int free_mem;

	ifstream          ifs("/tmp/tmpout.top", ios::in);

	while (ifs.getline(buffer, sizeof(buffer), '\n') && !done)
	{
		if (StringUtil::stringCompareCount(buffer, "Memory:", 7) == 0)
		{
			num_args = sscanf(buffer, "Memory: %luK (%luK) real, %luK (%luK) virtual, %luK free",
			                  &real_mem,
			                  &available_real_mem,
			                  &virtual_mem,
			                  &available_virtual_mem,
					  &free_mem);

			if (num_args == 5)
			{
				done = true;
			}
		}
	}

	if (!done)
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR,
		                "Failed to query memory stats");
	}
	else
	{
		unlink("/tmp/tmpout.top");
	}

	GMSEC_I64 total_virtual_memory = (GMSEC_I64) virtual_mem * 1024;
	GMSEC_I64 available_virtual_memory = (GMSEC_I64) available_virtual_mem * 1024;


	GMSEC_I64 total_physical_memory = (GMSEC_I64) real_mem * 1024;
	GMSEC_I64 available_physical_memory = (GMSEC_I64) available_real_mem * 1024;

	GMSEC_F32 memory_percent_utilized = 1.0 - ((GMSEC_F32) available_real_mem / (GMSEC_F32) real_mem);

	stats_this_iteration.memory_percent_utilized   = memory_percent_utilized*100.0;
	stats_this_iteration.total_physical_memory     = total_physical_memory;
	stats_this_iteration.available_physical_memory = available_physical_memory;
	stats_this_iteration.total_virtual_memory      = total_virtual_memory;
	stats_this_iteration.available_virtual_memory  = available_virtual_memory;
#elif defined(__linux__)

	struct sysinfo sys_info;

	if (sysinfo(&sys_info) == 0) // success
	{
		GMSEC_F32 memory_percent_utilized =
		  (GMSEC_F32)(sys_info.totalram-sys_info.freeram)/(GMSEC_F32)sys_info.totalram;

		GMSEC_I64 total_physical_memory =
		  (GMSEC_I64)sys_info.totalram*(GMSEC_I64)sys_info.mem_unit;

		GMSEC_I64 available_physical_memory =
		  (GMSEC_I64)sys_info.freeram*(GMSEC_I64)sys_info.mem_unit;

		GMSEC_I64 total_virtual_memory =
		  (GMSEC_I64)sys_info.totalswap*(GMSEC_I64)sys_info.mem_unit;

		GMSEC_I64 available_virtual_memory =
		  (GMSEC_I64)sys_info.freeswap*(GMSEC_I64)sys_info.mem_unit;

		stats_this_iteration.memory_percent_utilized   = memory_percent_utilized*100.0;
		stats_this_iteration.total_physical_memory     = total_physical_memory;
		stats_this_iteration.available_physical_memory = available_physical_memory;
		stats_this_iteration.total_virtual_memory      = total_virtual_memory;
		stats_this_iteration.available_virtual_memory  = available_virtual_memory;

	}
	else // failure
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR,
		                "Failed to query main memory stats");
	}

#elif defined(__APPLE__)

	system("top -l 1 > /tmp/tmpout.top");

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

	while (ifs.getline(buffer, sizeof(buffer), '\n') &&
	       (!phys_done || !virt_done))
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
			num_args = sscanf(buffer, "VM: %lu%c vsize",
			                  &virtual_mem, &units1);

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
		throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR,
		                "Failed to query memory stats");
	}
	else
	{
		unlink("/tmp/tmpout.top");
	}

	GMSEC_I64 total_virtual_memory = (GMSEC_I64) virtual_mem * 1073741824; // virtual_mem is in GBytes

	GMSEC_I64 total_physical_memory = ((GMSEC_I64) phys_mem_used + (GMSEC_I64) available_phys_mem) * 1048576; // phys_mem is in MBytes
	GMSEC_I64 available_physical_memory = (GMSEC_I64) available_phys_mem * 1048576; // phys mem is in MBytes

	GMSEC_F32 memory_percent_utilized = (GMSEC_F32) phys_mem_used / (GMSEC_F32) (phys_mem_used + available_phys_mem);

	stats_this_iteration.memory_percent_utilized   = memory_percent_utilized*100.0;
	stats_this_iteration.total_physical_memory     = total_physical_memory;
	stats_this_iteration.available_physical_memory = available_physical_memory;
	stats_this_iteration.total_virtual_memory      = total_virtual_memory;

#else
	throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR,
		        "Failed to query main memory stats:  unknown operating system");
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

	std::deque<MainMemoryStats>::iterator it = m_mainMemoryStatsQueue.begin();
	while (it != m_mainMemoryStatsQueue.end())
	{
		average_memory_percent_utilized   += it->memory_percent_utilized;
		average_total_physical_memory     += it->total_physical_memory;
		average_available_physical_memory += it->available_physical_memory;
		average_total_virtual_memory     += it->total_virtual_memory;
		average_available_virtual_memory  += it->available_virtual_memory;

		it++;
	}

	average_memory_percent_utilized   /= ((GMSEC_F32)m_mainMemoryStatsQueue.size());
	average_total_physical_memory     /= m_mainMemoryStatsQueue.size(); // The total physical memory shouldn't change (the average should be the same as a one-time sample), but keep it consistent with the other logic
	average_available_physical_memory /= m_mainMemoryStatsQueue.size();
	average_total_virtual_memory      /= m_mainMemoryStatsQueue.size(); // This total virtual memory shouldn't change (the average should be the same as a one-time sample), but keep it consistent with the other logic
	average_available_virtual_memory  /= m_mainMemoryStatsQueue.size();

	msg.addField("MEM.UTIL", average_memory_percent_utilized);

	if(version == GMSEC_ISD_2014_00){
		msg.addField("MEM.PHYSICAL.TOTAL", average_total_physical_memory); 
		msg.addField("MEM.PHYSICAL.AVAIL", average_available_physical_memory);
		msg.addField("MEM.VIRTUAL.TOTAL", average_total_virtual_memory);
		msg.addField("MEM.VIRTUAL.AVAIL", average_available_virtual_memory);
	}else if(version == GMSEC_ISD_2016_00){
		msg.addField("MEM.PHYSICAL.TOTAL", (GMSEC_U64) average_total_physical_memory); 
		msg.addField("MEM.PHYSICAL.AVAIL", (GMSEC_U64) average_available_physical_memory);
		msg.addField("MEM.VIRTUAL.TOTAL", (GMSEC_U64) average_total_virtual_memory);
		msg.addField("MEM.VIRTUAL.AVAIL", (GMSEC_U64) average_available_virtual_memory);
	}else{
		GMSEC_WARNING << "Specification version unknown: " << version << ", unable to autopopulate some RAM stats"
			<< " in Resource message";
	}
}


void ResourceInfoGenerator::addDiskStats(Message &msg, unsigned int version, size_t movingAverageSamples)
{
	//This function responsible for adding:
	//NUM-OF-DISKS
	//DISK.N.NAME
	//DISK.N.SIZE
	//DISK.N.UTIL

	DiskStats stats_this_iteration;

	stats_this_iteration.num_of_disks = 0;

#ifdef WIN32
	

	int drType=NULL;
	char drAvail[CHAR_BUF_LEN];//Max drive str size
	char *temp=drAvail;
	GetLogicalDriveStrings(CHAR_BUF_LEN, drAvail);
	GMSEC_I16 driveCount = 0;
	while(*temp != NULL) { // Split the buffer by null
		drType=GetDriveType(temp);

		if(drType == DRIVE_FIXED){
			driveCount++;

			StatsForOneDisk stats_for_one_disk;

			stats_for_one_disk.disk_name = temp;

			unsigned __int64 i64FreeBytesToCaller,
				i64TotalBytes, i64FreeBytes;

			BOOL fResult = GetDiskFreeSpaceEx (temp,
                                 (PULARGE_INTEGER)&i64FreeBytesToCaller,
                                 (PULARGE_INTEGER)&i64TotalBytes,
                                 (PULARGE_INTEGER)&i64FreeBytes);

			if (fResult)
			{
				GMSEC_F32 size = (GMSEC_F32) i64TotalBytes / (1024*1024);
				GMSEC_F32 free = (GMSEC_F32) i64FreeBytes / (1024*1024);
				GMSEC_F32 util = ((size - free) / size) * 100;

				stats_for_one_disk.disk_size = size;

				stats_for_one_disk.disk_util = util;
			}

			stats_this_iteration.stats.push_back(stats_for_one_disk);

		}

        temp += lstrlen(temp) +1; // incriment the buffer
    }

	stats_this_iteration.num_of_disks = driveCount;

#elif defined(__sun)

	//
	// Read from Solaris file /etc/vfstab to obtain mounted disks
	// info.
	//
	// The file fields are:
	//
	// 		 1 - device path to mount
	// 		 2 - device path to fsck
	// 		 3 - mount directory
	// 		 4 - filesystem type
	// 		 5 - fsck pass
	// 		 6 - mount at boot
	// 		 7 - mount options
	//

	char           buffer[CHAR_BUF_LEN];

	ifstream       ifs("/etc/vfstab", ios::in);

	GMSEC_I16      num_of_disks = 0;

	while (ifs.getline(buffer, sizeof(buffer), '\n'))
	{
		char*       token        = strtok(buffer, " \t");
		int         token_count  = 0;
		std::string device_path;
		std::string mount_directory;

		while (token != NULL)
		{
			token_count++;

			if (token_count == 1)
			{
				device_path = token;
			}
			else if (token_count == 3)
			{
				mount_directory = token;

				if (StringUtil::stringCompareCount(device_path.c_str(), "/dev/", 5) == 0 && strstr(mount_directory.c_str(), "/") != NULL)
				{
					StatsForOneDisk stats_for_one_disk;

					num_of_disks++;

					stats_for_one_disk.disk_name = device_path;

					struct statvfs stat_buf;

					if (statvfs(mount_directory.c_str(),
					            &stat_buf) == 0) // success
					{
						GMSEC_I64 disk_size_in_bytes =
						  (GMSEC_I64)stat_buf.f_blocks*(GMSEC_I64)stat_buf.f_frsize;

						stats_for_one_disk.disk_size = static_cast<GMSEC_F32>(disk_size_in_bytes)/static_cast<GMSEC_F32>(1024.0*1024.0);

						GMSEC_F64 disk_free_percentage =
						  (GMSEC_F64)stat_buf.f_bfree/
						  (GMSEC_F64)stat_buf.f_blocks;

						GMSEC_F64 disk_util_percentage =
						  1.0 - disk_free_percentage;

						stats_for_one_disk.disk_util = static_cast<GMSEC_F32>(disk_util_percentage*100.0);

					}
					else
					{
						std::string err_str =
						  "Failed to query disk stats on ";
						err_str += device_path;
	
						throw Exception(MIST_ERROR,
								RESOURCE_INFO_DISK_ERROR,
						                err_str.c_str());
					}

					stats_this_iteration.stats.push_back(stats_for_one_disk);
				}
			}

			token = strtok(NULL, " \t");

		}
	}

	ifs.close();

	stats_this_iteration.num_of_disks = num_of_disks;

#elif defined(__hpux)

	//
	// Read from file /etc/fstab to obtain mounted disks
	// info.
	//
	// The file fields are:
	//
	// 		 1 - device path to mount
	// 		 2 - mount directory
	// 		 3 - filesystem type
	// 		 4 - options
	// 		 5 - backup frequency
	// 		 6 - pass number
	// 		 7 - comment
	//

	char           buffer[CHAR_BUF_LEN];

	ifstream       ifs("/etc/fstab", ios::in);

	GMSEC_I16      num_of_disks = 0;

	while (ifs.getline(buffer, sizeof(buffer), '\n'))
	{
		char*       token        = strtok(buffer, " \t");
		int         token_count  = 0;
		std::string device_path;
		std::string mount_directory;

		while (token != NULL)
		{
			token_count++;

			if (token_count == 1)
			{
				device_path = token;
			}
			else if (token_count == 2)
			{
				mount_directory = token;

				if (StringUtil::stringCompareCount(device_path.c_str(), "/dev/", 5) == 0 && strstr(mount_directory.c_str(), "/") != NULL)
				{
					StatsForOneDisk stats_for_one_disk;

					num_of_disks++;

					stats_for_one_disk.disk_name = device_path;

					struct statvfs stat_buf;

					if (statvfs(mount_directory.c_str(),
					            &stat_buf) == 0) // success
					{
						GMSEC_I64 disk_size_in_bytes =
						  (GMSEC_I64)stat_buf.f_blocks*(GMSEC_I64)stat_buf.f_frsize;

						stats_for_one_disk.disk_size = static_cast<GMSEC_F32>(disk_size_in_bytes)/static_cast<GMSEC_F32>(1024.0*1024.0);

						GMSEC_F64 disk_free_percentage =
						  (GMSEC_F64)stat_buf.f_bfree/
						  (GMSEC_F64)stat_buf.f_blocks;

						GMSEC_F64 disk_util_percentage =
						  1.0 - disk_free_percentage;

						stats_for_one_disk.disk_util = static_cast<GMSEC_F32>(disk_util_percentage*100.0);

					}
					else
					{
						std::string err_str =
						  "Failed to query disk stats on ";
						err_str += device_path;
	
						throw Exception(MIST_ERROR,
								RESOURCE_INFO_DISK_ERROR,
						                err_str.c_str());
					}

					stats_this_iteration.stats.push_back(stats_for_one_disk);

				}
			}

			token = strtok(NULL, " \t");

		}
	}

	ifs.close();

	stats_this_iteration.num_of_disks = num_of_disks;

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

	ifstream       ifs("/proc/mounts", ios::in);

	GMSEC_I16      num_of_disks = 0;

	while (ifs.getline(buffer, sizeof(buffer), '\n'))
	{
		char*       token        = strtok(buffer, " \t");
		int         token_count  = 0;
		std::string device_path;
		std::string mount_directory;

		while (token != NULL)
		{
			token_count++;

			if (token_count == 1)
			{
				device_path = token;
			}
			else if (token_count == 2)
			{
				mount_directory = token;

				if (StringUtil::stringCompareCount(device_path.c_str(), "/dev", 4) == 0)
				{
					StatsForOneDisk stats_for_one_disk;

					num_of_disks++;

					stats_for_one_disk.disk_name = device_path;

					struct statfs stat_buf;
	
					if (statfs(mount_directory.c_str(),
					           &stat_buf) == 0) // success
					{
						GMSEC_I64 disk_size_in_bytes =
						  stat_buf.f_blocks*stat_buf.f_bsize;
	
						stats_for_one_disk.disk_size = static_cast<GMSEC_F32>(disk_size_in_bytes)/static_cast<GMSEC_F32>(1024.0*1024.0);

						GMSEC_F64 disk_free_percentage =
						  (GMSEC_F64)stat_buf.f_bfree/
						  (GMSEC_F64)stat_buf.f_blocks;

						GMSEC_F64 disk_util_percentage =
						  1.0 - disk_free_percentage;

						stats_for_one_disk.disk_util = static_cast<GMSEC_F32>(disk_util_percentage*100.0);

					}
					else
					{
						std::string err_str =
						  "Failed to query disk stats on ";
						err_str += device_path;
	
						throw Exception(MIST_ERROR,
								RESOURCE_INFO_DISK_ERROR,
						                err_str.c_str());
					}

					stats_this_iteration.stats.push_back(stats_for_one_disk);

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

	char           buffer[CHAR_BUF_LEN];

	GMSEC_I16      num_of_disks = 0;

	FILE* pipe_output_fp = popen("/bin/df", "r");

	std::istringstream iss;

	pipeStreamToInputStream(iss, pipe_output_fp);

	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer), '\n'))
	{
		char*       token        = strtok(buffer, " \t");
		int         token_count  = 0;
		std::string device_path;
		std::string mount_directory;

		while (token != NULL)
		{
			token_count++;

			if (token_count == 1)
			{
				device_path = token;
			}
			else if (token_count == 9)
			{
				mount_directory = token;

				if (StringUtil::stringCompareCount(device_path.c_str(), "/dev/", 5) == 0 && strstr(mount_directory.c_str(), "/") != NULL)
				{
					StatsForOneDisk stats_for_one_disk;

					num_of_disks++;

					stats_for_one_disk.disk_name = device_path;

					struct statvfs stat_buf;

					if (statvfs(mount_directory.c_str(),
					            &stat_buf) == 0) // success
					{
						GMSEC_I64 disk_size_in_bytes =
						  (GMSEC_I64)stat_buf.f_blocks*(GMSEC_I64)stat_buf.f_frsize;

						stats_for_one_disk.disk_size = static_cast<GMSEC_F32>(disk_size_in_bytes)/static_cast<GMSEC_F32>(1024.0*1024.0);

						GMSEC_F64 disk_free_percentage =
						  (GMSEC_F64)stat_buf.f_bfree/
						  (GMSEC_F64)stat_buf.f_blocks;

						GMSEC_F64 disk_util_percentage =
						  1.0 - disk_free_percentage;

						stats_for_one_disk.disk_util = static_cast<GMSEC_F32>(disk_util_percentage*100.0);

					}
					else
					{
						std::string err_str =
						  "Failed to query disk stats on ";
						err_str += device_path;
	
						throw Exception(MIST_ERROR,
								RESOURCE_INFO_DISK_ERROR,
						                err_str.c_str());
					}

					stats_this_iteration.stats.push_back(stats_for_one_disk);

				}
			}

			token = strtok(NULL, " \t");

		}
	}

	stats_this_iteration.num_of_disks = num_of_disks;


#else
	throw Exception(MIST_ERROR, RESOURCE_DISK_MEMORY_ERROR,
	                "Failed to query disk stats:  unknown operating system");
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

	for (int disk_count = 0; disk_count < stats_this_iteration.num_of_disks; disk_count++)
	{
		GMSEC_F32 average_disk_size = 0.0;
		GMSEC_F32 average_disk_util = 0.0;

		std::deque<DiskStats>::iterator it = m_diskStatsQueue.begin();
		while (it != m_diskStatsQueue.end())
		{
			average_disk_size += it->stats[disk_count].disk_size;
			average_disk_util += it->stats[disk_count].disk_util;

			it++;
		}

		average_disk_size /= ((GMSEC_F32)m_diskStatsQueue.size());
		average_disk_util /= ((GMSEC_F32)m_diskStatsQueue.size());

		char field_key[CHAR_BUF_LEN];
	
		StringUtil::stringFormat(field_key, sizeof(field_key), "DISK.%d.NAME", disk_count+1);
		msg.addField(field_key, stats_this_iteration.stats[disk_count].disk_name.c_str());

		StringUtil::stringFormat(field_key, sizeof(field_key), "DISK.%d.SIZE", disk_count+1);
		if (version == GMSEC_ISD_2014_00)
		{
			msg.addField(field_key, average_disk_size);
		}
		else if (version == GMSEC_ISD_2016_00)
		{
			msg.addField(field_key, (GMSEC_U32) average_disk_size);
		}
		else
		{
			GMSEC_WARNING << "Specification version unknown: " << version << ", unable to autopopulate "
				<< field_key << " in Resource message";
		}

		StringUtil::stringFormat(field_key, sizeof(field_key), "DISK.%d.UTIL", disk_count+1);
		msg.addField(field_key, average_disk_util);
	}

	if (version == GMSEC_ISD_2014_00)
	{
		msg.addField("NUM-OF-DISKS", stats_this_iteration.num_of_disks);
	}
	else if (version == GMSEC_ISD_2016_00)
	{
		msg.addField("NUM-OF-DISKS", (GMSEC_U16) stats_this_iteration.num_of_disks);
	}
	else
	{
		GMSEC_WARNING << "Specification version unknown: " << version << ", unable to autopopulate NUM-OF-DISKS "
			<< " in Resource message";
	}
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
		throw Exception(MIST_ERROR, RESOURCE_INFO_MEMORY_ERROR,
		                "Failed to query Windows version information");
	}

#elif defined(__sun) || defined(__hpux) || defined(__linux__) || defined(__APPLE__)

	struct utsname utsname_buf;

	if (uname(&utsname_buf) >= 0) // success
	{
		std::ostringstream ss;
		ss << "OS version:  " << utsname_buf.sysname << " "
		   << utsname_buf.release << " "
		   << utsname_buf.version << " "
		   << utsname_buf.machine;

		return ss.str();
	}
	else // failure in query
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_OS_VERSION_ERROR,
		                "Failed to query OS version");
	}

#else
	return "Unknown Operating System";
#endif    
}


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
#endif


void ResourceInfoGenerator::addCPUStats(Message &msg, unsigned int version, size_t movingAverageSamples)
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

	stats_this_iteration.num_cpus = (GMSEC_I16) systemInfo.dwNumberOfProcessors;

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
							stats_this_iteration.cpu_total_util = (GMSEC_F32) totalUtilization.doubleValue;

							// Get the individual CPU utilization readings
							//
							for (unsigned int i = 0; i < systemInfo.dwNumberOfProcessors && status == ERROR_SUCCESS; ++i)
							{
								status = PdhGetFormattedCounterValue(indivCounters[i], PDH_FMT_DOUBLE, NULL, &indivUtilization[i]);

								if (checkStatus("PdhGetFormattedCounterValue Individual", status))
								{
									stats_this_iteration.cpu_utils.push_back((GMSEC_F32) indivUtilization[i].doubleValue);
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


#elif defined(__sun)

	//
	// Read from output of mpstat program to determine number of CPUs
	// and CPU utilization.
	//
	// Sample user+system ticks and total ticks twice.  Get the difference
	// between the samples for each data point.
	//
	// Then divide delta_user_and_system_ticks/delta_total_ticks.
	//

	FILE* pipe_output_fp = popen("/usr/bin/sh -c mpstat", "r");

	char               buffer[CHAR_BUF_LEN];
	GMSEC_I16          num_cpus          = 0;
	int                num_args          = 0;
	unsigned long int  dummy             = 0;
	unsigned long int  cpu               = 0;

	CPUTickStruct      cpu_tick_struct;

	std::istringstream iss;

	pipeStreamToInputStream(iss, pipe_output_fp);

	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer), '\n'))
	{
		num_args = sscanf(buffer,
		                  "%lu %lu %lu %lu "
		                  "%lu %lu %lu %lu "
		                  "%lu %lu %lu %lu "
		                  "%lu %lu %lu %lu",
		                  &cpu, &dummy, &dummy, &dummy,
		                  &dummy, &dummy, &dummy, &dummy,
		                  &dummy, &dummy, &dummy, &dummy,
		                  &cpu_tick_struct.user_mode_ticks,
		                  &cpu_tick_struct.system_mode_ticks,
		                  &dummy,
		                  &cpu_tick_struct.idle_mode_ticks);

		if (num_args == 16)
		{
			num_cpus++;

			GMSEC_F32 cpu_util = 0.0;

			if (cpu_tick_struct.user_mode_ticks +
			    cpu_tick_struct.system_mode_ticks +
			    cpu_tick_struct.idle_mode_ticks > 0)
			{
				cpu_util = (GMSEC_F32)(cpu_tick_struct.user_mode_ticks + cpu_tick_struct.system_mode_ticks)/(GMSEC_F32)(cpu_tick_struct.user_mode_ticks + cpu_tick_struct.system_mode_ticks + cpu_tick_struct.idle_mode_ticks);
				cpu_util *= 100.0;
			}

			stats_this_iteration.cpu_utils.push_back(cpu_util);
		}
	}

	stats_this_iteration.num_cpus = num_cpus;

#elif defined(__hpux)

	system("top -w -f /tmp/tmpout.top");

	char              buffer[CHAR_BUF_LEN];
	bool              done                     = false;
	int               num_args                 = 0;
	float             dummy                    = 0.0;
	float             user                     = 0.0;
	float             nice                     = 0.0;
	float             sys                      = 0.0;
	float             idle                     = 0.0;
	GMSEC_I16         num_cpus                 = 0;
	GMSEC_I16         cpu_count                = 0;
	bool              ready_to_fetch_cpu_loads = false;
	CPUTickStruct     cpu_tick_struct;

	std::ifstream ifs("/tmp/tmpout.top", ios::in);

	while (ifs.getline(buffer, sizeof(buffer), '\n'))
	{
		if (StringUtil::stringCompareCount(buffer, "Cpu states:", 11) == 0)
		{
			num_args = sscanf(buffer, "Cpu states: %hd", &num_cpus);

		}
		else if (StringUtil::stringCompareCount(buffer, " LOAD", 5) == 0)
		{
			ready_to_fetch_cpu_loads = true;
		}
		else if (ready_to_fetch_cpu_loads)
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

				index++;
			}

			num_args = sscanf(buffer, " %f %f %f %f %f",
			                  &dummy, &user, &nice, &sys, &idle);

			if (num_args == 5 && cpu_count < num_cpus)
			{
				cpu_count++;

				GMSEC_F32 cpu_util =
				  (GMSEC_F32)(user + sys);

				stats_this_iteration.cpu_utils.push_back(cpu_util);
			}
		}
	}

	unlink("/tmp/tmpout.top");

	stats_this_iteration.num_cpus = num_cpus;

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

	char                       buffer[CHAR_BUF_LEN];

	GMSEC_I16                  num_cpus = 0;

	CPUTickStruct              cpu_tick_struct;

	ifstream                   ifs("/proc/stat", ios::in);

	while (ifs.getline(buffer, sizeof(buffer), '\n'))
	{
		char* current_str = strtok(buffer, " ");

		if (current_str != NULL)
		{
			if (StringUtil::stringCompareCount(current_str, "cpu", 3) == 0 && !StringUtil::stringEquals(current_str, "cpu")) // want "cpu0" and "cpu1" etc. but not just "cpu"
			{
				num_cpus++;

				current_str = strtok(NULL, "\n");

				int num_args = sscanf(current_str, "%lu %lu %lu %lu",
				                 &cpu_tick_struct.user_mode_ticks,
				                 &cpu_tick_struct.nice_mode_ticks,
				                 &cpu_tick_struct.system_mode_ticks,
				                 &cpu_tick_struct.idle_mode_ticks);

				if (num_args == 4)
				{
					GMSEC_U32 util_ticks = cpu_tick_struct.user_mode_ticks + cpu_tick_struct.system_mode_ticks;

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

	char               buffer[CHAR_BUF_LEN];
	bool               done                     = false;
	int                num_args                 = 0;
	float              dummy                    = 0.0;
	float              user                     = 0.0;
	float              sys                      = 0.0;
	float              idle                     = 0.0;
	int                num_cpus                 = 0;
	bool               ready_to_fetch_cpu_loads = false;
	CPUTickStruct      cpu_tick_struct;

	FILE* pipe_output_fp = popen("top -l 1", "r");

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

				index++;
			}

			num_args = sscanf(buffer, "CPU usage: %f user, %f sys, %f idle", &user, &sys, &idle);

			GMSEC_F32 cpu_util = (user+sys)/(user+sys+idle);
			cpu_util *= 100.0;

			stats_this_iteration.cpu_total_util = cpu_util;

			done = true;

		}
	}

	size_t size = sizeof(num_cpus);
	if (sysctlbyname("hw.logicalcpu", &num_cpus, &size, NULL, 0) != 0)
	{
		throw Exception(MIST_ERROR, RESOURCE_INFO_CPU_ERROR,
		                "Failed to query number of cpus");
	}

	stats_this_iteration.num_cpus = (GMSEC_I16) num_cpus;

	for (size_t synth_cpu_count = 0; synth_cpu_count < num_cpus; synth_cpu_count++)
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
	throw Exception(MIST_ERROR, RESOURCE_INFO_CPU_ERROR,
	                "Failed to query cpu stats:  unknown operating system");
#endif

	//
	// This section takes a queue of cpu stats and performs
	// a moving average, based on past calls to this function where
	// the data was recorded in a queue of fixed size (where
	// one old sample is disposed of and a new sample is added,
	// per call to this function).
	//

	GMSEC_F32 average_cpu_total_util = 0.0;

	while (m_cpuStatsQueue.size() >= movingAverageSamples)
	{
		m_cpuStatsQueue.pop_front();
	}

	m_cpuStatsQueue.push_back(stats_this_iteration);

	for (int cpu_count = 0; cpu_count < stats_this_iteration.num_cpus; cpu_count++)
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
		std::deque<CPUStats>::iterator it = m_cpuStatsQueue.begin();
		while (it != m_cpuStatsQueue.end())
		{
			average_cpu_utils += it->cpu_utils[cpu_count];

			it++;
		}

		average_cpu_utils /= ((GMSEC_F32)m_cpuStatsQueue.size());

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
		  		start_util_ticks = m_cpuStatsQueue.front().util_ticks[cpu_count];
		  		start_total_ticks = m_cpuStatsQueue.front().total_ticks[cpu_count];
			}

			GMSEC_U32 end_util_ticks = m_cpuStatsQueue.back().util_ticks[cpu_count];

			GMSEC_U32 end_total_ticks = m_cpuStatsQueue.back().total_ticks[cpu_count];

			util_ticks_delta = end_util_ticks-start_util_ticks;
			total_ticks_delta = end_total_ticks-start_total_ticks;

			if (total_ticks_delta > 0)
			{
				average_cpu_utils = (GMSEC_F32)util_ticks_delta/(GMSEC_F32)total_ticks_delta;
				average_cpu_utils *= 100.0;
			}
		}

#endif

		if (stats_this_iteration.cpu_total_util == 0.0) // nothing populated by system
		{
			average_cpu_total_util += average_cpu_utils;
		}

		char field_key[CHAR_BUF_LEN];

		StringUtil::stringFormat(field_key, sizeof(field_key), "CPU.%d.UTIL", cpu_count+1);
		msg.addField(field_key, average_cpu_utils);

	}// for cpu_count

	if (stats_this_iteration.cpu_total_util == 0.0) // nothing populated by system
	{
		average_cpu_total_util /= static_cast<GMSEC_F32>(stats_this_iteration.num_cpus);
	}
	else
	{
		average_cpu_total_util = 0.0;

		std::deque<CPUStats>::iterator it = m_cpuStatsQueue.begin();
		while (it != m_cpuStatsQueue.end())
		{
			average_cpu_total_util += it->cpu_total_util;

			it++;
		}

		average_cpu_total_util /= static_cast<GMSEC_F32>(m_cpuStatsQueue.size());		
	}

	msg.addField("CPU.TOTAL.UTIL", average_cpu_total_util);
	
	if(version == GMSEC_ISD_2014_00){
		msg.addField("NUM-OF-CPUS", stats_this_iteration.num_cpus);
	}else if(version == GMSEC_ISD_2016_00){
		msg.addField("NUM-OF-CPUS", (GMSEC_U16) stats_this_iteration.num_cpus);
	}else{
		GMSEC_WARNING << "Specification version unknown: " << version << ", unable to autopopulate NUM-OF-CPUS "
			<< " in Resource message";
	}
}


void ResourceInfoGenerator::addNetworkStats(Message &msg, unsigned int version, size_t movingAverageSamples)
{
	//This function is responsible for:
	//NUM-OF-NET-PORTS, I16
	//NET-PORT.n.NAME, String
	//NET-PORT.n.EUI-ADR, String
	//NET-PORT.n.IP-ADR, String
	//NET-PORT.n.TOTAL-BANDWIDTH, F32
	//NET-PORT.n.UTIL, F32
	//NET-PORT.n.BYTES-SENT, F32
	//NET-PORT.n.BYTES-RECEIVED, F32
	//NET-PORT.n.MSGS-SENT, F32
	//NET-PORT.n.MSGS-RECEIVED, F32
	//NET-PORT.n.ERRORS, F32
#ifdef WIN32
	IP_ADAPTER_INFO  *pAdapterInfo;
	ULONG            ulOutBufLen;
	DWORD            dwRetVal;

	pAdapterInfo = (IP_ADAPTER_INFO*) malloc(sizeof(IP_ADAPTER_INFO));
	ulOutBufLen  = sizeof(IP_ADAPTER_INFO);

	if (!pAdapterInfo)
	{
		GMSEC_WARNING << "Error allocating memory needed for GetAdaptersInfo";
		return;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) != ERROR_SUCCESS)
	{
		free(pAdapterInfo);

		pAdapterInfo = (IP_ADAPTER_INFO*) malloc(ulOutBufLen);

		if (!pAdapterInfo)
		{
			GMSEC_WARNING << "Error allocating memory following call to GetAdaptersInfo";
			return;
		}
	}

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) != ERROR_SUCCESS)
	{
		free(pAdapterInfo);

		GMSEC_WARNING << "GetAdaptersInfo call failed with error: " << dwRetVal;
		return;
	}

	PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
	size_t idx = 0;

	while (pAdapter)
	{
		std::ostringstream ss;
		ss << "NET-PORT." << idx+1;
		std::string pName = ss.str();
		pName.append(".NAME");
		msg.addField(pName.c_str(), pAdapter->AdapterName);

		std::string pDesc = ss.str();
		pDesc.append(".DESC");
		msg.addField(pDesc.c_str(), pAdapter->Description);
		
		std::ostringstream macAddr;
		for (UINT i = 0; i < pAdapter->AddressLength; i++)
		{
			TCHAR szCounterName[CHAR_BUF_LEN];
			if (i == (pAdapter->AddressLength - 1))
			{
				sprintf_s(szCounterName, sizeof(szCounterName) - 1, "%.2X", (int)pAdapter->Address[i]);
			}
			else
			{
				sprintf_s(szCounterName, sizeof(szCounterName) - 1, "%.2X-", (int)pAdapter->Address[i]);
			}
			macAddr << szCounterName;
		}

		std::string pUiAdr = ss.str();
		pUiAdr.append(".EUI-ADR");
		msg.addField(pUiAdr.c_str(), macAddr.str().c_str());

		std::string pIp = ss.str();
		pIp.append(".IP-ADR");
		msg.addField(pIp.c_str(), pAdapter->IpAddressList.IpAddress.String);

		//Windows TODO:
		//NET-PORT.n.TOTAL-BANDWIDTH, F32
		//NET-PORT.n.UTIL, F32
		//NET-PORT.n.BYTES-SENT, F32
		//NET-PORT.n.BYTES-RECEIVED, F32
		//NET-PORT.n.MSGS-SENT, F32
		//NET-PORT.n.MSGS-RECEIVED, F32
		//NET-PORT.n.ERRORS, F32

		pAdapter = pAdapter->Next;

		++idx;
	}

	if (version == GMSEC_ISD_2014_00)
	{
		msg.addField("NUM-OF-NET-PORTS", (GMSEC_I16) idx);
	}
	else if (version == GMSEC_ISD_2016_00)
	{
		msg.addField("NUM-OF-NET-PORTS", (GMSEC_U16) idx);
	}
	else
	{
		GMSEC_WARNING << "Specification version unknown: " << version
		              << ", unable to autopopulate NUM-OF-NET_PORTS in Resource message";
	}
	
	free(pAdapterInfo);

#elif defined(__sun)

	GMSEC_I16                num_net_ports = 0;
	int                      num_args = 0;
	char                     buffer[CHAR_BUF_LEN];
	char                     interface_name[CHAR_BUF_LEN];
	char                     dummy[CHAR_BUF_LEN];
	char                     ip_address[CHAR_BUF_LEN];
	char                     mac_address[CHAR_BUF_LEN];
	std::vector<std::string> ip_addresses;

	FILE* pipe_output_fp = popen("/usr/bin/sh -c \"netstat -in\"", "r");

	std::istringstream       iss;

	pipeStreamToInputStream(iss, pipe_output_fp);

	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer), '\n'))
	{
		num_args = sscanf(buffer, "%s %s %s %s",
		                  interface_name,
		                  dummy,
		                  dummy,
		                  ip_address);

		if (num_args == 4)
		{
			if (strstr(ip_address, ".") != NULL)
			{
				num_net_ports++;

				ip_addresses.push_back(ip_address);

				char tmp_string[CHAR_BUF_LEN];

				//
				// Add NET-PORT.n.NAME, String
				//
				StringUtil::stringFormat(tmp_string,
				                         sizeof(tmp_string),
				                         "NET-PORT.%u.NAME",
				                         num_net_ports);
				msg.addField(tmp_string, interface_name);

				//
				// Add NET-PORT.n.IP-ADR, String
				//
				StringUtil::stringFormat(tmp_string,
				                         sizeof(tmp_string),
				                         "NET-PORT.%u.IP-ADR",
				                         num_net_ports);
				msg.addField(tmp_string, ip_address);

			}
		}
	}

	iss.clear();

	pipe_output_fp = popen("/usr/bin/sh -c \"netstat -pn\"", "r");

	pipeStreamToInputStream(iss, pipe_output_fp);

	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer), '\n'))
	{
		num_args = sscanf(buffer, "%s %s %s %s %s",
		                  dummy,
		                  ip_address,
		                  dummy,
		                  dummy,
		                  mac_address);

		if (num_args == 5)
		{
			if (strstr(ip_address, ".") != NULL)
			{
				if (std::find(ip_addresses.begin(),
				              ip_addresses.end(),
				              ip_address) !=
				    ip_addresses.end())
				{
					char tmp_string[CHAR_BUF_LEN];

					//
					// Add NET-PORT.n.EUI-ADR,String
					//
					StringUtil::stringFormat(tmp_string,
					                         sizeof(tmp_string),
					                         "NET-PORT.%u.EUI-ADR",
					                         num_net_ports);
					msg.addField(tmp_string, mac_address);
				}
			}
		}
	}

	
	if(version == GMSEC_ISD_2014_00){
		msg.addField("NUM-OF-NET-PORTS", num_net_ports);
	}else if(version == GMSEC_ISD_2016_00){
		msg.addField("NUM-OF-NET-PORTS", (GMSEC_U16) num_net_ports);
	}else{
		GMSEC_WARNING << "Specification version unknown: " << version << ", unable to autopopulate NUM-OF-NET-PORTS "
			<< " in Resource message";
	}

#elif defined(__hpux)

	GMSEC_I16                num_net_ports = 0;
	int                      num_args = 0;
	char                     buffer[CHAR_BUF_LEN];
	char                     interface_name[CHAR_BUF_LEN];
	char                     dummy[CHAR_BUF_LEN];
	char                     ip_address[CHAR_BUF_LEN];
	GMSEC_I16                interface_num = 0;
	char                     mac_address_str[CHAR_BUF_LEN];

	std::vector<std::string> interface_names;

	FILE* pipe_output_fp = popen("netstat -in", "r");

	std::istringstream       iss;

	pipeStreamToInputStream(iss, pipe_output_fp);

	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer), '\n'))
	{
		num_args = sscanf(buffer, "%s %s %s %s",
		                  interface_name,
		                  dummy,
		                  dummy,
		                  ip_address);

		if (num_args == 4)
		{
			if (strstr(ip_address, ".") != NULL)
			{
				num_net_ports++;

				char tmp_string[CHAR_BUF_LEN];

				//
				// Add NET-PORT.n.NAME, String
				//
				StringUtil::stringFormat(tmp_string,
				                         sizeof(tmp_string),
				                         "NET-PORT.%u.NAME",
				                         num_net_ports);
				msg.addField(tmp_string, interface_name);

				interface_names.push_back(interface_name);

				//
				// Add NET-PORT.n.IP-ADR, String
				//
				StringUtil::stringFormat(tmp_string,
				                         sizeof(tmp_string),
				                         "NET-PORT.%u.IP-ADR",
				                         num_net_ports);
				msg.addField(tmp_string, ip_address);

			}
		}
	}

	iss.clear();

	pipe_output_fp = popen("/usr/sbin/lanscan", "r");

	pipeStreamToInputStream(iss, pipe_output_fp);

	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer), '\n'))
	{
		num_args = sscanf(buffer, "%s %s %s %s %s",
		                  dummy,
		                  mac_address_str,
		                  dummy,
		                  dummy,
		                  interface_name);

		if (num_args == 5)
		{
			char tmp_string[CHAR_BUF_LEN];

			//
			// Search for interface num in interface_names
			// vector; just use a linear search because
			// the list is not expected to be long
			//
			std::vector<std::string>::iterator it =
			  interface_names.begin();

			interface_num = 0;

			bool found = false;

			while (it != interface_names.end() &&
			       !found)
			{
				if (*it == interface_name)
				{
					found = true;
				}
				else
				{
					it++;
					interface_num++;
				}
			}

			if (found)
			{
				//
				// Add NET-PORT.n.EUI-ADR,String
				//
				StringUtil::stringFormat(tmp_string,
				                         sizeof(tmp_string),
				                         "NET-PORT.%hd.EUI-ADR",
				                         interface_num+1);

				msg.addField(tmp_string, normalizeMacAddressStr(mac_address_str));
			}
		}
	}

	if(version == GMSEC_ISD_2014_00){
		msg.addField("NUM-OF-NET-PORTS", num_net_ports);
	}else if(version == GMSEC_ISD_2016_00){
		msg.addField("NUM-OF-NET-PORTS", (GMSEC_U16) num_net_ports);
	}else{
		GMSEC_WARNING << "Specification version unknown: " << version << ", unable to autopopulate NUM-OF-NET-PORTS "
			<< " in Resource message";
	}

#elif defined(__linux__)

	GMSEC_I16 num_net_ports = 0;

	struct ifaddrs* ifap    = NULL;

	if (getifaddrs(&ifap) == 0) // success
	{
		struct ifaddrs* ifap_head = ifap;

		while (ifap != NULL)
		{
			if (((struct sockaddr_in* ) ifap->ifa_addr)->sin_family == AF_INET)
			{
				num_net_ports++;

				char tmp_string[CHAR_BUF_LEN];

				//
				// Add NET-PORT.n.NAME, String
				//
				StringUtil::stringFormat(tmp_string, sizeof(tmp_string), "NET-PORT.%u.NAME", num_net_ports);
				msg.addField(tmp_string, ifap->ifa_name);

				//
				// Add NET-PORT.n.IP-ADR, String
				//
				StringUtil::stringFormat(tmp_string, sizeof(tmp_string), "NET-PORT.%u.IP-ADR", num_net_ports);
				msg.addField(tmp_string, inet_ntoa(((struct sockaddr_in* ) ifap->ifa_addr)->sin_addr)); // C-string copy made by addField()

				//
				// Add NET-PORT.n.EUI-ADR, String
				//
				StringUtil::stringFormat(tmp_string, sizeof(tmp_string), "/sys/class/net/%s/address", ifap->ifa_name);

				ifstream ifs(tmp_string, ios::in);
				if (ifs.is_open())
				{
					char buffer[CHAR_BUF_LEN];

					ifs.getline(buffer, sizeof(buffer), '\n');

					StringUtil::stringFormat(tmp_string, sizeof(tmp_string), "NET-PORT.%u.EUI-ADR", num_net_ports);

					msg.addField(tmp_string, buffer);
				}
				else
				{
					throw Exception(MIST_ERROR, RESOURCE_INFO_NET_ERROR,
					                "Failed to query network stats:  failed to get MAC address.");
				}
				ifs.close();
			}

			//
			// Add NET-PORT.n.TOTAL-BANDWIDTH, F32
			//NET-PORT.n.UTIL, F32
			//NET-PORT.n.BYTES-SENT, F32
			//NET-PORT.n.BYTES-RECEIVED, F32
			//NET-PORT.n.MSGS-SENT, F32
			//NET-PORT.n.MSGS-RECEIVED, F32
			//NET-PORT.n.ERRORS, F32
			//

			ifap = ifap->ifa_next;
		}

		if (ifap_head != NULL)
		{
			freeifaddrs(ifap_head); // Please read man page for getifaddrs about dynamic allocation of ifap and the need to (ideally) free it after use
		}

		if (version == GMSEC_ISD_2014_00)
		{
			msg.addField("NUM-OF-NET-PORTS", num_net_ports);
		}
		else if (version == GMSEC_ISD_2016_00)
		{
			msg.addField("NUM-OF-NET-PORTS", (GMSEC_U16) num_net_ports);
		}
		else
		{
			GMSEC_WARNING << "Specification version unknown: " << version << ", unable to autopopulate NUM-OF-NET-PORTS "
				<< " in Resource message";
		}
		
	}
	else // failure on call to getifaddrs()
	{
		throw Exception(MIST_ERROR,
		                RESOURCE_INFO_NET_ERROR,
		                "Failed to query network stats:  failed call to getifaddrs()");
	}

#elif defined(__APPLE__)

	GMSEC_I16                num_net_ports = 0;
	int                      num_args = 0;
	char                     buffer[CHAR_BUF_LEN];
	char                     interface_name[CHAR_BUF_LEN];
	char                     dummy[CHAR_BUF_LEN];
	char                     address[CHAR_BUF_LEN];
	GMSEC_I16                interface_num = 0;

	std::vector<std::string> interface_names;

	FILE* pipe_output_fp = popen("/usr/sbin/netstat -in", "r");

	std::istringstream       iss;

	pipeStreamToInputStream(iss, pipe_output_fp);

	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer), '\n'))
	{
		num_args = sscanf(buffer, "%s %s %s %s",
		                  interface_name,
		                  dummy,
		                  dummy,
		                  address);

		if (num_args == 4)
		{
			if (strstr(address, ".") != NULL)
			{
				num_net_ports++;

				char tmp_string[CHAR_BUF_LEN];

				//
				// Add NET-PORT.n.NAME, String
				//
				StringUtil::stringFormat(tmp_string,
				                         sizeof(tmp_string),
				                         "NET-PORT.%u.NAME",
				                         num_net_ports);
				msg.addField(tmp_string, interface_name);

				interface_names.push_back(interface_name);

				//
				// Add NET-PORT.n.IP-ADR, String
				//
				StringUtil::stringFormat(tmp_string,
				                         sizeof(tmp_string),
				                         "NET-PORT.%u.IP-ADR",
				                         num_net_ports);
				msg.addField(tmp_string, address);

			}
		}
	}

	iss.clear();

	pipe_output_fp = popen("/usr/sbin/netstat -in", "r");

	pipeStreamToInputStream(iss, pipe_output_fp);

	pclose(pipe_output_fp);

	while (iss.getline(buffer, sizeof(buffer), '\n'))
	{
		num_args = sscanf(buffer, "%s %s %s %s",
		                  interface_name,
		                  dummy,
		                  dummy,
		                  address);

		if (num_args == 4)
		{
			char tmp_string[CHAR_BUF_LEN];

			//
			// Search for interface num in interface_names
			// vector; just use a linear search because
			// the list is not expected to be long
			//
			std::vector<std::string>::iterator it =
			  interface_names.begin();

			interface_num = 0;

			bool found = false;

			while (it != interface_names.end() &&
			       !found)
			{
				if (*it == interface_name)
				{
					found = true;
				}
				else
				{
					it++;
					interface_num++;
				}
			}

			if (found)
			{
				if (address[2] == ':' &&
				    address[5] == ':' &&
				    address[8] == ':' &&
				    address[11] == ':' &&
				    address[14] == ':') // if this looks like a MAC address
				{
					//
					// Add NET-PORT.n.EUI-ADR,String
					//
					StringUtil::stringFormat(tmp_string,
					                         sizeof(tmp_string),
					                         "NET-PORT.%hd.EUI-ADR",
					                         interface_num+1);

					msg.addField(tmp_string, address);
				}
			}
		}
	}

	if(version == GMSEC_ISD_2014_00){
		msg.addField("NUM-OF-NET-PORTS", num_net_ports);
	}else if(version == GMSEC_ISD_2016_00){
		msg.addField("NUM-OF-NET-PORTS", (GMSEC_U16) num_net_ports);
	}else{
		GMSEC_WARNING << "Specification version unknown: " << version << ", unable to autopopulate NUM-OF-NET-PORTS "
			<< " in Resource message";
	}

#else
	throw Exception(MIST_ERROR,
	                RESOURCE_INFO_NET_ERROR,
	                "Failed to query network stats:  unknown operating system");
#endif
}

void ResourceInfoGenerator::pipeStreamToInputStream(std::istringstream& iss,
                                                    FILE*               pStream)
{
	ostringstream oss;
	char          buffer[CHAR_BUF_LEN];
	bool          done = false;
	size_t        tmp_num_chars = 0;

	while (!done)
	{
		if ((tmp_num_chars = fread(buffer, 1, CHAR_BUF_LEN-1, pStream)) > 0)
		{
			buffer[tmp_num_chars] = '\0';		
			oss << buffer;
		}
		else
		{
			done = true;
		}
	}

	iss.str(oss.str());

}

#ifdef __hpux

const char* ResourceInfoGenerator::normalizeMacAddressStr(const char* macAddrStr)
{
	char                  buffer[CHAR_BUF_LEN];
	vector<unsigned char> mac_addr_elements;
	unsigned long long    mac_addr                  = 0;
	unsigned char         mac_addr_element          = 0;
	unsigned int          num_mac_addr_elements     = 0; 
	static char           ret_buffer[CHAR_BUF_LEN];

	sscanf(macAddrStr, "%llX", &mac_addr);

	while (mac_addr > 0)
	{
		num_mac_addr_elements++;
		mac_addr_element = mac_addr%16;
		mac_addr_elements.push_back(mac_addr_element);
		mac_addr /= 16;
	}

	while (num_mac_addr_elements < 12) // MAC addresses contain 12 hex digits
	{
		num_mac_addr_elements++;
		mac_addr_elements.push_back(0);
	}

	if (mac_addr_elements.size() == 12)
	{
		StringUtil::stringFormat(ret_buffer, sizeof(ret_buffer),
		                          "%X%X:%X%X:%X%X:%X%X:%X%X:%X%X",
		                          mac_addr_elements[11],
		                          mac_addr_elements[10],
		                          mac_addr_elements[9],
		                          mac_addr_elements[8],
		                          mac_addr_elements[7],
		                          mac_addr_elements[6],
		                          mac_addr_elements[5],
		                          mac_addr_elements[4],
		                          mac_addr_elements[3],
		                          mac_addr_elements[2],
		                          mac_addr_elements[1],
		                          mac_addr_elements[0]);
	}
	else
	{
		throw Exception(MIST_ERROR,
		                RESOURCE_INFO_NET_ERROR,
				"Hardware address found was invalid");
	}

	return ret_buffer;

}

#endif
