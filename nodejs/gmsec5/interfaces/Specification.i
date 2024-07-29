/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Specification

%include "std_vector.i"

%{
#include <gmsec5/Specification.h>
using namespace gmsec::api5;
%}


/* Functions containing lists that will be redefined */
%ignore gmsec::api5::Specification::getMessageSpecifications() const;
%ignore gmsec::api5::Specification::getHeaderFieldNames() const;


%include <gmsec5/util/wdllexp.h>
%include <gmsec5/Specification.h>


namespace std
{
    %template(MsgSpecArray) vector<gmsec::api5::MessageSpecification*>;
}


%extend gmsec::api5::Specification {

    std::vector<gmsec::api5::MessageSpecification*> CALL_TYPE getMsgSpecs()
    {
        const gmsec::api5::util::List<gmsec::api5::MessageSpecification*>& msgSpecs = self->getMessageSpecifications();

        std::vector<gmsec::api5::MessageSpecification*> msgSpecArray;

        for (gmsec::api5::util::List<gmsec::api5::MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
        {
            msgSpecArray.push_back(*it);
        }

        return msgSpecArray;
    }

    std::vector<std::string> CALL_TYPE getHeaderFields(const char* schemaName = NULL)
	{
		const gmsec::api5::util::List<const char*>& headers = self->getHeaderFieldNames(schemaName);
		
		std::vector<std::string> headerFieldNameArray;

		for (gmsec::api5::util::List<const char*>::const_iterator it = headers.begin(); it != headers.end(); ++it)
		{
			headerFieldNameArray.push_back(*it);
		}

        return headerFieldNameArray;
    }
}

