# Copyright 2007-2017 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

GMSEC_HOME = ../..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

# Note that in order to run Python scripts, the output library file needs the preceeding underscore
# Python can only interpret .so files (this is not an issue on mac)
TARGET	        = _libgmsec_python.so 

SWIG            = $(SWIG_HOME)/bin/swig
SWIG_VERSION    = $(shell $(SWIG) -version | grep Version | cut -d' ' -f3)
SWIG_SWG        = -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)/python -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)
SWIG_OPTS       = -c++ -python -threads -classic $(SWIG_SWG)

ifeq ($(findstring macosx,$(GMSEC_PLATFORM)), macosx)
SWIG_OPTS      += -DCHAR_BIT=8 -DGMSEC_S16IL32LL64=1
endif

FRAMEWORK_DIR   = ../../framework

GMSEC_INCS      = -I$(FRAMEWORK_DIR)/include -I/usr/include
PYTHON_VERSION  = $(shell python --version 2>&1 | cut -d' ' -f2 | awk 'BEGIN{FS="."}{print $$1"."$$2}')
PYTHON_INCS     = -I$(PYTHON_INC)$(PYTHON_VERSION)
API_CXXFLAGS   += $(PYTHON_INCS)

# TODO: Do we need any special handling here for Mac OSX? (i.e. PYTHON_LIB set to the directory where -lpython2.X is held)
LDFLAGS        += -lpython$(PYTHON_VERSION)

COREOBJDIR      = $(FRAMEWORK_DIR)/src/gmsec4
INTERNALOBJDIR  = $(FRAMEWORK_DIR)/src/gmsec4/internal
INTERFACEDIR    = interfaces

PYTHON4_BINDIR    = $(BINDIR)/lib/GMSECAPI4

WRAPPED_SRC     = $(INTERFACEDIR)/libgmsec_python_wrap.cxx

WRAPPED_OBJ     = $(INTERFACEDIR)/libgmsec_python_wrap.o

OBJECTS        += $(WRAPPED_OBJ)


default: $(WRAPPED_SRC) $(WRAPPED_OBJ) $(BINDIR)/$(TARGET) install


$(INTERFACEDIR)/%_wrap.cxx: $(INTERFACEDIR)/%.i
	$(SWIG) $(SWIG_OPTS) $(GMSEC_INCS) $<
	@ ./patch.sh


$(INTERFACEDIR)/%.o: $(INTERFACEDIR)/%.cxx
	$(CXX) -c $(API_CXXFLAGS) $< -o $@


$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@


install:
	mkdir -p $(PYTHON4_BINDIR)
	cp $(INTERFACEDIR)/*.py $(PYTHON4_BINDIR)


clean:
	find $(INTERFACEDIR) \( -name '*.cxx' -o -name '*.py' -o -name '*.o' -o -name '*.h' -o -name 'Makefile' -o -name 'help' -o -name 'pm_to_blib' -o -name 'blib' \) -exec rm -rf {} \;
	rm -rf $(BINDIR)/$(TARGET) $(PYTHON4_BINDIR)
