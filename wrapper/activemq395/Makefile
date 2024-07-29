# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


#	Makefile - CMS / ActiveMQ version 3[.9.5]


GMSEC_API_HOME = ../..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME  = gmsec_activemq
MAJOR_VER    = 3
MINOR_VER    = 9
PATCH_VER    = 5

TARGET       = lib$(LIBROOTNAME)$(MAJOR_VER)$(MINOR_VER)$(PATCH_VER).$(SHLIB_EXT)
LINK_TARGET  = lib$(LIBROOTNAME)$(MAJOR_VER)$(MINOR_VER).$(SHLIB_EXT)

OBJDIR       = ../activemq/src

SUPDIR       = $(SUPPORT)/activemq$(MAJOR_VER)$(MINOR_VER)$(PATCH_VER)

LOCAL_DEFS   = -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)$(MAJOR_VER)$(MINOR_VER)$(PATCH_VER)

LOCAL_INCS   = -I$(SUPDIR)/include \
               -I../activemq/include

ifeq ($(findstring macosx,$(GMSEC_PLATFORM)), macosx)
    # MacOS
LOCAL_INCS  += -I/usr/local/opt/apr/include/apr-1
else
ifeq (,$(wildcard /etc/lsb-release))
    # RHEL
LOCAL_INCS  += -I/usr/include/apr-1
else
    # Ubuntu
LOCAL_INCS  += -I/usr/include/apr-1.0
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
	$(RM) $(BINDIR)/$(LINK_TARGET)


$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@
	@ln -s $(TARGET) $(BINDIR)/$(LINK_TARGET)


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@
