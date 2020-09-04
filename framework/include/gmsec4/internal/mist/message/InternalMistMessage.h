/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef GMSEC_API_INTERNAL_MIST_MESSAGE_H
#define GMSEC_API_INTERNAL_MIST_MESSAGE_H

#include <gmsec4/internal/InternalMessage.h>

#include <gmsec4/internal/mist/MessageTemplate.h>
#include <gmsec4/internal/mist/FieldTemplate.h>

#include <string>


namespace gmsec
{
namespace api
{
	// Forward declaration(s)
	class Config;
	class Status;

namespace mist
{
	// Forward declaration(s)
	class Specification;

namespace message
{
namespace internal
{


class GMSEC_API InternalMistMessage : public gmsec::api::internal::InternalMessage
{
public:
	InternalMistMessage(const char* subject,
						Message::MessageKind kind,
						const char* schemaID,
						const gmsec::api::mist::Specification& spec);
	

	InternalMistMessage(const char* subject,
						Message::MessageKind kind,
						const char* schemaID,
						const gmsec::api::Config& config,
						const gmsec::api::mist::Specification& spec);


	InternalMistMessage(const InternalMistMessage& other);


	InternalMistMessage(const char* data);


	virtual ~InternalMistMessage();
	
	//TODO (MAV): These allow the message to remember if it's already been validated or not, or if it needs to be revalidated. This might not be necessary.
	bool isValid() const;
	void validate();
	void invalidate();

	const char* getSchemaID() const;

	unsigned int getSpecVersion() const;
	void setSpecVersion(unsigned int version);

	const gmsec::api::mist::internal::MessageTemplate& getTemplate() const;

	void setValue(const char* fieldName, const char* value);
	void setValue(const char* fieldName, GMSEC_I64 value);
	void setValue(const char* fieldName, GMSEC_F64 value);

	void init();

	// Helper methods to assist with preserving compatibility between MIST2 messages and those from MIST3
	static std::string buildSchemaID(Message::MessageKind kind, const char* type, const char* subType, unsigned int version);
	static Specification buildSpecification(unsigned int version);
	static Message::MessageKind findKind(const char* schemaID);

	//helper function for constructer, search spec for appropriate template
	//given by schema ID, assign to m_template
	void registerTemplate(const char* schemaID);

private:

	//helper function, returns index of fieldTemplate in m_template with the given fieldName
	//returns -1 if the fieldTemplate does not exist
	size_t findFieldTemplateIndex(const char* fieldName);
	const gmsec::api::mist::internal::FieldTemplate& findFieldTemplate(const char* fieldName);

	//flag determining whether the message has passed validation (may not be needed)
	bool m_valid;

	//this is a temporary buffer that will hold the return value of getValue()
	//without it, the string inside getValue() will be destroyed before it can be returned
	std::string	m_valueBuffer;

	//specification the message will be based off of
	gmsec::api::mist::Specification* m_spec;

	//this template contains the schema to be used for both populating the message
	//as well as for use by validation (to be implemented)
	gmsec::api::mist::internal::MessageTemplate m_template;
};

} // namespace internal
} // namespace message
} // namespace mist
} // namespace api
} // namespace gmsec

#endif
