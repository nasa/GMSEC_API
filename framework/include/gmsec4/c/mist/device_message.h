/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file device_message.h
 *
 *  @note This header file has been deprecated.  Use gmsec4/c/message/device_message.h instead.
 *
 *  @brief This file contains functions for the management of a MIST DeviceMessage object.
 *  The DeviceMessage is a specialization, or extension, of the standard GMSEC Message.
 *
 * Example creation and use:
 * @code
 * GMSEC_Status  status    = statusCreate();
 * GMSEC_Device  device    = deviceCreate("MyDevice", GMSEC_NOMINAL_GREEN, status);
 * GMSEC_Message deviceMsg = deviceMessageCreate("CriticalDevices", C_GMSEC_ISD_CURRENT, status);
 *
 * if (device && deviceMsg)
 * {
 *     messageAddStringField(deviceMsg, "SYSTEM-STATUS", "NOMINAL", status);
 *
 *     deviceMessageAddDevice(deviceMsg, device, status);
 *     deviceDestroy(&device);
 *
 *     connectionManagerPublish(connMgr, deviceMsg, status);
 *
 *     messageDestroy(&deviceMsg);
 * }
 * else
 * {
 *     printf("Error -- %s\n", statusGet(status));
 * }
 * @endcode
 */


#ifndef GMSEC_API_C_DEVICE_MESSAGE_H
#define GMSEC_API_C_DEVICE_MESSAGE_H


#include <gmsec4/c/mist/message/device_message.h>


#endif
