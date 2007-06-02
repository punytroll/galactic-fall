#ifndef CALLBACKS_H
#define CALLBACKS_H

template < typename Return >
class Callback0
{
public:
	virtual Return operator()(void) const = 0;
};

template < typename Return, typename Type1 >
class Callback1
{
public:
	virtual Return operator()(Type1 Argument1) const = 0;
};

template < typename Return, typename Type1, typename Type2 >
class Callback2
{
public:
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
	
	virtual Return operator()(Type1 Argument1, Type2 Argument2) const
	{
		return (*m_Function)(Argument1, Argument2);
	}
protected:
	Function m_Function;
};

template < typename Return, typename Class >
class MemberCallback0 : public Callback0< Return >
{
public:
	typedef Return (Class::*Function)();
	
	explicit MemberCallback0(Class * Object, Function Function) :
		m_Object(Object),
		m_Function(Function)
	{
	}
	
	virtual Return operator()(void) const
	{
		return (m_Object->*(this->m_Function))();
	}
protected:
	Class * m_Object;
	Function m_Function;
};

template < typename Return, typename Type1, typename Type2 >
class Argument1Binder1 : public Callback1< Return, Type2 >
{
public:
	explicit Argument1Binder1(const Callback2< Return, Type1, Type2 > & Callback, Type1 BoundArgument1) :
		m_Callback(Callback),
		m_BoundArgument1(BoundArgument1)
	{
	}
	
	virtual Return operator()(Type2 Argument2) const
	{
		return m_Callback(m_BoundArgument1, Argument2);
	}
protected:
	const Callback2< Return, Type1, Type2 > & m_Callback;
	Type1 m_BoundArgument1;
};

#endif
