# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








GMSEC_HOME = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

DIRS = gmpub gmreq gmreq_cb gmrpl gmrpl_cb \
	gmsub gmsub_cb gmsub_disp gmsub_disp_rr \
	gmconfig gmconfig_edit \
	logging Recorder Replier Requestor \
	gmsub_gmd gmpub_gmd \
	gm_msg_config Threading1 gm_optimized_msg \
	gmreq_openresp gmrpl_openresp gmmist_validation \
	gmmist_services
	

default all:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d || exit 1; \
	done


clean:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d $@; \
		$(RM) $$d/*.gcno $$d/*.gcda; \
	done
