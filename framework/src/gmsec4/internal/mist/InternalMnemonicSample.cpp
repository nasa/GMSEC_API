/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalMnemonicSample.cpp
 *
 *  @brief This file contains definitions for Mnemonic samples
**/

#include <gmsec4/internal/mist/InternalMnemonicSample.h>
#include <gmsec4/mist/MnemonicSample.h>
#include <gmsec4/Exception.h>
#include <gmsec4/Errors.h>
#include <gmsec4/internal/field/InternalField.h>
#include <string>

using namespace gmsec::api;
using namespace gmsec::api::internal;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalMnemonicSample::InternalMnemonicSample(const char*  timestamp,
                                               const Field& rawValue)
                      : m_rawValue(InternalField::makeFieldCopy(rawValue)),
                        m_hasEUValue(false),
                        m_euValue(NULL),
                        m_hasTextValue(false),
                        m_textValue(""),
                        m_hasFlags(false),
                        m_flags(0),
                        m_hasLimitEnableDisable(false),
                        m_limitEnableDisable(false),
                        m_hasLimit(false),
                        m_limit(MnemonicSample::UNSPECIFIED),
                        m_hasStalenessStatus(false),
                        m_stalenessStatus(false),
                        m_hasQuality(false),
                        m_quality(false)
{
	if (timestamp && !std::string(timestamp).empty())
	{
		m_timestamp = timestamp;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                OTHER_ERROR_CODE,
		                "InternalMnemonic constructor:  NULL or empty "
		                "timestamp provided.");
	}

}// InternalMnemonicSample constructor


InternalMnemonicSample::InternalMnemonicSample(const InternalMnemonicSample& other)
                      :
                        m_timestamp(other.m_timestamp),
                        m_rawValue(NULL),
                        m_hasEUValue(other.m_hasEUValue),
                        m_euValue(NULL),
                        m_hasTextValue(other.m_hasTextValue),
                        m_textValue(other.m_textValue),
                        m_hasFlags(other.m_hasFlags),
                        m_flags(other.m_flags),
                        m_hasLimitEnableDisable(other.m_hasLimitEnableDisable),
                        m_limitEnableDisable(other.m_limitEnableDisable),
                        m_hasLimit(other.m_hasLimit),
                        m_limit(other.m_limit),
                        m_hasStalenessStatus(other.m_hasStalenessStatus),
                        m_stalenessStatus(other.m_stalenessStatus),
                        m_hasQuality(other.m_hasQuality),
                        m_quality(other.m_quality)
{
	if(m_hasEUValue){
		setEUValue(*other.m_euValue);
	}else{
		m_euValue = NULL;
	}

	m_rawValue = InternalField::makeFieldCopy(*other.m_rawValue);
}// InternalMnemonicSample copy constructor


InternalMnemonicSample::~InternalMnemonicSample()
{
	if (m_rawValue)
	{
		delete m_rawValue;
	}

	if (m_euValue)
	{
		delete m_euValue;
	}

}// ~InternalMnemonicSample()


const char* InternalMnemonicSample::getTimestamp() const
{
	return m_timestamp.c_str();

}// getTimeStamp()


const Field& InternalMnemonicSample::getRawValue() const
{
	if (m_rawValue)
	{
		return *m_rawValue;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                INVALID_FIELD,
		                "InternalMnemonicSample::getRawValue:  raw value field does not exist in this MnemonicSample right now.");
	}

}// getRawValue()


bool InternalMnemonicSample::hasEUValue() const
{
	return m_hasEUValue;

}// hasEUValue()


const Field& InternalMnemonicSample::getEUValue() const
{
	if (m_euValue)
	{
		return *m_euValue;
	}
	else
	{
		throw Exception(MIST_ERROR,
		                INVALID_FIELD,
		                "InternalMnemonicSample::getEUValue:  engineering units value field does not exist in this MnemonicSample right now.");
	}

}// getEUValue()


void InternalMnemonicSample::setEUValue(const Field& value)
{
	if (m_euValue)
	{
		delete m_euValue;
	}

	m_euValue    = InternalField::makeFieldCopy(value);
	m_hasEUValue = true;

}// setEUValue()


bool InternalMnemonicSample::hasTextValue() const
{
	return m_hasTextValue;

}// hasTextValue()


