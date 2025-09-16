/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ResourceGenerator.cpp
 *
 * @brief Utility for starting a service that periodically publishes
 * Resource Messages to the GMSEC Bus.
 */

#include <gmsec5/ResourceGenerator.h>

#include <gmsec5/Config.h>
#include <gmsec5/MessageFactory.h>
#include <gmsec5/field/Field.h>

#include <gmsec5/internal/InternalResourceGenerator.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


ResourceGenerator::ResourceGenerator(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval)
	: m_internal(new InternalResourceGenerator(config, pubRate, sampleInterval, averageInterval))
{
}


ResourceGenerator::ResourceGenerator(const Config& config, GMSEC_U16 pubRate, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval,
                                     const gmsec::api5::util::List<Field*>& fields)
	: m_internal(new InternalResourceGenerator(config, pubRate, sampleInterval, averageInterval, fields))
{
}


ResourceGenerator::~ResourceGenerator()
{
	delete m_internal;
}


bool ResourceGenerator::start()
{
	return m_internal->start();
}


bool ResourceGenerator::stop()
{
	return m_internal->stop();
}


bool ResourceGenerator::isRunning()
{
	return m_internal->isRunning();
}


bool ResourceGenerator::setField(const Field& field)
{
	return m_internal->setField(field);
}


Message ResourceGenerator::createResourceMessage(MessageFactory& factory, GMSEC_U16 sampleInterval, GMSEC_U16 averageInterval)
{
	return InternalResourceGenerator::createResourceMessage(factory, sampleInterval, averageInterval);
}
