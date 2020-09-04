# Copyright 2007-2017 United States Government as represented by the
# Administrator of The National Aeronautics and Space Administration.
# No copyright is claimed in the United States under Title 17, U.S. Code.
# All Rights Reserved.



GMSEC_HOME = ../..

include $(GMSEC_HOME)/config/$(GMSEC_PLATFORM)

DIRS = configfile \
	connection_state \
	device_message \
	heartbeat_service \
	log_message \
	logging \
	message_bins \
	message_field_iterator \
	mnemonic_message \
	msg_field_container \
	open_response \
	product_message \
	publish \
	reply \
	reply_async \
	request \
	request_async \
	resource_service \
	subject_filtering \
	subscribe \
	subscribe_async \
	validation_addendum \
	validation_custom \
	websphere_publish_async


.PHONE: default all clean


default all:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d || exit 1; \
	done


clean:
	@ for d in $(DIRS); do \
		$(MAKE) -C $$d $@; \
	done
