 
# Copyright 2007-2022 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


ifndef GMSEC_PLATFORM

default check_support build library mw_wrappers tools env_val java_api csharp_api perl_api python3_api ruby_api patch-mac api_docs clean install:
	@ echo "Error: GMSEC_PLATFORM is not defined."
	@ echo
	@ echo "Please refer to the 'INSTALL.txt' file for reference"
	@ echo "on how to setup your build environment."
	@ echo
	@ false

else

GMSEC_API_HOME  = .
RELEASE_DIR     = ../release
GMSEC_API_DEST := $(RELEASE_DIR)/GMSEC_API

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

.PHONY: check_support build library env_val api_docs mw_wrapper clean install tools


default: check_support build library mw_wrappers tools env_val java_api csharp_api perl_api python3_api ruby_api patch-mac


check_support:
	@ if [ -d $$SUPPORT ]; then \
		echo "Found $$SUPPORT directory..." ; \
	fi
	mkdir -p $(BINDIR)


build:
ifneq (, $(shell which perl))
	CXX="$(CXX)" perl buildscript.pl
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  perl not found"
	@echo "#  Exiting build of the GMSEC API"
	@echo "#"
	@echo "###########################################################"
endif


library:
	$(MAKE) -C framework


java_api:
ifdef JDK_HOME
	$(MAKE) -C java
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  JDK_HOME is not defined"
	@echo "#  Skipping build of Java binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"
endif


csharp_api:
ifdef SWIG_HOME
ifneq (, $(shell which mcs))
	$(MAKE) -C csharp/gmsec5
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  mcs (from Mono Development package) not found"
	@echo "#  Skipping build of C# binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"
endif
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  SWIG_HOME is not defined"
	@echo "#  Skipping build of C# binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"
endif


perl_api:
ifdef SWIG_HOME
ifdef PERL5_LIB
ifdef PERL5_INC
	$(MAKE) -C perl/gmsec5
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  PERL5_INC is not defined, or Perl tools not available"
	@echo "#  Skipping build of Perl binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"
endif
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  PERL5_LIB is not defined, or Perl tools not available"
	@echo "#  Skipping build of Perl binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"
endif
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  SWIG_HOME is not defined"
	@echo "#  Skipping build of Perl binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"
endif


python3_api:
ifdef SWIG_HOME
ifdef PYTHON3_HOME
	$(MAKE) -C python3/gmsec5
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  PYTHON3_HOME is not defined"
	@echo "#  Skipping build of Python3 binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"
endif
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  SWIG_HOME is not defined"
	@echo "#  Skipping build of Python3 binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"
endif


ruby_api:
ifdef SWIG_HOME
	@cd ruby/gmsec5 && $(MAKE) -f MakeRuby
else
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  SWIG_HOME is not defined"
	@echo "#  Skipping build of Ruby binding for GMSEC API 5.x"
	@echo "#"
	@echo "###########################################################"
endif


env_val:
	@echo
	@echo
	@echo "###########################################################"
	@echo "#"
	@echo "#  Setting up GMSEC API Validator"
	@echo "#"
	@echo "###########################################################"
	mkdir -p ./bin/validator
	cp validator/env_validator.sh ./bin/validator
	cp validator/perl_ver.pl ./bin/validator
	cp validator/python_ver.py ./bin/validator
	cp validator/*.env ./bin/validator


tools:
	$(MAKE) -C tools


patch-mac:
	@$(GMSEC_API_HOME)/fix-loader-path.sh


api_docs:
	$(MAKE) -C doxygen


mw_wrappers:
	$(MAKE) -C wrapper


all_examples:
	$(MAKE) -C examples


all_tests:
	$(MAKE) -C tests


clean: clean-gcov
ifneq (, $(shell which perl))
	CXX="$(CXX)" perl buildscript.pl $@
endif
	$(RM) -r $(BINDIR)/lib
	$(RM) -r $(BINDIR)/validator
	@ for dir in framework examples wrapper java csharp/gmsec5 perl/gmsec5 python3/gmsec5 doxygen tests tools; do \
		$(MAKE) -C $$dir $@ ; \
	done
	@ cd ruby/gmsec5 && $(MAKE) -f MakeRuby $@
	$(RM) $(BINDIR)/*
	$(RM) c.ver os.ver java.ver


clean-gcov:
	@ for dir in framework examples tests wrapper; do \
	    find $$dir \( -name '*.gcno' -o -name '*.gcda' \) -exec rm -rf {} \; ; \
	done


install:
	@ echo
	@ echo
	@ echo "###########################################################"
	@ echo "#"
	@ echo "# Installing GMSEC API"
	@ echo "#"
	@ echo "###########################################################"
	mkdir -p $(RELEASE_DIR)
	$(RM) -r $(GMSEC_API_DEST) && mkdir $(GMSEC_API_DEST)
	cp -a README.txt CHANGE-LOG.txt bin config templates examples framework/include $(GMSEC_API_DEST)
	$(RM) -r $(GMSEC_API_DEST)/include/gmsec5/internal

install_docs:
	$(MAKE) install -C doxygen

endif

