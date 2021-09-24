# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

GMSEC_HOME = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

TARGET	        = libgmsec_csharp.so 

ASSEMBLY        = gmsec_csharp.dll

RELEASE_DIR     = $(GMSEC_HOME)/../release/GMSEC_API/bin

MCS             = /usr/bin/mcs
SWIG            = $(SWIG_HOME)/bin/swig
SWIG_VERSION    = $(shell $(SWIG) -version | grep Version | cut -d' ' -f3)
SWIG_INC        = -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)/csharp -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)
SWIG_NAMESPACE  = GMSEC.API
SWIG_OPTS       = -c++ -csharp -namespace $(SWIG_NAMESPACE) $(SWIG_INC) -DCHAR_BIT=8 -DGMSEC_S16IL32LL64=1

FRAMEWORK_DIR   = ../../framework

GMSEC_INCS      = -I$(FRAMEWORK_DIR)/include -I/usr/include/c++/$(shell $(CXX) -dumpversion)

INTERFACE_DIR   = interfaces

WRAPPED_SRCS    = $(INTERFACE_DIR)/Gmsec_wrap.cxx
WRAPPED_OBJS    = $(WRAPPED_SRCS:.cxx=.o)

OBJECTS        += $(WRAPPED_OBJS)

.PHONY: clean check_mono check_swig message


default: check_mono check_swig message $(WRAPPED_SRCS) $(WRAPPED_OBJS) $(BINDIR)/$(TARGET)


check_mono:
	@if [ ! -e $(MCS) ]; then \
		echo; \
		echo; \
		echo "###########################################################"; \
		echo "#"; \
		echo "# Mono development tools are not installed"; \
		echo "#"; \
		echo "# Skipping build of Csharp binding for API 4.x"; \
		echo "#"; \
		echo "###########################################################"; \
		exit 1; \
	fi


check_swig:
	@if [ -z $(SWIG_HOME) ]; then \
		echo; \
		echo; \
		echo "###########################################################"; \
		echo "#"; \
		echo "# SWIG_HOME is not defined"; \
		echo "#"; \
		echo "# Skipping build of Csharp binding for API 4.x"; \
		echo "#"; \
		echo "###########################################################"; \
		exit 1; \
	fi


message:
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Building Csharp binding of the GMSEC API"
	@echo "#"
	@echo "###########################################################"


$(INTERFACE_DIR)/%_wrap.cxx: $(INTERFACE_DIR)/%.i
	$(SWIG) $(SWIG_OPTS) $(GMSEC_INCS) $<
	@ ./patch.sh
	$(MCS) -t:library $(INTERFACE_DIR)/*.cs -out:$(BINDIR)/$(ASSEMBLY)


$(INTERFACE_DIR)/%.o: $(INTERFACE_DIR)/%.cxx
	@ ./nowarnings.sh
	$(CXX) -c $(API_CXXFLAGS) -Wno-maybe-uninitialized $< -o $@


$(BINDIR)/$(TARGET): $(OBJECTS)
	$(DLINK) $(OBJECTS) $(LDFLAGS) -o $@
	@ ./cleanup.sh


clean:
	@ ./cleanup.sh
	$(RM) $(BINDIR)/$(TARGET) $(BINDIR)/$(ASSEMBLY)
