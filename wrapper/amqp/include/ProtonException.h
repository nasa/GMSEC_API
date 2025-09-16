/*
 * Copyright 2007-2025 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */

#ifndef AMQP_PROTON_EXCEPTION_H
#define AMQP_PROTON_EXCEPTION_H


#include <stdexcept>


namespace gmsec_amqp
{

// Exception raised if a sender or receiver is closed when trying to send/receive
class ProtonException : public std::runtime_error
{
public:
	ProtonException(const std::string& msg) : std::runtime_error(msg) {}
};

}

#endif
