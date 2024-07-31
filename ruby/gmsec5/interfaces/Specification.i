/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%include "std_vector.i"

%{
#include <gmsec5/Specification.h>
#include <vector>
using namespace gmsec::api5;
%}


/* methods containing lists that will be redefined */
%ignore gmsec::api5::Specification::getSchemaIDIterator() const;
%ignore gmsec::api5::Specification::getMessageSpecifications() const;
%ignore gmsec::api5::Specification::getHeaderFieldNames() const;

%rename("get_schema_id_iterator") getSchemaIDIterator;
%rename("get_version") getVersion;
%rename("get_schema_level") getSchemaLevel;
%rename("get_message_specifications") getMessageSpecifications;
%rename("get_header_field_names") getHeaderFieldNames;


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Specification.h>


namespace std
{
    %template(MessageSpecificationArray) vector<gmsec::api5::MessageSpecification*>;
    %template(HeaderArray) vector<std::string>;
}


%extend gmsec::api5::Specification
{
    SchemaIDIterator& get_schema_id_iterator()
    {
        return self->getSchemaIDIterator();
    }

    const std::vector<gmsec::api5::MessageSpecification*> CALL_TYPE getMessageSpecifications()
    {
        const gmsec::api5::util::List<gmsec::api5::MessageSpecification*>& msgSpecs = self->getMessageSpecifications();

        std::vector<gmsec::api5::MessageSpecification*> newMsgSpecs;

        for (gmsec::api5::util::List<gmsec::api5::MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
        {
            newMsgSpecs.push_back(*it);
        }

        return newMsgSpecs;
    }
	
	const std::vector<std::string> CALL_TYPE getHeaderFieldNames()
	{
		const gmsec::api5::util::List<const char*>& headers = self->getHeaderFieldNames("DEFAULT");
		
		std::vector<std::string> headerList;
		for (gmsec::api5::util::List<const char*>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		{
			headerList.push_back(*it);
		}
		
		return headerList;
	}
	
	const std::vector<std::string> CALL_TYPE getHeaderFieldNames(std::string schemaName)
	{
		const gmsec::api5::util::List<const char*>& headers = self->getHeaderFieldNames(schemaName.c_str());
		
		std::vector<std::string> headerList;
		for (gmsec::api5::util::List<const char*>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		{
			headerList.push_back(*it);
		}
		
		return headerList;
	}
}

