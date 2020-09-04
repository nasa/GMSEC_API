# Copyright 2007-2017 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



GMSEC_HOME = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

DIRS = publish subscribe subscribe_async \
       request reply request_async reply_async open_response \
       configfile logging msg_field_container \
       subject_filtering message_bins connection_state \
       websphere_publish_async log_message heartbeat_service \
       resource_service mnemonic_message device_message \
       product_message validation_addendum validation_custom

.PHONE: default all clean


default all:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d || exit 1; \
	done


clean:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d $@; \
	done
