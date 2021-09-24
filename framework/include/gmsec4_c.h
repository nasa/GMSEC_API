/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/**  @file gmsec4_c.h
 *
 *  @brief This file contains includes for all required header files for the C binding of API 4.x
 */

#ifndef GMSEC4_C_H
#define GMSEC4_C_H

#include <gmsec4_defs.h>

#include <gmsec4/c/config_file.h>
#include <gmsec4/c/config_file_iterator.h>
#include <gmsec4/c/config.h>
#include <gmsec4/c/connection.h>
#include <gmsec4/c/errors.h>
#include <gmsec4/c/fields.h>
#include <gmsec4/c/heartbeat_generator.h>
#include <gmsec4/c/message.h>
#include <gmsec4/c/message_field_iterator.h>
#include <gmsec4/c/status.h>
#include <gmsec4/c/subscription_info.h>

#include <gmsec4/c/mist/connection_manager.h>
#include <gmsec4/c/mist/device.h>
#include <gmsec4/c/mist/device_param.h>
#include <gmsec4/c/mist/message_specification.h>
#include <gmsec4/c/mist/mist_defs.h>
#include <gmsec4/c/mist/mnemonic.h>
#include <gmsec4/c/mist/mnemonic_sample.h>
#include <gmsec4/c/mist/product_file.h>
#include <gmsec4/c/mist/schema_id_iterator.h>
#include <gmsec4/c/mist/service_param.h>
#include <gmsec4/c/mist/specification.h>

#include <gmsec4/c/mist/message/device_message.h>
#include <gmsec4/c/mist/message/mist_message.h>
#include <gmsec4/c/mist/message/mnemonic_message.h>
#include <gmsec4/c/mist/message/product_file_message.h>

#include <gmsec4/c/util/log.h>
#include <gmsec4/c/util/timeutil.h>

#endif
