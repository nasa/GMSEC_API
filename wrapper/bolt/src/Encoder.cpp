
/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







#include <bolt/Encoder.h>

namespace bolt {



Encoder::Encoder()
{
}


Encoder::~Encoder()
{
}


bool Encoder::isValid() const
{
	return problem.size() == 0;
}


string Encoder::getProblem() const
{
	return problem;
}


void Encoder::put(const u8 *pi, i32 count, u8 *po) const
{
	memcpy(po, pi, count);
}



class BigEncoder : public Encoder
{
public:

	virtual ~BigEncoder() { }

	/** @fn put1
	* @brief Puts the least significant byte of x to po in network order.
	**/
	virtual void put1(i32 x, u8 *po) const {
		po[0] = x & 0xff;
	}


	/** @fn put2
	* @brief Puts the least significant bytes of x to po in network order.
	**/
	virtual void put2(i32 x, u8 *po) const {
		const u8 * pi = (u8*) &x;
		po[0] = pi[2];
		po[1] = pi[3];
	}


	/** @fn put3
	* @brief Puts the least significant bytes of x to po in network order.
	**/
	virtual void put3(i32 x, u8 *po) const {
		const u8 * pi = (u8*) &x;
		po[0] = pi[1];
		po[1] = pi[2];
		po[2] = pi[3];
	}


	/** @fn put4
	* @brief Puts the least significant bytes of x to po in network order.
	**/
	virtual void put4(i32 x, u8 *po) const {
		const u8 * pi = (u8*) &x;
		po[0] = pi[0];
		po[1] = pi[1];
		po[2] = pi[2];
		po[3] = pi[3];
	}


	void put8(const u8 *pi, u8 *po) const {
		po[0] = pi[0];
		po[1] = pi[1];
		po[2] = pi[2];
		po[3] = pi[3];
		po[4] = pi[4];
		po[5] = pi[5];
		po[6] = pi[6];
		po[7] = pi[7];
	}

	virtual void putr8(f64 x, u8 *po) const {
		put8((u8*) &x, po);
	}



};



class LittleEncoder : public Encoder
{
public:

	virtual ~LittleEncoder() { }

	/** @fn put1
	* @brief Puts the least significant byte of x to po in network order.
	**/
	virtual void put1(i32 x, u8 *po) const {
		po[0] = x & 0xff;
	}


	/** @fn put2
	* @brief Puts the least significant bytes of x to po in network order.
	**/
	virtual void put2(i32 x, u8 *po) const {
		const u8 * pi = (u8 *) &x;
		po[0] = pi[1];
		po[1] = pi[0];
	}


	/** @fn put3
	* @brief Puts the least significant bytes of x to po in network order.
	**/
	virtual void put3(i32 x, u8 *po) const {
		const u8 * pi = (u8*) &x;
		po[0] = pi[2];
		po[1] = pi[1];
		po[2] = pi[0];
	}


	/** @fn put4
	* @brief Puts the least significant bytes of x to po in network order.
	**/
	virtual void put4(i32 x, u8 *po) const {
		const u8 * pi = (u8 *) &x;
		po[0] = pi[3];
		po[1] = pi[2];
		po[2] = pi[1];
		po[3] = pi[0];
	}


	void put8(const u8 *pi, u8 *po) const {
		po[0] = pi[7];
		po[1] = pi[6];
		po[2] = pi[5];
		po[3] = pi[4];
		po[4] = pi[3];
		po[5] = pi[2];
		po[6] = pi[1];
		po[7] = pi[0];
	}

	virtual void putr8(f64 x, u8 *po) const {
		put8((u8*) &x, po);
	}

};



Encoder &Encoder::get()
{
	int x = 1;
	u8 *p = (u8*) &x;
	if (p[0]) {
		static LittleEncoder little;
		return little;
	}
	else {
		static BigEncoder big;
		return big;
	}
}




} // namespace bolt
