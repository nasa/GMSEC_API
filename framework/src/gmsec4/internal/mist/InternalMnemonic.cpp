/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file InternalMnemonic.cpp
 *
 *  @brief This file contains definitions for Mnemonic containers
**/

#include <gmsec4/internal/mist/InternalMnemonic.h>
#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/internal/field/InternalField.h>
#include <gmsec4/Exception.h>


using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


InternalMnemonic::InternalMnemonic(const char* name, const DataList<MnemonicSample*>& samples)
	: m_hasStatus(false),
	  m_status(NULL),
	  m_hasUnits(false),
	  m_sampleList()
{
	if (!name || std::string(name).empty())
	{
		throw Exception(MIST_ERROR,
		                OTHER_ERROR_CODE,
		                "InternalMnemonic constructor with list:  NULL or empty name provided.");
	}

	m_name = name;

	for (DataList<MnemonicSample*>::const_iterator it = samples.begin(); it != samples.end(); ++it)
	{
		const MnemonicSample* sample = *it;

		if (sample)
		{
			m_sampleList.add(new MnemonicSample(*sample));
		}
	}
}


InternalMnemonic::InternalMnemonic(const InternalMnemonic& other)
	: m_name(other.m_name),
	  m_hasStatus(other.m_hasStatus),
	  m_status(NULL),
	  m_hasUnits(other.m_hasUnits),
	  m_units(other.m_units),
	  m_sampleList()
{
	for (DataList<MnemonicSample*>::const_iterator it = other.m_sampleList.begin(); it != other.m_sampleList.end(); ++it)
	{
		const MnemonicSample* sample = *it;

		if (sample)
		{
			m_sampleList.add(new MnemonicSample(*sample));
		}
	}

	if (m_hasStatus)
	{
		setStatus(*other.m_status);
	}
}

InternalMnemonic& InternalMnemonic::operator=(const InternalMnemonic& other)
{
    if (this != &other)
    {
        m_name = other.m_name;

		if (other.m_status)
        {
            m_status = gmsec::api::internal::InternalField::makeFieldCopy(*other.m_status);
        }
		m_hasStatus = other.m_hasStatus;

		m_units = other.m_units;
		m_hasUnits = other.m_hasUnits;
	
		for (DataList<MnemonicSample*>::const_iterator it = other.m_sampleList.begin(); it != other.m_sampleList.end(); ++it)
		{
			const MnemonicSample* sample = *it;

			if (sample)
			{
				m_sampleList.add(new MnemonicSample(*sample));
			}
		}
    }
 
    return *this;
}


InternalMnemonic::~InternalMnemonic()
{
	
	if (m_hasStatus)
	{
		delete m_status;
	}
	
	for (DataList<MnemonicSample*>::iterator it = m_sampleList.begin(); it != m_sampleList.end(); ++it)
	{
		delete *it;
	}

	m_sampleList.clear();
}


const char* InternalMnemonic::getName() const
{
	return m_name.c_str();
}


bool InternalMnemonic::statusAvailable() const
{
	return m_hasStatus;
}


const Field& InternalMnemonic::getStatus() const
{
	if (m_hasStatus)
	{
		return *m_status;
	}

	throw Exception(MIST_ERROR,
	                INVALID_FIELD,
	                "InternalMnemonic::getStatus:  status field does not exist in this Mnemonic right now.");
}


void InternalMnemonic::setStatus(const Field& status)
{
	if (m_status)
	{
		delete m_status;
	}

	m_status    = gmsec::api::internal::InternalField::makeFieldCopy(status);
	m_hasStatus = true;
}


bool InternalMnemonic::unitsAvailable() const
{
	return m_hasUnits;
}


const char* InternalMnemonic::getUnits() const
{
	if (m_hasUnits)
	{
		return m_units.c_str();
	}

	throw Exception(MIST_ERROR,
	                OTHER_ERROR_CODE,
	                "InternalMnemonic::getUnits():  Units not available.");
}


void InternalMnemonic::setUnits(const char* units)
{
	m_units    = units;
	m_hasUnits = true;
}


size_t InternalMnemonic::getSampleCount() const
{
	return m_sampleList.size();
}


const MnemonicSample& InternalMnemonic::getSample(size_t idx) const
{
	if (idx < m_sampleList.size())
	{
		return *(m_sampleList.get(idx));
	}

	throw Exception(MIST_ERROR,
	                INDEX_OUT_OF_RANGE,
	                "InternalDevice::getSample():  Mnemonic sample index was specified out-of-range.");
}


void InternalMnemonic::addSample(const MnemonicSample& sample)
{
	m_sampleList.add(new MnemonicSample(sample));
}
