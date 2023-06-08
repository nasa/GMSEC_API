rem Copyright 2007-2023 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

@echo off
rem There does not appear to be a way within SWIG to add code to the C++ wrapper at the very beginning of a Director function
rem (SwigDirector_Callback_onMessage), so we will have to make do with using perl's regex replacement functionality for now to
rem ensure that threading works for allowing a C++ thread to invoke the Callback which is defined within the Perl interpreter.
rem These statements can be removed if SWIG ever begins to natively support calling into a Perl interpreter from an external
rem thread.
perl -i.bak -pe "s/onMessage\(gmsec::api5::Connection \&conn, gmsec::api5::Message const \&msg\) \{/onMessage(gmsec::api5::Connection &conn, gmsec::api5::Message const &msg) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g" interfaces\libgmsec_perl_wrap.cxx
perl -i.bak -pe "s/onEvent\(gmsec::api5::Connection \&conn, gmsec::api5::Status const \&status, gmsec::api5::Connection::Event event\) \{/onEvent(gmsec::api5::Connection &conn, gmsec::api5::Status const &status, gmsec::api5::Connection::Event event) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g" interfaces\libgmsec_perl_wrap.cxx
perl -i.bak -pe "s/onReply\(gmsec::api5::Connection \&conn, gmsec::api5::Message const \&request, gmsec::api5::Message const \&reply\) \{/onReply(gmsec::api5::Connection &conn, gmsec::api5::Message const &request, gmsec::api5::Message const &reply) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g" interfaces\libgmsec_perl_wrap.cxx
perl -i.bak -pe "s/LogHandler::onMessage\(gmsec::api5::util::LogEntry const \&entry\) \{/LogHandler::onMessage(gmsec::api5::util::LogEntry const &entry) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g" interfaces\libgmsec_perl_wrap.cxx

rem Additional handling for ensuring that the globally-defined XEntry classes (from ConfigFile) are used
rem Need to include the comma at the end of the replacement string in order to not overwrite the SWIG-generated definition for the (unused) ConfigFile::XEntry classes.
perl -i.bak -pe "s/SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry,/SWIGTYPE_p_SubscriptionEntry,/g" interfaces\libgmsec_perl_wrap.cxx
perl -i.bak -pe "s/SWIGTYPE_p_gmsec__api5__ConfigFile__MessageEntry,/SWIGTYPE_p_MessageEntry,/g" interfaces\libgmsec_perl_wrap.cxx
perl -i.bak -pe "s/SWIGTYPE_p_gmsec__api5__ConfigFile__ConfigEntry,/SWIGTYPE_p_ConfigEntry,/g" interfaces\libgmsec_perl_wrap.cxx

@echo on
