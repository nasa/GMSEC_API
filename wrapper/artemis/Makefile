# Copyright 2007-2025 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


#	Makefile - Artemis


GMSEC_API_HOME = ../..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME  = gmsec_artemis

MAJOR_VER    = 3
MINOR_VER    = 9
PATCH_VER    = 5

TARGET       = lib$(LIBROOTNAME).$(SHLIB_EXT)
LINK_TARGET  = lib$(LIBROOTNAME).$(SHLIB_EXT)


OBJDIR       = ../activemq/src

SUPDIR       = $(SUPPORT)/activemq$(MAJOR_VER)$(MINOR_VER)$(PATCH_VER)

LOCAL_DEFS   = -DGMSEC_LIBROOTNAME=$(LIBROOTNAME) -DGMSEC_ARTEMIS

LOCAL_INCS   = -I$(SUPDIR)/include \
               -I../activemq/include

ifeq ($(findstring macosx.x86_64,$(GMSEC_PLATFORM)), macosx.x86_64)
    # MacOS
LOCAL_INCS  += -I/usr/local/opt/apr/include/apr-1
else
ifeq ($(findstring macosx.aarch64,$(GMSEC_PLATFORM)), macosx.aarch64)
    # MacOS
LOCAL_INCS  += -I/opt/homebrew/opt/apr/include/apr-1
else
ifeq (,$(wildcard /etc/lsb-release))
    # RHEL
LOCAL_INCS  += -I/usr/include/apr-1
else
    # Ubuntu
LOCAL_INCS  += -I/usr/include/apr-1.0
endif
endif
endif

CXXFLAGS    += $(LOCAL_DEFS) $(LOCAL_INCS) -Wno-deprecated-declarations

LDFLAGS     += -L$(SUPDIR)/lib -lactivemq-cpp

SRCDIR       = ../activemq/src

OBJECTS      = $(OBJDIR)/gmsec_activemq.o \
               $(OBJDIR)/CMSConnection.o \
               $(OBJDIR)/CMSDestination.o

.PHONY: default clean


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
