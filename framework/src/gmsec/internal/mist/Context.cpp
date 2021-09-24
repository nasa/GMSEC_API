
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file Context.cpp
 *
 *  @brief This file contains the Context class.
 *
**/

#include <gmsec/internal/mist/Context.h>
#include <gmsec/internal/mist/BaseContext.h>

using namespace gmsec;
using namespace gmsec::internal;
using namespace gmsec::mist::internal;

Context::Context()
: ptr(new BaseContext())
{
}

Context::Context(const char * templateId, bool validate)
: ptr(new BaseContext(templateId, validate))
{
}
	
Context::~Context()
{
	delete ptr;
	ptr = 0;
}

Status Context::ValidateBoolean(bool validate)
{
	return ptr->ValidateBoolean(validate);
}
	
Status Context::GetTemplateId(const char * &id) const
{
	return ptr->GetTemplateId(id);
}

Status Context::Reset(const char * templateId)
{
	return ptr->Reset(templateId);
}

Status Context::ResetContext(BaseContext* bc)
{
	Status status;

	const char * idName;
	status = GetTemplateId(idName);

	if(status.isError())
	{
		return status;
	}
	bool validate = ptr->GetValidate();

	delete ptr;
	ptr = bc;
	status = ptr->Reset(idName);
	ptr->ValidateBoolean(validate);

	// Clean up the const char* allocated by BaseContext::GetTemplateId()
	gmsec::util::stringDestroy(idName);

	return status;
}

bool Context::GetValidate() const
{
	return ptr->GetValidate();
}

bool Context::Visited(const char * name) const
{
	return ptr->Visited(name);
}

size_t Context::GetStatusCount()
{
	return ptr->GetStatusCount();
}

Status Context::GetStatusIndex(size_t index, const char * &name, const char * &desc, int &code)
{
	return ptr->GetStatusIndex(index, name, desc, code);
}
