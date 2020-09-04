# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.





GMSEC_HOME=../../..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

TARGET = gmmist_services


default: $(BINDIR)/$(TARGET)

clean:
	rm -f $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): gmmist_services.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

