/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */




#ifndef CALLBACK_H
#define CALLBACK_H

class CallbackInterface
{
public:
	virtual bool execute() const = 0;

	virtual ~CallbackInterface() {}

protected:
	CallbackInterface() {}
};

template <typename T>
class Callback : public CallbackInterface
{
public:
	typedef bool (T::*Function)(void);

	Callback() : m_function(0) {}

	virtual ~Callback() {}

	virtual bool execute() const
	{
		if (m_function)
		{
			return (m_instance->*m_function)();
		}
		return false;
	}

	void setCallback(T* instance, Function f)
	{
		m_instance = instance;
		m_function = f;
	}

private:
	T*       m_instance;
	Function m_function;
};

#endif
