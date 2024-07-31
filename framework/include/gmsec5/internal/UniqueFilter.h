/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_UNIQUE_FILTER_H
#define GMSEC_API5_INTERNAL_UNIQUE_FILTER_H

#include <gmsec5/util/wdllexp.h>

#include <string>
#include <map>

namespace gmsec
{
namespace api5
{
namespace internal
{

class GMSEC_API UniqueFilter
{
public:
	UniqueFilter();

	~UniqueFilter();

	/**
	* @method update(const char* id)
	* Checks for duplicate IDs.  The assumption is that the input id's are
	* of the form <prefix>_<counter>.  <prefix> is free-form; <counter> is
	* an integer.  Each <prefix> has its own <counter> which is increasing.
	* @param the id to test.
	* @return true if the id is novel
	*/
	bool update(const char* id);

private:
	// Declared, but not implemented.
	UniqueFilter(const UniqueFilter&);
	UniqueFilter& operator=(const UniqueFilter&);

	char separator;
	std::map<std::string, int> state;
};

} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
