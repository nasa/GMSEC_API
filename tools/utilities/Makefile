# Copyright 2007-2024 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.


GMSEC_API_HOME = ../..

include $(GMSEC_API_HOME)/config/$(GMSEC_PLATFORM)

DIRS = gmconfig_edit gmlog gmpub gmreq gmrpl gmsub msg_schema_ids throughput_pub throughput_sub


.PHONY: default all clean


default all:
	@ for d in $(DIRS); do \
		echo ; \
		echo ; \
		echo "###########################################################" ; \
		echo "#" ; \
		echo "#  Building $$d" ; \
		echo "#" ; \
		echo "###########################################################" ; \
		$(MAKE) -C $$d || exit 1; \
	done


clean:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d $@; \
	done
