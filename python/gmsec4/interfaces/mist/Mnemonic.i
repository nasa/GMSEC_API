/*
 * Copyright 2007-2016 United States Government as represented by the
 * Administrator of the National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

%module Mnemonic
%{
#include <gmsec4/mist/Mnemonic.h>
#include <list>
using namespace gmsec::api::mist;
%}

%ignore gmsec::api::mist::Mnemonic::Mnemonic(const char*, const gmsec::api::util::DataList<gmsec::api::mist::MnemonicSample*>&);
%ignore gmsec::api::mist::Mnemonic::addSample(const MnemonicSample&);


%include "dox/Mnemonic_dox.i"
%include "dox/MnemonicSampleList_dox.i"
%include <gmsec4/util/wdllexp.h>
%include <gmsec4/mist/Mnemonic.h>

%include <std_list.i>

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

    void CALL_TYPE addSample(const gmsec::api::mist::MnemonicSample& sample)
    {
        self->addSample(sample);
    }
}

