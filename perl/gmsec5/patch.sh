#!/usr/bin/env bash

# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


# There does not appear to be a way within SWIG to add code to the C++ wrapper at the very beginning of a Director function
# (SwigDirector_Callback_onMessage), so we will have to make do with using sed for now to ensure that threading works for
# allowing a C++ thread to invoke the Callback which is defined within the Perl interpreter.
# These statements can be removed if SWIG ever begins to natively support calling into a Perl interpreter from an external
# thread.


WRAPPED_SRC=./interfaces/libgmsec_perl_wrap.cxx


if [[ "$GMSEC_PLATFORM" == "linux"* ]]; then

	echo "Patching $WRAPPED_SRC for Linux..."

        sed -i 's/onMessage(gmsec::api5::Connection &conn, gmsec::api5::Message const &msg) {/onMessage(gmsec::api5::Connection \&conn, gmsec::api5::Message const \&msg) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC
        sed -i 's/onEvent(gmsec::api5::Connection &conn, gmsec::api5::Status const &status, gmsec::api5::Connection::Event event) {/onEvent(gmsec::api5::Connection \&conn, gmsec::api5::Status const \&status, gmsec::api5::Connection::Event event) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC
        sed -i 's/onReply(gmsec::api5::Connection &conn, gmsec::api5::Message const &request, gmsec::api5::Message const &reply) {/onReply(gmsec::api5::Connection \&conn, gmsec::api5::Message const \&request, gmsec::api5::Message const \&reply) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC
        sed -i 's/LogHandler::onMessage(gmsec::api5::util::LogEntry const &entry) {/LogHandler::onMessage(gmsec::api5::util::LogEntry const \&entry) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC

	# Additional handling for ensuring that the globally-defined XEntry classes (from ConfigFile) are used
	# Need to include the comma at the end of the replacement string in order to not overwrite the SWIG-generated definition for the (unused) ConfigFile::XEntry classes.
	sed -i 's/SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry,/SWIGTYPE_p_SubscriptionEntry,/g' $WRAPPED_SRC
	sed -i 's/SWIGTYPE_p_gmsec__api5__ConfigFile__MessageEntry,/SWIGTYPE_p_MessageEntry,/g' $WRAPPED_SRC
	sed -i 's/SWIGTYPE_p_gmsec__api5__ConfigFile__ConfigEntry,/SWIGTYPE_p_ConfigEntry,/g' $WRAPPED_SRC

elif [[ "$GMSEC_PLATFORM" == "macosx"* ]]; then

	echo "Patching $WRAPPED_SRC for OS X..."

	sed -i '' 's/onMessage(gmsec::api5::Connection &conn, gmsec::api5::Message const &msg) {/onMessage(gmsec::api5::Connection \&conn, gmsec::api5::Message const \&msg) {\'$'\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC

	sed -i '' 's/onEvent(gmsec::api5::Connection &conn, gmsec::api5::Status const &status, gmsec::api5::Connection::Event event) {/onEvent(gmsec::api5::Connection \&conn, gmsec::api5::Status const \&status, gmsec::api5::Connection::Event event) {\'$'\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC

	sed -i '' 's/onReply(gmsec::api5::Connection &conn, gmsec::api5::Message const &request, gmsec::api5::Message const &reply) {/onReply(gmsec::api5::Connection \&conn, gmsec::api5::Message const \&request, gmsec::api5::Message const \&reply) {\'$'\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC

	sed -i '' 's/LogHandler::onMessage(gmsec::api5::util::LogEntry const &entry) {/LogHandler::onMessage(gmsec::api5::util::LogEntry const \&entry) {\'$'\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $WRAPPED_SRC

	# Additional handling for ensuring that the globally-defined XEntry classes (from ConfigFile) are used
	# Need to include the comma at the end of the replacement string in order to not overwrite the SWIG-generated definition for the (unused) ConfigFile::XEntry classes.
	sed -i '' 's/SWIGTYPE_p_gmsec__api5__ConfigFile__SubscriptionEntry,/SWIGTYPE_p_SubscriptionEntry,/g' $WRAPPED_SRC
	sed -i '' 's/SWIGTYPE_p_gmsec__api5__ConfigFile__MessageEntry,/SWIGTYPE_p_MessageEntry,/g' $WRAPPED_SRC
	sed -i '' 's/SWIGTYPE_p_gmsec__api5__ConfigFile__ConfigEntry,/SWIGTYPE_p_ConfigEntry,/g' $WRAPPED_SRC

else

	echo "$WRAPPED_SRC is not being patched!"

fi

exit 0
