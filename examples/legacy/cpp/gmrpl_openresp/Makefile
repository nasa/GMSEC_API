# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.





GMSEC_HOME=../../..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

TARGET = gmrpl_openresp

default: $(BINDIR)/$(TARGET)

clean:
	$(RM) $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): gmrpl_openresp.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

