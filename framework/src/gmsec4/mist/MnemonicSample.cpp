/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file MnemonicSample.cpp
 *
 *  @brief This file contains definitions for Mnemonic samples
**/

#include <gmsec4/mist/MnemonicSample.h>
#include <gmsec4/internal/mist/InternalMnemonicSample.h>

using namespace gmsec::api;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


MnemonicSample::MnemonicSample(const char* timestamp, const Field& rawValue)
              : m_iMnemonicSample(new InternalMnemonicSample(timestamp, rawValue))
{
}// MnemonicSample constructor


MnemonicSample::MnemonicSample(const MnemonicSample& other)
              : m_iMnemonicSample(new InternalMnemonicSample(*(other.m_iMnemonicSample)))
{
}// MnemonicSample constructor

MnemonicSample::~MnemonicSample()
{
	delete m_iMnemonicSample;
}// MnemonicSample destructor


const char* MnemonicSample:: getTimestamp() const
{
	return m_iMnemonicSample->getTimestamp();

}// getTimestamp()


const Field& MnemonicSample::getRawValue() const
{
	return m_iMnemonicSample->getRawValue();

}// getRawValue()


bool MnemonicSample::hasEUValue() const
{
	return m_iMnemonicSample->hasEUValue();

}// hasEUValue()


const Field& MnemonicSample::getEUValue() const
{
	return m_iMnemonicSample->getEUValue();

}// getEUValue()


void MnemonicSample::setEUValue(const Field& value)
{
	m_iMnemonicSample->setEUValue(value);

}// setEUValue()


bool MnemonicSample::hasTextValue() const
{
	return m_iMnemonicSample->hasTextValue();

}// hasTextValue()


const char* MnemonicSample::getTextValue() const
{
	return m_iMnemonicSample->getTextValue();

}// getTextValue()


void MnemonicSample::setTextValue(const char* value)
{
	m_iMnemonicSample->setTextValue(value);

}// setTextValue()


bool MnemonicSample::hasFlags() const
{
	return m_iMnemonicSample->hasFlags();

} // hasFlags()


GMSEC_I32 MnemonicSample::getFlags() const
{
	return m_iMnemonicSample->getFlags();

}// getFlags()


void MnemonicSample::setFlags(GMSEC_I32 flags)
{
	m_iMnemonicSample->setFlags(flags);

}// setFlags()


bool MnemonicSample::hasLimitEnableDisable() const
{
	return m_iMnemonicSample->hasLimitEnableDisable();

}// hasLimiitEnableDisable()


bool MnemonicSample::getLimitEnableDisable() const
{
	return m_iMnemonicSample->getLimitEnableDisable();

}// getLimitEnableDisable()


void MnemonicSample::setLimitEnableDisable(bool value)
{
	m_iMnemonicSample->setLimitEnableDisable(value);

}// setLimitEnableDisable()


bool MnemonicSample::hasLimit() const
{
	return m_iMnemonicSample->hasLimit();

}// hasLimit()


MnemonicSample::LimitFlag MnemonicSample::getLimit() const
{
	return m_iMnemonicSample->getLimit();

}// getLimit()


void MnemonicSample::setLimit(LimitFlag limit)
{
	m_iMnemonicSample->setLimit(limit);

}// setLimit()


bool MnemonicSample::hasStalenessStatus() const
{
	return m_iMnemonicSample->hasStalenessStatus();

}// hasStalenessStatus()


bool MnemonicSample::getStalenessStatus() const
{
	return m_iMnemonicSample->getStalenessStatus();

}// getStalenessStatus()


void MnemonicSample::setStalenessStatus(bool staleness)
{
	m_iMnemonicSample->setStalenessStatus(staleness);

}// setStalenessStatus()


bool MnemonicSample::hasQuality() const
{
	return m_iMnemonicSample->hasQuality();

}// hasQuality()


bool MnemonicSample::getQuality() const
{
	return m_iMnemonicSample->getQuality();

}// getQuality()


void MnemonicSample::setQuality(bool quality)
{
	m_iMnemonicSample->setQuality(quality);

}// setQuality()


bool MnemonicSample::operator==(const MnemonicSample& mnemonicSample) const
{
	return m_iMnemonicSample->operator==(*(mnemonicSample.m_iMnemonicSample));

}// operator==()


bool MnemonicSample::operator!=(const MnemonicSample& mnemonicSample) const
{
	return !(operator==(mnemonicSample));

}// operator!=()