const char* InternalMnemonicSample::getTextValue() const
{
	if (m_hasTextValue)
	{
		return m_textValue.c_str();
	}
	else
	{
		throw Exception(MIST_ERROR,
		                INVALID_FIELD,
		                "InternalMnemonicSample::getTextValue:  text value field does not exist in this MnemonicSample right now.");
	}

}// getTextValue()


void InternalMnemonicSample::setTextValue(const char* value)
{
	m_textValue = value;
	m_hasTextValue = true;

}// setTextValue()


bool InternalMnemonicSample::hasFlags() const
{
	return m_hasFlags;

}// hasFlags()


GMSEC_I32 InternalMnemonicSample::getFlags() const
{
	return m_flags;

}// getFlags()


void InternalMnemonicSample::setFlags(GMSEC_I32 flags)
{
	m_flags    = flags;
	m_hasFlags = true;

}// setFlags()


bool InternalMnemonicSample::hasLimitEnableDisable() const
{
	return m_hasLimitEnableDisable;

}// hasLimitEnableDisable()


bool InternalMnemonicSample::getLimitEnableDisable() const
{
	return m_limitEnableDisable;

}// getLimitEnableDisable()


void InternalMnemonicSample::setLimitEnableDisable(bool value)
{
	m_limitEnableDisable    = value;
	m_hasLimitEnableDisable = true;

	if(!m_limitEnableDisable)
	{
		m_limit    = MnemonicSample::UNSPECIFIED;
		m_hasLimit = false;
	}

}// setLimitEnableDisable()


bool InternalMnemonicSample::hasLimit() const
{
	return m_hasLimit;

}// hasLimit()


MnemonicSample::LimitFlag InternalMnemonicSample::getLimit() const
{
	return m_limit;

}// getLimit()


void InternalMnemonicSample::setLimit(MnemonicSample::LimitFlag value)
{
	m_limit    = value;
	m_hasLimit = true;

	//If you set a limit, implicitly enable the limit enable/disable to true
	m_limitEnableDisable    = true;
	m_hasLimitEnableDisable = true;

}// setLimit()


bool InternalMnemonicSample::hasStalenessStatus() const
{
	return m_hasStalenessStatus;

}// hasStalenessStatus()


bool InternalMnemonicSample::getStalenessStatus() const
{
	return m_stalenessStatus;

}// getStalenessStatus()


void InternalMnemonicSample::setStalenessStatus(bool staleness)
{
	m_stalenessStatus    = staleness;
	m_hasStalenessStatus = true;

}// setStalenessStatus()


bool InternalMnemonicSample::hasQuality() const
{
	return m_hasQuality;

}// hasQuality()


bool InternalMnemonicSample::getQuality() const
{
	return m_quality;

}// getQuality()


void InternalMnemonicSample::setQuality(bool quality)
{
	m_quality     = quality;
	m_hasQuality  = true;

}// setQuality()


bool InternalMnemonicSample::operator==(const InternalMnemonicSample& other) const
{
	bool ret_val = false;

	if (m_timestamp             == other.m_timestamp &&
	    InternalField::testEquals(*m_rawValue, *(other.m_rawValue)) &&
	    m_hasEUValue            == other.m_hasEUValue &&
	    m_hasTextValue          == other.m_hasTextValue &&
	    m_textValue             == other.m_textValue &&
	    m_hasFlags              == other.m_hasFlags &&
	    m_flags                 == other.m_flags &&
	    m_hasLimitEnableDisable == other.m_hasLimitEnableDisable &&
	    m_limitEnableDisable    == other.m_limitEnableDisable &&
	    m_hasLimit              == other.m_hasLimit &&
	    m_limit                 == other.m_limit &&
	    m_hasStalenessStatus    == other.m_hasStalenessStatus &&
	    m_stalenessStatus       == other.m_stalenessStatus &&
	    m_hasQuality            == other.m_hasQuality &&
	    m_quality               == other.m_quality
	   )
	{
		if (m_hasEUValue)
		{
			if (InternalField::testEquals(*m_euValue, *(other.m_euValue)))
			{
				ret_val = true;
			}
		}
		else
		{
			ret_val = true;
		}
	}

	return ret_val;

}// operator==()


bool InternalMnemonicSample::operator!=(const InternalMnemonicSample& mnemonicSample) const
{
	return !operator==(mnemonicSample);

}// operator!=()
