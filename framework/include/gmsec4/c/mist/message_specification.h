/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file message_specification.h
 *
 *  @brief This file contains data structures for a GMSEC_MessageSpecification
 *  and a GMSEC_FieldSpecification.
 *
 *  @sa specificationGetMessageSpecifications
 */


#ifndef GMSEC_API_C_MESSAGE_SPECIFICATION_H
#define GMSEC_API_C_MESSAGE_SPECIFICATION_H


typedef struct __FieldSpecification
{
	const char* name;
	const char* type;
	const char* mode;
	const char* classification;
	const char* value;
	const char* description;

} GMSEC_FieldSpecification;


typedef struct __MessageSpecification
{
	const char* schemaID;

	GMSEC_FieldSpecification* fieldSpecs;

	int numFieldSpecs;

} GMSEC_MessageSpecification;


#endif
