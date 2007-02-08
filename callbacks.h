#ifndef CALLBACKS_H
#define CALLBACKS_H

template < typename Return >
class Callback0
{
public:
	virtual Return operator()(void) const = 0;
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

#endif
