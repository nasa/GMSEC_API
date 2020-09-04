/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module(directors="1") Specification

%{
#include <gmsec4/mist/Specification.h>
using namespace gmsec::api::mist;
%}

// Turn on director mapping for the Specification package
%feature("director") Specification;

%ignore gmsec::api::mist::Specification::getMessageSpecifications() const;

/*
%feature("director:except") {
    if ($error != NULL) {
        throw gmsec::api::Exception(gmsec::api::OTHER_ERROR, gmsec::api::OTHER_ERROR_CODE, SvPV_nolen($error));
    }
}

%exception {
    try { $action }
    catch (const gmsec::api::Exception& e) { SWIG_croak(e.what()); }
}

%apply SWIGTYPE *DISOWN { Specification *spec };
*/

%include "dox/Specification_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Specification.h>

%include <std_list.i>

namespace std
{
    %template(MessageSpecificationList) list<gmsec::api::mist::MessageSpecification*>;
}


%extend gmsec::api::mist::Specification {

    const std::list<gmsec::api::mist::MessageSpecification*> CALL_TYPE getMessageSpecifications()
    {
        const gmsec::api::util::DataList<gmsec::api::mist::MessageSpecification*>& msgSpecs = self->getMessageSpecifications();

        std::list<gmsec::api::mist::MessageSpecification*> newMsgSpecs;

        for (gmsec::api::util::DataList<gmsec::api::mist::MessageSpecification*>::const_iterator it = msgSpecs.begin(); it != msgSpecs.end(); ++it)
        {
            newMsgSpecs.push_back(*it);
        }

        return newMsgSpecs;
    }
}

