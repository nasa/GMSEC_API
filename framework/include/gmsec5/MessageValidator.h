/*
 * Copyright 2007-2023 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */


/** @file MessageValidator.h
 *
 *  @brief This file contains the abstract base class for defining a custom message validator.
 */

#ifndef GMSEC_API5_MESSAGE_VALIDATOR_H
#define GMSEC_API5_MESSAGE_VALIDATOR_H

#include <gmsec5/util/wdllexp.h>


namespace gmsec
{
namespace api5
{
	// Forward declaration
	class Message;
	class Status;


/**
 * @class MessageValidator
 *
 * @brief This class is the abstract base class for performing custom message validation.
 * A user created class, derived from this class, can be passed into
 * %registerMessageValidator() to have user code executed when a message is being validated.
 *
 * This custom message validator will be used to supplement the already built-in message validator
 * offered by the Connection (via a Specification object). The Connection will first
 * call on the custom message validator, and if the returned status is nominal, then the API's
 * message validator will be called.
 *
 * Note that because users are able to create their own MessageValidator class,
 * reentrancy is not guaranteed unless if reentrancy rules are specified.
 *
 * In addition, if a MessageValidator is registered to multiple Message objects,
 * validateMessage() can be invoked concurrently from different threads. Use of a AutoMutex
 * is suggested to enforce thread safety.
 *
 * Example MessageValidator class:
 * @code
   class CustomMessageValidator : public MessageValidator
   {
   public:
       virtual Status validateMessage(const Message& msg)
       {
           Status status;

           gmsec::api5::util::AutoMutex lock(myMutex);
  
           // Validate message

           return status;
       }
  
   private:
       gmsec::api5::util::Mutex myMutex;
   };
 * @endcode
 *
 * Example callback registration:
 * @code
   int main(int argc, char** argv)
   {
       CustomMessageValidator* validator = new CustomMessageValidator();
       try {
           Config config("gmsec-msg-content-validate-all=true");
           MessageFactory factory(config);
           factory.registerMessageValidator(validator);
           Message msg = factory.createMessage("HB");
            ...
           if (msg.isCompliant().hasError()) {
               // message is not valid/compliant
           }
       }
       catch (const GmsecException& e) {
           cerr << e.what() << endl;
       }
   }
 * @endcode
*/
class GMSEC_API MessageValidator
{
public:
	// base class needs virtual destructor
	virtual ~MessageValidator() { }


	/**
	 * @fn validateMessage(const Message& msg)
	 *
	 * @brief This method is called by the API to perform a user-defined message validation.
	 *
	 * If a MessageValidator is registered to multiple connections, validateMessage() can be
	 * invoked concurrently from different connection threads.
	 *
	 * The prototype for this method is:
	 * @code virtual Status validateMessage(const Message& msg) @endcode
	 *
	 * This custom message validator will be used to supplement the already built-in message validator
	 * offered by the Connection (via a Specification object). The Connection will first
	 * call on the custom message validator, and if the returned status is nominal, then the API's
	 * message validator will be called.
	 *
	 * @note <b>DO NOT STORE</b> the Message object for use beyond the scope of the function.
	 *
	 * @param msg - the message to validate
	 *
	 * @returns The Status of the operation.
	 *
	 * @sa Connection::registerMessageValidator()
	 * @sa Specification::registerMessageValidator()
	 */
	virtual Status CALL_TYPE validateMessage(const Message& msg) = 0;
};

} // end namespace api5
} // end namespace gmsec

#endif
