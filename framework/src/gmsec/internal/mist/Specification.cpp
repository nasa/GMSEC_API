
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Specification.cpp
 *
 *  @brief This file contains the Specification class.
 *
**/

#include <gmsec/internal/mist/Specification.h>

#include <gmsec/mist/ConnectionManager.h>

#include <gmsec/internal/mist/BaseSpecification.h>

#include <gmsec/util/List.h>
#include <gmsec/util/String.h>

using namespace gmsec;
using namespace gmsec::util;
using namespace gmsec::mist;
using namespace gmsec::internal;
using namespace gmsec::mist::internal;

Specification::Specification()
: ptr(new internal::BaseSpecification())
{}

Specification::~Specification()
{
	delete ptr;
	ptr = 0;
}

Status Specification::Load(const char* basePath, unsigned int version)
{
	return ptr->Load(basePath, version);
}

// path will hold XML message definitions of the form above
Status Specification::LoadTemplate(const char * id, const char * path)
{
	return ptr->LoadTemplate(id, path);
}

// Update message (already created) according to template <id>
Status Specification::Apply(Message *message, Context &context) const
{
	return ptr->Apply(message, context);
}

Status Specification::Instantiate(Message *message, Context &context) const
{
	return ptr->Instantiate(message, context);
}

// validate message against template
Status Specification::Validate(Message *message, Context &context) const
{
	return ptr->Validate(message, context);
}

Status Specification::Register(const char* id, MessageTemplate* newTemplate)
{
	return ptr->Register(id, newTemplate);
}

// Fetches list of template IDs
void Specification::GetTemplateIds(List<String>& idList)
{
	ptr->GetTemplateIds(idList);

}

// Checks if a given message matches a template in this BaseSpecification
Status Specification::LookupTemplate(Message* message, String&  templateIdStr)
{
	return ptr->LookupTemplate(message, templateIdStr);

}
