# Copyright 2007-2016 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



GMSEC_HOME = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

DIRS = gmconfig  gmconfig_edit  gmmist_services  gmmist_validation  \
       gm_msg_config  gm_optimized_msg  gmpub gmpub_wsmq_async  gmreq \
       gmreq_cb gmreq_openresp gmrpl gmrpl_cb gmrpl_openresp gmsub \
       gmsub_cb gmsub_disp gmsub_disp_rr logging recorder replier \
       requestor gmthreads


.PHONE: default all clean


default all:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d || exit 1; \
	done


clean:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d $@; \
	done
