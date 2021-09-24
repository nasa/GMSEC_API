 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


#	Makefile - Generic JMS


GMSEC_HOME   = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME  = gmsec_generic_jms

TARGET       = lib$(LIBROOTNAME).$(SHLIB_EXT)

OBJDIR       = ./src

LOCAL_DEFS   = -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS   = -I./include \
               $(JAVA_INCS)
	
CXXFLAGS    += $(LOCAL_DEFS) $(LOCAL_INCS)

LDFLAGS     += $(JAVA_LIBS)

SRCDIR       = ./src

OBJECTS      = $(OBJDIR)/gmsec_generic_jms.o \
               $(OBJDIR)/JMS.o \
               $(OBJDIR)/JMSConnection.o \
               $(OBJDIR)/JNI.o


default: check-env clean library

check-env:
ifndef JDK_HOME
	$(error JDK_HOME is not defined; GMSEC API Generic JMS wrapper will not be built)
endif

library: $(BINDIR)/$(TARGET)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(OBJDIR)/*.gcda $(OBJDIR)/*.gcno
	$(RM) $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) $(JLINK_FLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@
