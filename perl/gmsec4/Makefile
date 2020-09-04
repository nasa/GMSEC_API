# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

GMSEC_HOME = ../..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

TARGET	= libgmsec_perl.$(SHLIB_EXT)

SWIG          = $(SWIG_HOME)/bin/swig 
SWIG_VERSION  = $(shell $(SWIG) -version | grep Version | cut -d' ' -f3)
SWIG_SWG      = -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION) -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)/perl5
SWIG_OPTS     = -c++ -perl5 $(SWIG_SWG)

FRAMEWORK_DIR=../../framework

GMSEC_INCS   = -I$(FRAMEWORK_DIR)/include -I/usr/include
PERL_INCS    = -I$(PERL5_LIB)/CORE
API_CXXFLAGS += $(PERL_INCS)

COREOBJDIR     = $(FRAMEWORK_DIR)/src/gmsec4
INTERNALOBJDIR = $(FRAMEWORK_DIR)/src/gmsec4/internal
INTERFACEDIR   = interfaces

PERL4_BINDIR = $(BINDIR)/lib/GMSECAPI4

WRAPPED_SRC = \
	$(INTERFACEDIR)/libgmsec_perl_wrap.cxx

WRAPPED_OBJ = \
	$(INTERFACEDIR)/libgmsec_perl_wrap.o

OBJECTS += $(WRAPPED_OBJ)

default: $(WRAPPED_SRC) $(WRAPPED_OBJ) $(BINDIR)/$(TARGET) install

$(INTERFACEDIR)/%_wrap.cxx: $(INTERFACEDIR)/%.i
	$(SWIG) $(SWIG_OPTS) $(GMSEC_INCS) $<
# There does not appear to be a way within SWIG to add code to the C++ wrapper at the very beginning of a Director function
# (SwigDirector_Callback_onMessage), so we will have to make do with using sed for now to ensure that threading works for
# allowing a C++ thread to invoke the Callback which is defined within the Perl interpreter.
# These statements can be removed if SWIG ever begins to natively support calling into a Perl interpreter from an external
# thread.
	sed -i 's/onMessage(gmsec::api::Connection &conn, gmsec::api::Message const &msg) {/onMessage(gmsec::api::Connection \&conn, gmsec::api::Message const \&msg) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $(WRAPPED_SRC)
	sed -i 's/onEvent(gmsec::api::Connection &conn, gmsec::api::Status const &status, gmsec::api::Connection::ConnectionEvent event) {/onEvent(gmsec::api::Connection \&conn, gmsec::api::Status const \&status, gmsec::api::Connection::ConnectionEvent event) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $(WRAPPED_SRC)
	sed -i 's/onReply(gmsec::api::Connection &conn, gmsec::api::Message const &request, gmsec::api::Message const &reply) {/onReply(gmsec::api::Connection \&conn, gmsec::api::Message const \&request, gmsec::api::Message const \&reply) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $(WRAPPED_SRC)
	sed -i 's/LogHandler::onMessage(gmsec::api::util::LogEntry const &entry) {/LogHandler::onMessage(gmsec::api::util::LogEntry const \&entry) {\n  GMSEC_SWIG_SET_PERL_CONTEXT;/g' $(WRAPPED_SRC)

$(INTERFACEDIR)/%.o: $(INTERFACEDIR)/%.cxx
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@

install:
	mkdir -p $(PERL4_BINDIR)
	cp $(INTERFACEDIR)/*.pm $(PERL4_BINDIR)

clean:
	find $(INTERFACEDIR) \( -name '*.cxx' -o -name '*.pm' -o -name '*.o' -o -name '*.h' -o -name 'Makefile' -o -name 'help' -o -name 'pm_to_blib' -o -name 'blib' \) -exec rm -rf {} \;
	rm -rf $(BINDIR)/$(TARGET) $(PERL4_BINDIR)
	

