/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#ifndef CALLBACKS_H
#define CALLBACKS_H

template < typename Return >
class Callback0
{
public:
	virtual ~Callback0(void)
	{
	}
	
	virtual Return operator()(void) const = 0;
};

template < typename Return, typename Type1 >
class Callback1
{
public:
	virtual ~Callback1(void)
	{
	}
	
	virtual Return operator()(Type1 Argument1) const = 0;
};

template < typename Return, typename Type1, typename Type2 >
class Callback2
{
public:
	virtual ~Callback2(void)
	{
	}
	
	virtual Return operator()(Type1 Argument1, Type2 Argument2) const = 0;
};

template < typename Return >
class FunctionCallback0 : public Callback0< Return >
{
public:
	typedef Return (*Function)();
	
	explicit FunctionCallback0(Function Function) :
		m_Function(Function)
	{
	}
	
	virtual ~FunctionCallback0(void)
	{
	}
	
	virtual Return operator()(void) const
	{
		return (*m_Function)();
	}
protected:
	Function m_Function;
};

template < typename Return, typename Type1 >
class FunctionCallback1 : public Callback1< Return, Type1 >
{
public:
	typedef Return (*Function)(Type1);
	
	explicit FunctionCallback1(Function Function) :
		m_Function(Function)
	{
	}
	
	virtual ~FunctionCallback1(void)
	{
	}
	
	virtual Return operator()(Type1 Argument1) const
	{
		return (*m_Function)(Argument1);
	}
protected:
	Function m_Function;
};

template < typename Return, typename Type1, typename Type2 >
class FunctionCallback2 : public Callback2< Return, Type1, Type2 >
{
public:
	typedef Return (*Function)(Type1, Type2);
	
	explicit FunctionCallback2(Function Function) :
		m_Function(Function)
	{
	}
	
	virtual ~FunctionCallback2(void)
	{
	}
	
	virtual Return operator()(Type1 Argument1, Type2 Argument2) const
	{
		return (*m_Function)(Argument1, Argument2);
	}
protected:
	Function m_Function;
};

template < typename Return, typename Class >
class MethodCallback0 : public Callback0< Return >
{
public:
	typedef Return (Class::*Method)();
	
	explicit MethodCallback0(Class * Object, Method Method) :
		m_Object(Object),
		m_Method(Method)
	{
	}
	
	virtual ~MethodCallback0(void)
	{
	}
	
	virtual Return operator()(void) const
	{
		return (m_Object->*(this->m_Method))();
	}
protected:
	Class * m_Object;
	Method m_Method;
};

template < typename Return, typename Type1, typename Class >
class MethodCallback1 : public Callback1< Return, Type1 >
{
public:
	typedef Return (Class::*Method)(Type1);
	
	explicit MethodCallback1(Class * Object, Method Method) :
		m_Object(Object),
		m_Method(Method)
	{
	}
	
	virtual ~MethodCallback1(void)
	{
	}
	
	virtual Return operator()(Type1 Argument1) const
	{
		return (m_Object->*(this->m_Method))(Argument1);
	}
protected:
	Class * m_Object;
	Method m_Method;
};

template < typename Return, typename Type1 >
class Argument1Binder0 : public Callback0< Return >
{
public:
	explicit Argument1Binder0(Callback1< Return, Type1 > * Callback, Type1 BoundArgument1) :
		m_Callback(Callback),
		m_BoundArgument1(BoundArgument1)
	{
	}
	
	virtual ~Argument1Binder0(void)
	{
		delete m_Callback;
	}
	
	virtual Return operator()(void) const
	{
		return (*m_Callback)(m_BoundArgument1);
	}
protected:
	Callback1< Return, Type1 > * m_Callback;
	Type1 m_BoundArgument1;
};

template < typename Return, typename Type1, typename Type2 >
class Argument1Binder1 : public Callback1< Return, Type2 >
{
public:
	explicit Argument1Binder1(Callback2< Return, Type1, Type2 > * Callback, Type1 BoundArgument1) :
		m_Callback(Callback),
		m_BoundArgument1(BoundArgument1)
	{
	}
	
	virtual ~Argument1Binder1(void)
	{
		delete m_Callback;
	}
	
	virtual Return operator()(Type2 Argument2) const
	{
		return (*m_Callback)(m_BoundArgument1, Argument2);
	}
protected:
	Callback2< Return, Type1, Type2 > * m_Callback;
	Type1 m_BoundArgument1;
};

template < typename Return >
FunctionCallback0< Return > * Callback(Return (*Function)())
{
	return new FunctionCallback0< Return >(Function);
}

template < typename Return, typename Type1 >
FunctionCallback1< Return, Type1 > * Callback(Return (*Function)(Type1))
{
	return new FunctionCallback1< Return, Type1 >(Function);
}

template < typename Return, typename Type1, typename Type2 >
FunctionCallback2< Return, Type1, Type2 > * Callback(Return (*Function)(Type1, Type2))
{
	return new FunctionCallback2< Return, Type1, Type2 >(Function);
}

template < typename Return, typename Class >
MethodCallback0< Return, Class > * Callback(Class * Object, Return (Class::*Function)())
{
	return new MethodCallback0< Return, Class >(Object, Function);
}

template < typename Return, typename Type1, typename Class >
MethodCallback1< Return, Type1, Class > * Callback(Class * Object, Return (Class::*Function)(Type1))
{
	return new MethodCallback1< Return, Type1, Class >(Object, Function);
}

template < typename Return, typename Type1 >
Argument1Binder0< Return, Type1 > * Bind1(Callback1< Return, Type1 > * Callback, Type1 BoundArgument1)
{
	return new Argument1Binder0< Return, Type1 >(Callback, BoundArgument1);
}

template < typename Return, typename Type1, typename Type2 >
Argument1Binder1< Return, Type1, Type2 > * Bind1(Callback2< Return, Type1, Type2 > * Callback, Type1 BoundArgument1)
{
	return new Argument1Binder1< Return, Type1, Type2 >(Callback, BoundArgument1);
}

#endif
