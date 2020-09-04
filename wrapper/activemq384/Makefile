# Copyright 2007-2018 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


#	Makefile - CMS / ActiveMQ version 3[.8.4]


GMSEC_HOME	 = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

LIBROOTNAME  = gmsec_activemq38

TARGET       = lib$(LIBROOTNAME).$(SHLIB_EXT)

ALT_TARGET   = lib$(LIBROOTNAME)4.$(SHLIB_EXT)

OBJDIR       = ../activemq/src

SUPDIR       = $(SUPPORT)/activemq384

LOCAL_DEFS   = -DACTIVEMQ_CMS_V3=1 -DACTIVEMQ_USE_MSG_RESET=1 -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

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
	$(RM) $(BINDIR)/$(ALT_TARGET)


$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@
	@ln -s $(TARGET) $(BINDIR)/$(ALT_TARGET)


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(API_CXXFLAGS) $< -o $@
