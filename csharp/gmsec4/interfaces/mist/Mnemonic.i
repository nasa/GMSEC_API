/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Mnemonic

%include "std_list.i"

%{
#include <gmsec4/mist/Mnemonic.h>
#include <list>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::Mnemonic::Mnemonic(const char*, const gmsec::api::util::DataList<MnemonicSample*>&);
%ignore gmsec::api::mist::Mnemonic::getSample(size_t) const;
%ignore gmsec::api::mist::Mnemonic::addSample(const MnemonicSample&);

%ignore gmsec::api::mist::Mnemonic::operator=(const Mnemonic&);

%rename("GetName") getName;
%rename("StatusAvailable") statusAvailable;
%rename("GetStatus") getStatus;
%rename("SetStatus") setStatus;
%rename("UnitsAvailable") unitsAvailable;
%rename("GetUnits") getUnits;
%rename("SetUnits") setUnits;
%rename("GetSampleCount") getSampleCount;
%rename("GetSample") getSample;
%rename("AddSample") addSample;


%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Mnemonic.h>

PTR_REF_TYPEMAPS(MnemonicSample, gmsec::api::mist::MnemonicSample*)
SWIG_STD_LIST_SPECIALIZE(MnemonicSample, gmsec::api::mist::MnemonicSample*)

namespace std 
{
    %template(MnemonicSampleList) list<gmsec::api::mist::MnemonicSample*>;
}


%extend gmsec::api::mist::Mnemonic {

    Mnemonic(const char* name, const std::list<gmsec::api::mist::MnemonicSample*>& samples)
    {
        gmsec::api::util::DataList<gmsec::api::mist::MnemonicSample*> list;

        for(std::list<gmsec::api::mist::MnemonicSample*>::const_iterator it = samples.begin(); it != samples.end(); ++it)
            list.push_back(*it);
    
        return new Mnemonic(name, list);
    }

    const gmsec::api::mist::MnemonicSample& CALL_TYPE getSample(size_t idx)
    {
        return self->getSample(idx);
    }

    void CALL_TYPE addSample(const gmsec::api::mist::MnemonicSample& sample)
    {
        self->addSample(sample);
    }
}

