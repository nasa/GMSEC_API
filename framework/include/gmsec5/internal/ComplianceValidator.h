/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file ComplianceValidator.h
 *
 * @brief Utility class used to determine if GMSEC Message is compliant with the specification in use.
 */

#ifndef GMSEC_API5_INTERNAL_COMPLIANCE_VALIDATOR_H
#define GMSEC_API5_INTERNAL_COMPLIANCE_VALIDATOR_H

#include <gmsec5/internal/CustomMessageValidator.h>
#include <gmsec5/internal/TrackingDetails.h>

#include <gmsec5/Status.h>

#include <gmsec5/field/Field.h>

#include <gmsec5/util/StdUniquePtr.h>
#include <gmsec5/util/wdllexp.h>

#include <list>
#include <string>
#include <vector>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Config;
	class Message;
	class MessageValidator;

namespace internal
{
	// Forward declaration(s)
	class FieldTemplate;
	class InternalMessage;


class GMSEC_API ComplianceValidator
{
public:
	ComplianceValidator(const Config& config);

	ComplianceValidator(const ComplianceValidator& other);

	~ComplianceValidator();

	void CALL_TYPE updateConfig(const Config& config);

	Status CALL_TYPE validateMessage(const Message& msg);

	void CALL_TYPE registerMessageValidator(MessageValidator* validator);

	void CALL_TYPE registerMessageValidator(GMSEC_MessageValidator* validator);

private:
	typedef std::vector<FieldTemplate*> FieldTemplateCollection;

	Status compare(const InternalMessage& msg, const FieldTemplateCollection& fields, const std::string& prefix, std::string& validFieldNames);

	std::string prepFieldName(const std::string& name, const std::list<std::string>& charList, const std::list<size_t>& indexList);

	std::string getTypeStr(Field::Type type);

	Status validate(const InternalMessage& msg, const FieldTemplate& ftmp);

	Status validateContent(const InternalMessage& msg, const FieldTemplate& ftmp);

	bool isApiReservedField(const char* fieldName);

	Status checkTrackingField(const char* fieldName, const TrackingDetails& configTracking, const TrackingDetails& msgTracking);

	// Member data
	TrackingDetails                               m_configTracking;
	MessageValidator*                             m_customMessageValidator;
	util::StdUniquePtr<CustomMessageValidator>    m_ceeMessageValidator;
};


} // end namespace internal
} // end namespace api5
} // end namespace gmsec


#endif
