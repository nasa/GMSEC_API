# Copyright 2007-2018 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


#	Makefile - WebSphere MQ 8.0


GMSEC_HOME	= ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)


LIBVER		= 80
LIBROOTNAME	= gmsec_websphere$(LIBVER)

TARGET		= lib$(LIBROOTNAME).$(SHLIB_EXT)

SUPDIR		= $(SUPPORT)/websphere$(LIBVER)

GLUE		= ../websphere

LOCAL_DEFS	= -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS	= -I$(SUPDIR)/include -I$(GLUE)/include

CXXFLAGS	+= $(LOCAL_DEFS) $(LOCAL_INCS)

LDFLAGS		+= -L$(SUPDIR)/lib -lmqic_r

SRCDIR		= $(GLUE)/src
OBJDIR		= ../websphere/src

OBJECTS     = $(OBJDIR)/WebSConnection.o \
              $(OBJDIR)/gmsec_websphere.o


.PHONY: default library clean


default: clean library


library: $(BINDIR)/$(TARGET)


clean:
	$(RM) $(OBJECTS)
	$(RM) $(OBJDIR)/*.gcno $(OBJDIR)/*.gcda
	$(RM) $(BINDIR)/$(TARGET)


$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@
