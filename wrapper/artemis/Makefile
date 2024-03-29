# Copyright 2007-2023 United States Government as represented by the
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

LOCAL_DEFS   = -DACTIVEMQ_CMS_V3=1 -DACTIVEMQ_USE_MSG_RESET=1 -DGMSEC_LIBROOTNAME=$(LIBROOTNAME) -DGMSEC_ARTEMIS

LOCAL_INCS   = -I$(SUPDIR)/include \
               -I$(SUPDIR)/include/apr-1 \
               -I../activemq/include

CXXFLAGS    += $(LOCAL_DEFS) $(LOCAL_INCS) -Wno-deprecated-declarations

LDFLAGS     += -L$(SUPDIR)/lib -lactivemq-cpp -lapr-1

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
