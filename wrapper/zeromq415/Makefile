# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#	Makefile - ZeroMQ415

GMSEC_HOME   = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME  = gmsec_zeromq415

TARGET       = lib$(LIBROOTNAME).$(SHLIB_EXT)

OBJDIR       = ../zeromq/src

SUPDIR       = $(SUPPORT)/zeromq415

LOCAL_DEFS   = -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS   = -I$(SUPDIR)/include -I../zeromq/include

CXXFLAGS    += $(LOCAL_DEFS) $(LOCAL_INCS)

LDFLAGS     += -L$(SUPDIR)/lib -lzmq

SRCDIR       = ../zeromq/src

OBJECTS      = $(OBJDIR)/gmsec_zeromq.o \
               $(OBJDIR)/ZMQConnection.o \
               $(OBJDIR)/ZMQMessageListener.o


default: clean library

library: $(BINDIR)/$(TARGET)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(OBJDIR)/*.gcda $(OBJDIR)/*.gcno
	$(RM) $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@
