# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

GMSEC_API_HOME = ../..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

TARGET	        = libgmsec_perl.$(SHLIB_EXT)

SWIG            = $(SWIG_HOME)/bin/swig
SWIG_VERSION    = $(shell $(SWIG) -version | grep Version | cut -d' ' -f3)
SWIG_SWG        = -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)/perl5 -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)
SWIG_OPTS       = -c++ -perl5 $(SWIG_SWG) -DCHAR_BIT=8 -DSWIG_BINDING

FRAMEWORK_DIR   = ../../framework

GMSEC_INCS      = -I$(FRAMEWORK_DIR)/include -I$(CXX_INCS)

API_CXXFLAGS   += -Wno-literal-suffix -I$(PERL5_INC)/CORE

ifeq (,$(wildcard /etc/lsb-release))
# RHEL and MacOS
LDFLAGS        += -L$(PERL5_LIB)/CORE -lperl
else
# Ubuntu
PERL_VERSION    = $(shell perl -e 'printf ".%vd", $$^V')
LDFLAGS        += -L$(PERL5_LIB)/libperl.$(SHLIB_EXT)$(PERL_VERSION)
endif

COREOBJDIR      = $(FRAMEWORK_DIR)/src/gmsec5
INTERNALOBJDIR  = $(FRAMEWORK_DIR)/src/gmsec5/internal
INTERFACEDIR    = interfaces

PERL_BINDIR     = $(BINDIR)/lib/GMSECAPI5

WRAPPED_SRC     = $(INTERFACEDIR)/libgmsec_perl_wrap.cxx

WRAPPED_OBJ     = $(INTERFACEDIR)/libgmsec_perl_wrap.o

OBJECTS        += $(WRAPPED_OBJ)


default: check_swig message $(WRAPPED_SRC) $(WRAPPED_OBJ) $(BINDIR)/$(TARGET) install


check_swig:
	@if [ -z $(SWIG_HOME) ]; then \
		echo; \
		echo; \
		echo "###########################################################"; \
		echo "#"; \
		echo "# SWIG_HOME is not defined"; \
		echo "#"; \
		echo "# Skipping build of Perl binding for GMSEC API 5.x"; \
		echo "#"; \
		echo "###########################################################"; \
		exit 1; \
	fi


message:
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Building Perl binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"


$(INTERFACEDIR)/%_wrap.cxx: $(INTERFACEDIR)/%.i
	$(SWIG) $(SWIG_OPTS) $(GMSEC_INCS) $<
	@ ./patch.sh


$(INTERFACEDIR)/%.o: $(INTERFACEDIR)/%.cxx
	$(CXX) -c $(API_CXXFLAGS) $< -o $@


$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@


install:
	mkdir -p $(PERL_BINDIR)
	cp $(INTERFACEDIR)/libgmsec_perl.pm $(PERL_BINDIR)


clean:
	find $(INTERFACEDIR) \( -name '*.gcno' -o -name '*.gcda' \) -exec rm -rf {} \;
	find $(INTERFACEDIR) \( -name '*.cxx' -o -name '*.pm' -o -name '*.o' -o -name '*.h' -o -name 'Makefile' -o -name 'help' -o -name 'pm_to_blib' -o -name 'blib' \) -exec rm -rf {} \;
	$(RM) -r $(BINDIR)/$(TARGET) $(PERL_BINDIR)/libgmsec_perl.pm
