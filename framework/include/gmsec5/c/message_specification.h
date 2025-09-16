/*
 * Copyright 2007-2025 United States Government as represented by the
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


#ifndef GMSEC_API5_C_MESSAGE_SPECIFICATION_H
#define GMSEC_API5_C_MESSAGE_SPECIFICATION_H

#include <stddef.h>


/**
 * @struct GMSEC_FieldSpecification
 */
typedef struct __FieldSpecification
{
	const char* name;
	const char* type;
	const char* mode;
	const char* classification;
	const char* value;
	const char* description;

} GMSEC_FieldSpecification;


/**
 * @struct GMSEC_MessageSpecification
 */
typedef struct __MessageSpecification
{
	const char*               schemaID;
	const char*               subjectTemplate;
	GMSEC_FieldSpecification* fieldSpecs;
	size_t                    numFieldSpecs;

} GMSEC_MessageSpecification;


#endif
