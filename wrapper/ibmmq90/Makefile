# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


#	Makefile - IBM MQ 9.0


GMSEC_HOME	= ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)


LIBVER		= 90
LIBROOTNAME	= gmsec_ibmmq$(LIBVER)

TARGET		= lib$(LIBROOTNAME).$(SHLIB_EXT)

SUPDIR		= $(SUPPORT)/ibmmq$(LIBVER)

GLUE		= ../websphere

LOCAL_DEFS	= -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS	= -I$(SUPDIR)/include -I$(GLUE)/include

CXXFLAGS	+= $(LOCAL_DEFS) $(LOCAL_INCS)

LDFLAGS		+= -L$(SUPDIR)/lib64 -lmqic_r

SRCDIR		= $(GLUE)/src
OBJDIR		= ../websphere/src

OBJECTS     = $(OBJDIR)/WebSConnection.o \
              $(OBJDIR)/gmsec_websphere.o


.PHONY: default library clean


# Need to call on 'clean' so that we regenerate object files.
#
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
