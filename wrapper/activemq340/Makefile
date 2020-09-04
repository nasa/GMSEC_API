 
# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


#	Makefile - CMS / ActiveMQ version 3[.4.0]


GMSEC_HOME	= ../..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)


LIBROOTNAME	= gmsec_activemq340

TARGET		= lib$(LIBROOTNAME).$(SHLIB_EXT)

OBJDIR		= ../activemq/src

SUPDIR		= $(SUPPORT)/activemq340

LOCAL_DEFS		= -DACTIVEMQ_CMS_V3=1 -DGMSEC_LIBROOTNAME=$(LIBROOTNAME)

LOCAL_INCS		= \
	-I$(SUPDIR)/include \
	-I$(SUPDIR)/include/apr-1 \
	-I../activemq/include

CXXFLAGS	+= $(LOCAL_DEFS) $(LOCAL_INCS)

LDFLAGS     += -L$(SUPDIR)/lib -lactivemq-cpp -lapr-1 -laprutil-1

SRCDIR		= ../activemq/src

OBJECTS		= \
	$(OBJDIR)/CMSConnection.o \
	$(OBJDIR)/gmsec_activemq.o \
	$(OBJDIR)/CMSMessage.o

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

