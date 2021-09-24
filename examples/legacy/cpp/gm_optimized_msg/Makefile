# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.






GMSEC_HOME=../../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

TARGET   = gm_opt_msg
CFG_FILE = gm_opt_msg.xml


all default: $(BINDIR)/$(TARGET)

clean:
	$(RM) $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): gm_opt_msg.cpp
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)

run:
	@ $(BINDIR)/$(TARGET) $(CFG_FILE)
