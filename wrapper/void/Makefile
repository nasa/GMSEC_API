 
# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.






#	Makefile - middleware wrapper template


GMSEC_HOME	= ../..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)


LIBROOTNAME	= gmsec_void

TARGET		= lib$(LIBROOTNAME).$(SHLIB_EXT)

OBJDIR		= ./src

LOCAL_DEFS		= -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS		= \
	-Iinclude

CXXFLAGS	+= $(LOCAL_DEFS) $(LOCAL_INCS)

LIBDIRS		=

LIBS		=

#
SRCDIR		= src

OBJECTS		= \
	$(OBJDIR)/gmsec_void.o \
	$(OBJDIR)/VoidConnection.o \
	$(OBJDIR)/VoidMessage.o


# default: clean library
default: library

library: $(BINDIR)/$(TARGET)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(BASE_LDFLAGS) $(OBJECTS) $(LIBDIRS) $(LIBS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

