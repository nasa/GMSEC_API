/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file Mnemonic.h
 *
 *  @brief This file contains definitions for Mnemonic containers
**/

#include <gmsec4/mist/Mnemonic.h>
#include <gmsec4/Exception.h>

#include <gmsec4/internal/mist/InternalMnemonic.h>

#include <gmsec4/util/DataList.h>

using namespace gmsec::api;
using namespace gmsec::api::util;
using namespace gmsec::api::mist;
using namespace gmsec::api::mist::internal;


Mnemonic::Mnemonic(const char* name, const DataList<MnemonicSample*>& samples)
	: m_iMnemonic(new InternalMnemonic(name, samples))
{
}


Mnemonic::Mnemonic(const char* name)
	: m_iMnemonic(new InternalMnemonic(name, DataList<MnemonicSample*>()))
{
}


Mnemonic::Mnemonic(const Mnemonic& other)
	: m_iMnemonic(new InternalMnemonic(*(other.m_iMnemonic)))
{
}


Mnemonic& Mnemonic::operator=(const Mnemonic& other)
{
	if (this != &other)
    {
        delete m_iMnemonic;
 
        m_iMnemonic = new InternalMnemonic(*other.m_iMnemonic);
    }
 
    return *this;
}

Mnemonic::~Mnemonic()
{
	delete m_iMnemonic;
}


const char* Mnemonic::getName() const
{
	return m_iMnemonic->getName();
}


bool Mnemonic::statusAvailable() const
{
	return m_iMnemonic->statusAvailable();
}


const Field& Mnemonic::getStatus() const
{
	return m_iMnemonic->getStatus();
}


void Mnemonic::setStatus(const Field& status)
{
	m_iMnemonic->setStatus(status);
}


bool Mnemonic::unitsAvailable() const
{
	return m_iMnemonic->unitsAvailable();
}


const char* Mnemonic::getUnits() const
{
	return m_iMnemonic->getUnits();
}


void Mnemonic::setUnits(const char* units)
{
	m_iMnemonic->setUnits(units);
}


size_t Mnemonic::getSampleCount() const
{
	return m_iMnemonic->getSampleCount();
}


const MnemonicSample& Mnemonic::getSample(size_t idx) const
{
	return m_iMnemonic->getSample(idx);
}


void Mnemonic::addSample(const MnemonicSample& sample)
{
	m_iMnemonic->addSample(sample);
}

