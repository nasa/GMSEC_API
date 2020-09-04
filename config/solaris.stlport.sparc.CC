# Copyright 2007-2017 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

# Solaris sparc build configuration for GMSEC_API using Sun CC but w/ the stlport extension

# load the default macros
include $(GMSEC_HOME)/config/default

BINDIR    = $(GMSEC_HOME)/bin

# platform location of Java headers for JNI
JAVA_INCS = -I$(JDK_HOME)/include -I$(JDK_HOME)/include/solaris

# the binary architecture tag
ARCH_DIR  = solaris/sparc

CXX	      = CC
CC        = cc

PERL_CC   = $(CXX)
LINK      = $(CXX)
DLINK     = $(CXX) -G
SHLIB_EXT = so

INCS      = -I$(GMSEC_HOME)/include \
            -I$(GMSEC_HOME)/framework/include \
            -I$(GMSEC_HOME)/framework/support/include

STL_PORT = -library=stlport4

# define C/C++ compiler flags
ifneq ($(DEBUG_FLAGS),)
DEBUG_FLAGS = -g
endif

CXXFLAGS = $(STL_PORT) $(DEBUG_FLAGS) $(INCS)
CFLAGS   = $(DEBUG_FLAGS) $(INCS)

# define api/example/test applications LDFLAGS
LDFLAGS  = -L$(BINDIR) -lGMSECAPI -ldl -lpthread -lm -lsocket -lnsl

# define API API_CXXFLAGS and API_LDFLAGS
API_CXXFLAGS = -mt -PIC $(CXXFLAGS)
API_LDFLAGS  = $(STL_PORT) -ldl -lpthread -lposix4 -lm -lsocket -lnsl
