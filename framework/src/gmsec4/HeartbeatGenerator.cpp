/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file HeartbeatGenerator.cpp
 *
 *  @brief This file contains a class for starting a Heartbeat message generator.
 */

#include <gmsec4/HeartbeatGenerator.h>

#include <gmsec4/internal/InternalHeartbeatGenerator.h>

#include <gmsec4/field/Field.h>

#include <gmsec4/Config.h>


using namespace gmsec::api;
using namespace gmsec::api::util;


HeartbeatGenerator::HeartbeatGenerator(const Config& config, const char* hbMsgSubject, const GMSEC_U16 hbPubRate)
	: m_internal(new gmsec::api::internal::InternalHeartbeatGenerator(config, hbMsgSubject, hbPubRate))
{
}


HeartbeatGenerator::HeartbeatGenerator(const Config& config, const char* hbMsgSubject, const GMSEC_U16 hbPubRate, const DataList<Field*>& fields)
	: m_internal(new gmsec::api::internal::InternalHeartbeatGenerator(config, hbMsgSubject, hbPubRate, fields))
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
