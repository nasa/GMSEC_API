# Copyright 2007-2021 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.








GMSEC_HOME=../..
include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)


DIRS = gmpub_c gmreq_c gmreq_c_cb gmrpl_c gmrpl_c_cb \
	gmsub_c gmsub_c_disp gmsub_c_disp_rr gmconfig_c logging_c \
	Recorder Replier Requestor gmpub_c_gmd gmsub_c_gmd \
	gm_msg_config_c gmsub_c_cb


all:
	for d in $(DIRS) ; do (cd $$d && $(MAKE)) ; done


clean:
	for d in $(DIRS) ; do (cd $$d && $(MAKE) clean) ; done
	for d in $(DIRS) ; do (cd $$d && $(RM) *.gcno *.gcda) ; done

