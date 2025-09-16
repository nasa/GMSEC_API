# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

#	Makefile - loopback

GMSEC_API_HOME = ../..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME  = gmsec_loopback

TARGET       = lib$(LIBROOTNAME).$(SHLIB_EXT)

OBJDIR       = ./src

LOCAL_DEFS   = -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS   = -I$(GMSEC_API_HOME)/include -I./include

CXXFLAGS    += $(LOCAL_DEFS) $(LOCAL_INCS)

UNAME_S     := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
LDFLAGS     += -lrt
endif

SRCDIR       = src

OBJECTS      = $(OBJDIR)/gmsec_loopback.o \
               $(OBJDIR)/LoopbackConnection.o


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
