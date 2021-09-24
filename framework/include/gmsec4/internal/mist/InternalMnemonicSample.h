/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalMnemonicSample.h
 *
 *  @brief This file contains definitions for Mnemonic samples
**/

#ifndef GMSEC_API_INTERNAL_MNEMONICSAMPLE_H
#define GMSEC_API_INTERNAL_MNEMONICSAMPLE_H

#include <gmsec4/mist/MnemonicSample.h>
#include <gmsec4/util/wdllexp.h>
#include <string>

namespace gmsec
{
namespace api
{
namespace mist
{
namespace internal
{

class GMSEC_API InternalMnemonicSample
{

  public:

	InternalMnemonicSample(const char* timestamp, const Field& rawValue);

	InternalMnemonicSample(const InternalMnemonicSample& internalMnemonicSample);

	~InternalMnemonicSample();

	const char* CALL_TYPE getTimestamp() const;

	const Field& CALL_TYPE getRawValue() const;

	bool CALL_TYPE hasEUValue() const;

	const Field& CALL_TYPE getEUValue() const;

	void CALL_TYPE setEUValue(const Field& value);

	bool CALL_TYPE hasTextValue() const;

	const char* CALL_TYPE getTextValue() const;

	void CALL_TYPE setTextValue(const char* value);

	bool CALL_TYPE hasFlags() const;

	GMSEC_I32 CALL_TYPE getFlags() const;

	void CALL_TYPE setFlags(GMSEC_I32 flags);

	bool CALL_TYPE hasLimitEnableDisable() const;

	bool CALL_TYPE getLimitEnableDisable() const;

	void CALL_TYPE setLimitEnableDisable(bool value);

	bool CALL_TYPE hasLimit() const;

	MnemonicSample::LimitFlag CALL_TYPE getLimit() const;

	void CALL_TYPE setLimit(MnemonicSample::LimitFlag limit);

	bool CALL_TYPE hasStalenessStatus() const;

	bool CALL_TYPE getStalenessStatus() const;

	void CALL_TYPE setStalenessStatus(bool staleness);

	bool CALL_TYPE hasQuality() const;

	bool CALL_TYPE getQuality() const; 

	void CALL_TYPE setQuality(bool quality);

	bool CALL_TYPE operator==(const InternalMnemonicSample& mnemonicSample) const;

	bool CALL_TYPE operator!=(const InternalMnemonicSample& mnemonicSample) const;

  private:

	InternalMnemonicSample(); // not implemented

	std::string               m_timestamp;

	Field*                    m_rawValue;

	bool                      m_hasEUValue;
	Field*                    m_euValue;

	bool                      m_hasTextValue;
	std::string               m_textValue;

	bool                      m_hasFlags;
	GMSEC_I32                 m_flags;

	bool                      m_hasLimitEnableDisable;
	bool                      m_limitEnableDisable;

	bool                      m_hasLimit;
	MnemonicSample::LimitFlag m_limit;

	bool                      m_hasStalenessStatus;
	bool                      m_stalenessStatus;

	bool                      m_hasQuality;
	bool                      m_quality;

};

}
}
}
}

#endif
