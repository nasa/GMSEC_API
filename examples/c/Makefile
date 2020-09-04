# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



GMSEC_HOME = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

DIRS = gmconfig_c \
       gm_msg_config_c \
       gmmist_services_c \
       gmmist_validation_c \
       gmpub_c \
       gmpub_c_wsmq_async \
       gmreq_c \
       gmreq_c_cb \
       gmrpl_c \
       gmrpl_c_cb \
       gmsub_c \
       gmsub_c_cb \
       gmsub_c_disp \
       gmsub_c_disp_rr \
       logging_c \
       recorder \
       replier \
       requestor


.PHONE: default all clean


default all:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d || exit 1; \
	done


clean:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d $@; \
	done
