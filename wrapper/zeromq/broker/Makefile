# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



GMSEC_HOME=../../..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

TARGET = ZMQBroker

SUPDIR		= $(SUPPORT)/zeromq413

LOCAL_INCS	= -I$(SUPDIR)/include

CXXFLAGS        += $(LOCAL_INCS)

LDFLAGS		+= -L$(SUPDIR)/lib -lzmq

default: clean build

clean:
	rm -f $(BINDIR)/$(TARGET)

build: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): SimpleBroker.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

