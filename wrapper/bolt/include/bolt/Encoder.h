
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_Encoder_h
#define bolt_Encoder_h

#include <bolt/Types.h>

#include <string.h>


namespace bolt {


/**	@class Encoder
 *
 *	@brief This class is responsible for converting data into different formats
 *
**/
class Encoder
{
public:
	/**	@fn get()
	 *	@brief Returns a pointer to the singleton encoder object for this platform.
	**/
	static Encoder &get();


	virtual ~Encoder();

	bool isValid() const;

	/**	@fn getProblem() const
	 *
	 *	@brief Returns the problem string
	 *
	**/
	string getProblem() const;

	/** @fn put
	* @brief Puts a raw byte array.
	**/
	void put(const u8 *pi, i32 count, u8 *po) const;


	/** @fn put1
	* @brief Puts the least significant byte of x to po in network order.
	**/
	virtual void put1(i32 x, u8 *po) const = 0;

	/** @fn put2
	* @brief Puts the least significant bytes of x to po in network order.
	**/
	virtual void put2(i32 x, u8 *po) const = 0;

	/** @fn put3
	* @brief Puts the least significant bytes of x to po in network order.
	**/
	virtual void put3(i32 x, u8 *po) const = 0;

	/** @fn put4
	* @brief Puts the least significant bytes of x to po in network order.
	**/
	virtual void put4(i32 x, u8 *po) const = 0;


	/** @fn put
	* @brief Puts x to po in network order.
	**/
	virtual void putr8(f64 x, u8 *po) const = 0;



protected:

	Encoder();

	void setProblem(const string &text);


private:

	Encoder(const Encoder &other); // not implemented
	Encoder &operator=(const Encoder &other); // not implemented

	string problem;
};


} // namespace bolt


#endif /* bolt_Encoder_h */

