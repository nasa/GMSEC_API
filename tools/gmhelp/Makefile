# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.





GMSEC_HOME =../..
INTERNAL   = 1

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

TARGET   = $(BINDIR)/gmhelp

SRCS     = gmhelp.cpp
HDRS     = $(wildcard *.h)

.PHONY: default clean


default: $(TARGET)

clean:
	$(RM) $(TARGET)
	$(RM) *.gcno *.gcda

$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)
