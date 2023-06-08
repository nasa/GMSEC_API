# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.

GMSEC_API_HOME = ../..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

TARGET          = gmsec_nodejs.node

NODE            = $(NODE_HOME)/bin/node
NODE_GYP        = $(NODE_HOME)/lib/node_modules/npm/bin/node-gyp-bin/node-gyp

SWIG            = $(SWIG_HOME)/bin/swig
SWIG_VERSION    = $(shell $(SWIG) -version | grep Version | cut -d' ' -f3)
SWIG_INC        = -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)/javascript/v8 -I$(SWIG_HOME)/share/swig/$(SWIG_VERSION)
SWIG_OPTS       = -c++ -javascript -node $(SWIG_INC) -DCHAR_BIT=8 -DGMSEC_S16IL32LL64=1

FRAMEWORK_DIR   = ../../framework

GMSEC_INCS      = -I$(FRAMEWORK_DIR)/include -I/usr/include/c++/$(shell $(CXX) -dumpversion)

INTERFACE_DIR   = interfaces

WRAPPED_SRCS    = $(INTERFACE_DIR)/gmsec_nodejs_wrap.cxx

ifeq ($(findstring macosx,$(GMSEC_PLATFORM)), macosx)
SYMLINK_OPTS    = -s
else
SYMLINK_OPTS    = -rs
endif


.PHONY: check_nodejs check_swig message clean


default: check_nodejs check_swig message $(WRAPPED_SRCS) $(BINDIR)/$(TARGET)


check_nodejs:
	@if [ ! -e $(NODE) ]; then \
        echo; \
        echo; \
        echo "###########################################################"; \
        echo "#"; \
        echo "# NodeJS tools are not installed"; \
        echo "#"; \
        echo "# Skipping build of NodeJS binding for API 5.x"; \
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
		echo "# Skipping build of NodeJS binding for API 5.x"; \
		echo "#"; \
		echo "###########################################################"; \
		exit 1; \
	fi


message:
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Building NodeJS binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"


$(INTERFACE_DIR)/%_wrap.cxx: $(INTERFACE_DIR)/%.i
	$(SWIG) $(SWIG_OPTS) $(GMSEC_INCS) $<
	@ ./patch.sh


$(BINDIR)/$(TARGET):
	$(NODE_GYP) configure
	mkdir -p build/Release/obj.target/include/gmsec5
	mkdir -p build/Release/obj.target/include/gmsec5/c/field
	mkdir -p build/Release/obj.target/include/gmsec5/field
	mkdir -p build/Release/obj.target/include/gmsec5/internal/field
	mkdir -p build/Release/obj.target/include/gmsec5/internal/util
	mkdir -p build/Release/obj.target/include/gmsec5/secure
	mkdir -p build/Release/obj.target/include/gmsec5/util
	mkdir -p build/Release/obj.target/src/gmsec5
	mkdir -p build/Release/obj.target/src/gmsec5/field
	mkdir -p build/Release/obj.target/src/gmsec5/internal/field
	mkdir -p build/Release/obj.target/src/gmsec5/internal/util
	mkdir -p build/Release/obj.target/src/gmsec5/secure
	mkdir -p build/Release/obj.target/src/gmsec5/util
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/include/*.h build/Release/obj.target/include/
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/include/gmsec5/*.h build/Release/obj.target/include/gmsec5/
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/include/gmsec5/c/*.h build/Release/obj.target/include/gmsec5/c
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/include/gmsec5/c/field/*.h build/Release/obj.target/include/gmsec5/c/field
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/include/gmsec5/field/*.h build/Release/obj.target/include/gmsec5/field
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/include/gmsec5/secure/*.h build/Release/obj.target/include/gmsec5/secure
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/include/gmsec5/util/*.h build/Release/obj.target/include/gmsec5/util
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/include/gmsec5/internal/*.h build/Release/obj.target/include/gmsec5/internal
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/include/gmsec5/internal/field/*.h build/Release/obj.target/include/gmsec5/internal/field
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/include/gmsec5/internal/util/*.h build/Release/obj.target/include/gmsec5/internal/util
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/src/gmsec5/*.cpp build/Release/obj.target/src/gmsec5
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/src/gmsec5/field/*.cpp build/Release/obj.target/src/gmsec5/field
	ln $(SYMLINK_OPTS) $(GMSEC_API_HOME)/framework/src/gmsec5/util/*.cpp build/Release/obj.target/src/gmsec5/util
	$(NODE_GYP) build
	cp -p build/Release/$(TARGET) $(BINDIR)


clean:
	$(RM) -r build
	$(RM) $(WRAPPED_SRCS)
	$(RM) $(BINDIR)/$(TARGET)
