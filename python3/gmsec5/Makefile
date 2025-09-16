# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

GMSEC_API_HOME = ../..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

# Note that in order to run Python scripts, the output library file needs the preceeding underscore
# Python can only interpret .so files (this is not an issue on mac)
TARGET	        = _libgmsec_python3.so 

SWIG            = $(SWIG_HOME)/bin/swig
SWIG_VERSION    = $(shell $(SWIG) -version | grep Version | cut -d' ' -f3)
SWIG_SWG        = -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)/python -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)
SWIG_OPTS       = -c++ -python -threads $(SWIG_SWG) -DCHAR_BIT=8 -DSWIG_BINDING

FRAMEWORK_DIR   = ../../framework

GMSEC_INCS      = -I$(FRAMEWORK_DIR)/include -I$(CXX_INCS)

version_full   := $(wordlist 2,4,$(subst ., ,$(shell $(PYTHON3_HOME)/bin/python3 --version 2>&1)))
version_major  := $(word 1,$(version_full))
version_minor  := $(word 2,$(version_full))

API_CXXFLAGS   += $(shell $(PYTHON3_HOME)/bin/python3-config --includes)
LDFLAGS        += $(shell $(PYTHON3_HOME)/bin/python3-config --ldflags)

ifeq ($(findstring macosx,$(GMSEC_PLATFORM)), macosx)
LDFLAGS        += -L$(PYTHON3_HOME)/lib
LDFLAGS        += -lpython$(version_major).$(version_minor)
endif

COREOBJDIR      = $(FRAMEWORK_DIR)/src/gmsec5
INTERNALOBJDIR  = $(FRAMEWORK_DIR)/src/gmsec5/internal
INTERFACEDIR    = interfaces

PYTHON_BINDIR   = $(BINDIR)/lib/GMSECAPI5

WRAPPED_SRC     = $(INTERFACEDIR)/libgmsec_python3_wrap.cxx

WRAPPED_OBJ     = $(INTERFACEDIR)/libgmsec_python3_wrap.o

OBJECTS        += $(WRAPPED_OBJ)


default: check_swig check_python message $(WRAPPED_SRC) $(WRAPPED_OBJ) $(BINDIR)/$(TARGET) install


check_swig:
	@if [ -z $(SWIG_HOME) ]; then \
		echo; \
		echo; \
		echo "###########################################################"; \
		echo "#"; \
		echo "# SWIG_HOME is not defined"; \
		echo "#"; \
		echo "# Skipping build of Python3 binding for API 5.x"; \
		echo "#"; \
		echo "###########################################################"; \
		exit 1; \
	fi


check_python:
	@if [ -z $(PYTHON3_HOME) ]; then \
		echo; \
		echo; \
		echo "###########################################################"; \
		echo "#"; \
		echo "# PYTHON3_HOME is not defined"; \
		echo "#"; \
		echo "# Skipping build of Python3 binding for API 5.x"; \
		echo "#"; \
		echo "###########################################################"; \
		exit 1; \
	fi


message:
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Building Python3 binding for GMSEC API 5.x"
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
	mkdir -p $(PYTHON_BINDIR)
	cp $(INTERFACEDIR)/*.py $(PYTHON_BINDIR)


clean:
	find $(INTERFACEDIR) \( -name '*.gcno' -o -name '*.gcda' \) -exec rm -rf {} \;
	find $(INTERFACEDIR) \( -name '*.cxx' -o -name '*.py' -o -name '*.o' -o -name '*.h' -o -name 'Makefile' -o -name 'help' -o -name 'pm_to_blib' -o -name 'blib' \) -exec rm -rf {} \;
	$(RM) -f $(BINDIR)/$(TARGET) $(PYTHON_BINDIR)/libgmsec_python3.py
