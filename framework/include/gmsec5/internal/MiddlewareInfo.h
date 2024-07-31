/*
 * Copyright 2007-2024 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/**
 * @file MiddlewareInfo.h
 *
 * @brief This file contains the class definition which will be used to store data
 * that can later be used to acknowledge the completion of the processing of a
 * middleware message.
 *
 * Middleware wrappers requiring this feature should use the middleware info from
 * within their implementation of ConnectionInterface::mwAcknowledge().
 */

#ifndef GMSEC_API5_MIDDLEWARE_INFO_H
#define GMSEC_API5_MIDDLEWARE_INFO_H

namespace gmsec
{
namespace api5
{
namespace internal
{

// Forward declarion(s)
class ConnectionInterface;


class MiddlewareInfo
{
public:
    MiddlewareInfo(ConnectionInterface* mwConnIf, void* mwMsg, void* mwOpaque)
        : m_mwConnIf(mwConnIf),
          m_mwMsg(mwMsg),
          m_mwOpaque(mwOpaque)
    {
    }

    ConnectionInterface* m_mwConnIf;   // ConnectionInterface from where m/w message originated
    void*                m_mwMsg;      // Middleware message
    void*                m_mwOpaque;   // Middleware specific object (if necessary)
};

} // namespace internal
} // namespace api5
} // namespace gmsec

#endif
