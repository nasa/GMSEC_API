#!/usr/bin/env bash


# There does not appear to be a way within SWIG to add code to the C++ wrapper at the very beginning of a Director function
# (SwigDirector_Callback_onMessage), so we will have to make do with using sed for now to ensure that threading works for
# allowing a C++ thread to invoke the Callback which is defined within the Perl interpreter.
# These statements can be removed if SWIG ever begins to natively support calling into a Perl interpreter from an external
# thread.


WRAPPED_SRC=./interfaces/libgmsec_perl_wrap.cxx


if [[ "$GMSEC_PLATFORM" == "linux"* ]]; then

	echo "Patching $WRAPPED_SRC for Linux..."

        sed -i 's/onMessage(gmsec::api::Connection &conn, gmsec::api::Message const &msg) {/onMessage(gmsec::api::Connection \&conn, gmsec::api::Message const \&msg) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC
        sed -i 's/onEvent(gmsec::api::Connection &conn, gmsec::api::Status const &status, gmsec::api::Connection::ConnectionEvent event) {/onEvent(gmsec::api::Connection \&conn, gmsec::api::Status const \&status, gmsec::api::Connection::ConnectionEvent event) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC
        sed -i 's/onReply(gmsec::api::Connection &conn, gmsec::api::Message const &request, gmsec::api::Message const &reply) {/onReply(gmsec::api::Connection \&conn, gmsec::api::Message const \&request, gmsec::api::Message const \&reply) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC
        sed -i 's/LogHandler::onMessage(gmsec::api::util::LogEntry const &entry) {/LogHandler::onMessage(gmsec::api::util::LogEntry const \&entry) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC

elif [[ "$GMSEC_PLATFORM" == "macosx"* ]]; then

	echo "Patching $WRAPPED_SRC for OS X..."

	sed -i '' 's/onMessage(gmsec::api::Connection &conn, gmsec::api::Message const &msg) {/onMessage(gmsec::api::Connection \&conn, gmsec::api::Message const \&msg) {\'$'\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC

	sed -i '' 's/onEvent(gmsec::api::Connection &conn, gmsec::api::Status const &status, gmsec::api::Connection::ConnectionEvent event) {/onEvent(gmsec::api::Connection \&conn, gmsec::api::Status const \&status, gmsec::api::Connection::ConnectionEvent event) {\'$'\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC

	sed -i '' 's/onReply(gmsec::api::Connection &conn, gmsec::api::Message const &request, gmsec::api::Message const &reply) {/onReply(gmsec::api::Connection \&conn, gmsec::api::Message const \&request, gmsec::api::Message const \&reply) {\'$'\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC

	sed -i '' 's/LogHandler::onMessage(gmsec::api::util::LogEntry const &entry) {/LogHandler::onMessage(gmsec::api::util::LogEntry const \&entry) {\'$'\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC

else

	echo "$WRAPPED_SRC is not being patched!"

fi

exit 0




