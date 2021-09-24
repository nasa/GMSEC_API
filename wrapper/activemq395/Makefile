# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


#	Makefile - CMS / ActiveMQ version 3[.9.5]


GMSEC_HOME	 = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME  = gmsec_activemq

MAJOR_VER    = 3
MINOR_VER    = 9
PATCH_VER    = 5

TARGET       = lib$(LIBROOTNAME)$(MAJOR_VER)$(MINOR_VER)$(PATCH_VER).$(SHLIB_EXT)
LINK_TARGET  = lib$(LIBROOTNAME)$(MAJOR_VER)$(MINOR_VER).$(SHLIB_EXT)


OBJDIR       = ../activemq/src

SUPDIR       = $(SUPPORT)/activemq$(MAJOR_VER)$(MINOR_VER)$(PATCH_VER)

LOCAL_DEFS   = -DACTIVEMQ_CMS_V3=1 -DACTIVEMQ_USE_MSG_RESET=1 -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)$(MAJOR_VER)$(MINOR_VER)$(PATCH_VER)

LOCAL_INCS   = -I$(SUPDIR)/include \
               -I$(SUPDIR)/include/apr-1 \
               -I../activemq/include

CXXFLAGS    += $(LOCAL_DEFS) $(LOCAL_INCS)

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
	$(RM) $(BINDIR)/$(LINK_TARGET)


$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@
	@ln -s $(TARGET) $(BINDIR)/$(LINK_TARGET)


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@
