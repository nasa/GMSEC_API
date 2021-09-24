
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <bolt/Decoder.h>

namespace bolt {



Decoder::Decoder()
{
}


Decoder::~Decoder()
{
}


bool Decoder::isValid() const
{
	return problem.size() == 0;
}


string Decoder::getProblem() const
{
	return problem;
}


void Decoder::get(const u8 *pi, i32 count, u8 *po) const
{
	memcpy(po, pi, count);
}



class BigDecoder : public Decoder
{
public:

	virtual ~BigDecoder() { }

	/** @fn get1
	* @brief Gets the least significant byte of x to po in network order.
	**/
	virtual i32 get1(const u8 *pi) const {
		i32 tmp = *pi;
		return tmp;
	}


	/** @fn get2
	* @brief Gets the least significant bytes of x to po in network order.
	**/
	virtual i32 get2(const u8 *pi) const {
		i32 tmp = pi[0] * 256 + pi[1];
		return tmp;
	}


	/** @fn get3
	* @brief Gets the least significant bytes of x to po in network order.
	**/
	virtual i32 get3(const u8 *pi) const {
		i32 tmp = 0;
		u8 *po = (u8*) &tmp;
		po[0] = pi[0];
		po[1] = pi[1];
		po[2] = pi[2];
		return tmp;
	}


	/** @fn get4
	* @brief Gets the least significant bytes of x to po in network order.
	**/
	virtual i32 get4(const u8 *pi) const {
		i32 tmp = 0;
		u8 *po = (u8*) &tmp;
		po[0] = pi[0];
		po[1] = pi[1];
		po[2] = pi[2];
		po[3] = pi[3];
		return tmp;
	}


	virtual f64 getr8(const u8 *pi) const {
		f64 tmp = 0;
		u8 *po = (u8*) &tmp;
		po[0] = pi[0];
		po[1] = pi[1];
		po[2] = pi[2];
		po[3] = pi[3];
		po[4] = pi[4];
		po[5] = pi[5];
		po[6] = pi[6];
		po[7] = pi[7];
		return tmp;
	}



};



class LittleDecoder : public Decoder
{
public:

	virtual ~LittleDecoder() { }

	/** @fn get1
	* @brief Gets the least significant byte of x to po in network order.
	**/
	virtual i32 get1(const u8 *pi) const {
		i32 tmp = *pi;
		return tmp;
	}


	/** @fn get2
	* @brief Gets the least significant bytes of x to po in network order.
	**/
	virtual i32 get2(const u8 *pi) const {
		i32 tmp = pi[0] * 256 + pi[1];
		return tmp;
	}


	/** @fn get3
	* @brief Gets the least significant bytes of x to po in network order.
	**/
	virtual i32 get3(const u8 *pi) const {
		i32 tmp = 0;
		u8 *po = (u8*) &tmp;
		po[2] = pi[0];
		po[1] = pi[1];
		po[0] = pi[2];
		return tmp;
	}


	/** @fn get4
	* @brief Gets the least significant bytes of x to po in network order.
	**/
	virtual i32 get4(const u8 *pi) const {
		i32 tmp = 0;
		u8 *po = (u8*) &tmp;
		po[3] = pi[0];
		po[2] = pi[1];
		po[1] = pi[2];
		po[0] = pi[3];
		return tmp;
	}


	virtual f64 getr8(const u8 *pi) const {
		f64 tmp = 0;
		u8 *po = (u8*) &tmp;
		po[7] = pi[0];
		po[6] = pi[1];
		po[5] = pi[2];
		po[4] = pi[3];
		po[3] = pi[4];
		po[2] = pi[5];
		po[1] = pi[6];
		po[0] = pi[7];
		return tmp;
	}



};




Decoder &Decoder::get()
{
	int x = 1;
	u8 *p = (u8*) &x;
	if (p[0]) {
		static LittleDecoder little;
		return little;
	}
	else {
		static BigDecoder big;
		return big;
	}
}



} // namespace bolt

