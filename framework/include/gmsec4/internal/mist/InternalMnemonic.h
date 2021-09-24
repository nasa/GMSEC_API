/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalMnemonic.h
 *
 *  @brief This file contains definitions for Mnemonic containers
**/

#ifndef GMSEC_API_INTERNAL_MNEMONIC_H
#define GMSEC_API_INTERNAL_MNEMONIC_H

#include <gmsec4/mist/MnemonicSample.h>
#include <gmsec4/util/DataList.h>
#include <gmsec4/util/wdllexp.h>
#include <string>
#include <vector>

namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

class GMSEC_API InternalMnemonic
{
public:
	InternalMnemonic(const char* name, const gmsec::api::util::DataList<MnemonicSample*>& samples);

	InternalMnemonic(const InternalMnemonic& other);

	InternalMnemonic& operator=(const InternalMnemonic& other);

	~InternalMnemonic();

	const char* CALL_TYPE getName() const;

	bool CALL_TYPE statusAvailable() const;

	const Field& CALL_TYPE getStatus() const;

	void CALL_TYPE setStatus(const Field& status);

	bool CALL_TYPE unitsAvailable() const;

	const char* CALL_TYPE getUnits() const;

	void CALL_TYPE setUnits(const char* units);

	size_t CALL_TYPE getSampleCount() const;

	const MnemonicSample& CALL_TYPE getSample(size_t idx) const;

	void CALL_TYPE addSample(const MnemonicSample& sample);


private:
	std::string  m_name;
	bool         m_hasStatus;
	Field*       m_status;
	bool         m_hasUnits;
	std::string  m_units;

	gmsec::api::util::DataList<MnemonicSample*>  m_sampleList;
};

}
}
}
}

#endif
