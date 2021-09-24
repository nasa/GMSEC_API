# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

GMSEC_HOME =../..
INTERNAL   = 1

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

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
