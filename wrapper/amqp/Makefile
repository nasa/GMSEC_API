# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#	Makefile - AMQP

GMSEC_HOME   = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME  = gmsec_amqp

TARGET       = lib$(LIBROOTNAME).$(SHLIB_EXT)

OBJDIR       = ./src

SUPDIR       = $(SUPPORT)/amqp-1.0

LOCAL_DEFS   = -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS   = -I$(GMSEC_HOME)/include \
                -I$(SUPDIR)/include \
                -I./include

CXXFLAGS    += $(LOCAL_DEFS) $(LOCAL_INCS)

LDFLAGS     += -L$(SUPDIR)/lib64 -lqpid-proton

SRCDIR       = src

OBJECTS      = $(OBJDIR)/gmsec_amqp.o \
               $(OBJDIR)/AMQPConnection.o


default: clean library

library: $(BINDIR)/$(TARGET)

clean:
	$(RM) $(OBJECTS)
	$(RM) $(OBJDIR)/*.gcda $(OBJDIR)/*.gcno
	$(RM) $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@
	@mkdir -p $(BINDIR)/amqp
	@cp amqp.xml $(BINDIR)/amqp

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@

