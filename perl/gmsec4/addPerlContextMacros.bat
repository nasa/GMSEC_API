rem Copyright 2007-2016 United States Government as represented by the
rem Administrator of The National Aeronautics and Space Administration.
rem No copyright is claimed in the United States under Title 17, U.S. Code.
rem All Rights Reserved.

@echo off
rem There does not appear to be a way within SWIG to add code to the C++ wrapper at the very beginning of a Director function
rem (SwigDirector_Callback_onMessage), so we will have to make do with using perl's regex replacement functionality for now to
rem ensure that threading works for allowing a C++ thread to invoke the Callback which is defined within the Perl interpreter.
rem These statements can be removed if SWIG ever begins to natively support calling into a Perl interpreter from an external
rem thread.
perl -i.bak -pe "s/onMessage\(gmsec::api::Connection \&conn, gmsec::api::Message const \&msg\) \{/onMessage(gmsec::api::Connection &conn, gmsec::api::Message const &msg) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g" interfaces\libgmsec_perl_wrap.cxx
perl -i.bak -pe "s/onEvent\(gmsec::api::Connection \&conn, gmsec::api::Status const \&status, gmsec::api::Connection::ConnectionEvent event\) \{/onEvent(gmsec::api::Connection &conn, gmsec::api::Status const &status, gmsec::api::Connection::ConnectionEvent event) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g" interfaces\libgmsec_perl_wrap.cxx
perl -i.bak -pe "s/onReply\(gmsec::api::Connection \&conn, gmsec::api::Message const \&request, gmsec::api::Message const \&reply\) \{/onReply(gmsec::api::Connection &conn, gmsec::api::Message const &request, gmsec::api::Message const &reply) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g" interfaces\libgmsec_perl_wrap.cxx
perl -i.bak -pe "s/LogHandler::onMessage\(gmsec::api::util::LogEntry const \&entry\) \{/LogHandler::onMessage(gmsec::api::util::LogEntry const &entry) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g" interfaces\libgmsec_perl_wrap.cxx
@echo on
