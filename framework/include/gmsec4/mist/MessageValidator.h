/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */



/** @file MessageValidator.h
 *
 *  @brief This file contains the abstract base class for defining a custom message validator.
 */

#ifndef MESSAGE_VALIDATOR_H
#define MESSAGE_VALIDATOR_H

#include <gmsec4/util/wdllexp.h>


namespace gmsec
{
namespace api
{
// Forward declaration
class Message;
class Status;

namespace mist
{

/**
 * @class MessageValidator
 *
 * @brief This class is the abstract base class for performing custom message validation.
 * A user created class, derived from this class, can be passed into
 * %registerMessageValidator() to have user code executed when a message is being validated.
 *
 * This custom message validator will be used to supplement the already built-in message validator
 * offered by the ConnectionManager (via a Specification object). The ConnectionManager will first
 * call on the custom message validator, and if the returned status is nominal, then the API's
 * message validator will be called.
 *
 * Note that because users are able to create their own MessageValidator class,
 * reentrancy is not guaranteed unless if reentrancy rules are specified.
 *
 * In addition, if a MessageValidator is registered to multiple ConnectionManager objects,
 * validateMessage() can be invoked concurrently from different threads. Use of a gmsec::util::AutoMutex
 * is suggested to enforce thread safety.
 *
 * Example MessageValidator class:
 * @code
   class CustomMessageValidator : public MessageValidator
   {
   public:
       virtual Status CALL_TYPE validateMessage(const Message& msg)
       {
           Status status;

           gmsec::util::AutoMutex lock(myMutex);
  
           // Validate message

           return status;
       }
  
   private:
       gmsec::util::Mutex myMutex;
   };
 * @endcode
 *
 * Example callback registration:
 * @code
   int main(int argc, char** argv)
   {
       CustomMessageValidator* validator = new CustomMessageValidator();
       try {
           Config config(argc, argv);
           config.addValue("gmsec-msg-content-validate-all", "true");
           ConnectionManager connMgr(config);
           connMgr.registerMessageValidator(validator);
            ...
           connMgr.publish(msg);
       }
       catch (const Exception& e) {
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
	 * @code virtual Status CALL_TYPE validateMessage(const Message& msg) @endcode
	 * The CALL_TYPE macro is required and is there to handle a compatibility
	 * problem with the Windows linker.
	 *
	 * This custom message validator will be used to supplement the already built-in message validator
	 * offered by the ConnectionManager (via a Specification object). The ConnectionManager will first
	 * call on the custom message validator, and if the returned status is nominal, then the API's
	 * message validator will be called.
	 *
	 * @note <b>DO NOT STORE</b> the Message object for use beyond the scope of the function.
	 *
	 * @param msg - the message to validate
	 *
	 * @returns The Status of the operation.
	 *
	 * @sa ConnectionManager::registerMessageValidator()
	 * @sa Specification::registerMessageValidator()
	 */
	virtual Status CALL_TYPE validateMessage(const Message& msg) = 0;
};

} // namespace mist
} // namespace api 
} // namespace gmsec

#endif
