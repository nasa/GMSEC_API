# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

GMSEC_API_HOME =../..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

INTERNAL = 1

TARGET   = $(BINDIR)/gmhelp

SRCS     = gmhelp.cpp
HDRS     = $(wildcard *.h)

.PHONY: message default clean


default: message $(TARGET)


message:
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Building gmhelp"
	@echo "#"
	@echo "###########################################################"


clean:
	$(RM) $(TARGET)
	$(RM) *.gcno *.gcda


$(TARGET): $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) $< -o $@ $(LDFLAGS)
