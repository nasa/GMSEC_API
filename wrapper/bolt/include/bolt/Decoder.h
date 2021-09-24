
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */






#ifndef bolt_Decoder_h
#define bolt_Decoder_h

#include <bolt/Types.h>

#include <string.h>


namespace bolt {


/**	@class Decoder
 *
 *	@brief This class is responsible for converting data into different formats
 *
**/
class Decoder
{
public:
	/**	@fn get()
	 *	@brief Returns a pointer to the singleton encoder object for this platform.
	**/
	static Decoder &get();


	virtual ~Decoder();

	bool isValid() const;

	/**	@fn getProblem() const
	 *
	 *	@brief Returns the problem string
	 *
	**/
	string getProblem() const;

	/** @fn get
	* @brief Puts a raw byte array.
	**/
	void get(const u8 *pi, i32 count, u8 *po) const;

	/** @fn get1
	* @brief Gets a single byte unsigned integer in network order.
	**/
	virtual i32 get1(const u8 *pi) const = 0;

	/** @fn get2
	* @brief Gets a two byte unsigned integer in network order.
	**/
	virtual i32 get2(const u8 *pi) const = 0;

	/** @fn get3
	* @brief Gets a three byte unsigned integer in network order.
	**/
	virtual i32 get3(const u8 *pi) const = 0;

	/** @fn get4
	* @brief Gets a four byte *signed* integer in network order.
	**/
	virtual i32 get4(const u8 *pi) const = 0;


	/** @fn get
	* @brief Gets an 8 byte real in network order.
	**/
	virtual f64 getr8(const u8 *pi) const = 0;



protected:

	Decoder();

	void setProblem(const string &text);


private:

	Decoder(const Decoder &other); // not implemented
	Decoder &operator=(const Decoder &other); // not implemented

	string problem;
};


} // namespace bolt


#endif /* bolt_Decoder_h */

