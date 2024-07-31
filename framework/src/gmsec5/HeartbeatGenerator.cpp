/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file HeartbeatGenerator.cpp
 *
 * @brief This file contains a class for starting a Heartbeat message generator.
 */

#include <gmsec5/HeartbeatGenerator.h>

#include <gmsec5/internal/InternalHeartbeatGenerator.h>

#include <gmsec5/field/Field.h>

#include <gmsec5/Config.h>


using namespace gmsec::api5;
using namespace gmsec::api5::internal;
using namespace gmsec::api5::util;


HeartbeatGenerator::HeartbeatGenerator(const Config& config, const GMSEC_U16 hbPubRate)
	: m_internal(new InternalHeartbeatGenerator(config, hbPubRate))
{
}


HeartbeatGenerator::HeartbeatGenerator(const Config& config, const GMSEC_U16 hbPubRate, const List<Field*>& fields)
	: m_internal(new InternalHeartbeatGenerator(config, hbPubRate, fields))
{
}


HeartbeatGenerator::~HeartbeatGenerator()
{
	delete m_internal;
}


bool HeartbeatGenerator::start()
{
	return m_internal->start();
}


bool HeartbeatGenerator::stop()
{
	return m_internal->stop();
}


bool HeartbeatGenerator::isRunning()
{
	return m_internal->isRunning();
}


void HeartbeatGenerator::changePublishRate(GMSEC_U16 pubRate)
{
	m_internal->changePublishRate(pubRate);
}


bool HeartbeatGenerator::setField(const Field& field)
{
	return m_internal->setField(field);
}


bool HeartbeatGenerator::setField(const char* fieldName, GMSEC_I64 fieldValue)
{
	return m_internal->setField(fieldName, fieldValue);
}


bool HeartbeatGenerator::setField(const char* fieldName, GMSEC_F64 fieldValue)
{
	return m_internal->setField(fieldName, fieldValue);
}


bool HeartbeatGenerator::setField(const char* fieldName, const char* fieldValue)
{
	return m_internal->setField(fieldName, fieldValue);
}
