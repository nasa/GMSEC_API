# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#	Makefile - AMQP

GMSEC_API_HOME = ../..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME  = gmsec_amqp

TARGET       = lib$(LIBROOTNAME).$(SHLIB_EXT)

OBJDIR       = ./src

SUPDIR       = $(SUPPORT)/amqp-1.0

LOCAL_DEFS   = -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS   = -I$(GMSEC_API_HOME)/include -I./include -I$(SUPDIR)/include

CXXFLAGS    += $(LOCAL_DEFS) $(LOCAL_INCS)

LDFLAGS     += -L$(SUPDIR)/lib64 -lqpid-proton-cpp

SRCDIR       = src

OBJECTS      = $(OBJDIR)/gmsec_amqp.o \
               $(OBJDIR)/AMQPConnection.o \
               $(OBJDIR)/ProtonReceiver.o \
               $(OBJDIR)/ProtonSender.o


.PHONY: all clean

all: clean library


library: $(BINDIR)/$(TARGET)


$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@


clean:
	$(RM) $(OBJECTS)
	$(RM) $(OBJDIR)/*.gcno $(OBJDIR)/*.gcda
	$(RM) $(BINDIR)/$(TARGET)
