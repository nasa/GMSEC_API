# Copyright 2007-2023 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



GMSEC_API_HOME=..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

default: 
	@for wrapper in $(WRAPPERS) ; do \
		echo ; \
		echo ; \
		echo "###########################################################" ; \
		echo "#" ; \
		echo "#  Building $$wrapper" ; \
		echo "#" ; \
		echo "###########################################################" ; \
		( cd $$wrapper && $(MAKE) ) \
	done

clean: 
	@for wrapper in $(WRAPPERS) ; do \
		( cd $$wrapper && $(MAKE) clean ) \
	done

