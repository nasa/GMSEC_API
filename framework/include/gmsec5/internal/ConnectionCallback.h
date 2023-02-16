/*
 * Copyright 2007-2022 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


#ifndef GMSEC_API5_INTERNAL_CONNECTION_CALLBACK_H
#define GMSEC_API5_INTERNAL_CONNECTION_CALLBACK_H

#include <gmsec5/Callback.h>

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration(s)
	class Connection;
	class Message;

namespace internal
{

class GMSEC_API ConnectionCallback : public Callback
{
public:
	ConnectionCallback(Connection* conn, Callback* userCallback, bool doValidate);

    inline Connection* getConnection() const { return m_conn; }
    inline Callback* getUserCallback() const { return m_userCallback; }
    inline bool doValidate() const { return m_doValidate; }

    virtual void CALL_TYPE onMessage(Connection& conn, const Message& msg);

private:
	Connection* m_conn;
	Callback*   m_userCallback;
	bool        m_doValidate;
};

} // end namespace internal
} // end namespace api5
} // end namespace gmsec


#endif
