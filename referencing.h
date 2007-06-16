#ifndef REFERENCING_H
#define REFERENCING_H

#include <assert.h>

class Core
{
public:
	Core(void * Object) :
		m_ReferenceCount(0),
		m_Object(Object)
	{
	}
	
	~Core(void)
	{
		assert(m_ReferenceCount == 0);
	}
	
	void Reference(void)
	{
		++m_ReferenceCount;
	}
	
	void Release(void)
	{
		assert(m_ReferenceCount > 0);
		--m_ReferenceCount;
	}
	
	void * Get(void)
	{
		return m_Object;
	}
	
	void Invalidate(void)
	{
		m_Object = 0;
	}
	
	bool IsValid(void) const
	{
		return m_Object != 0;
	}
	
	unsigned int GetReferenceCount(void) const
	{
		return m_ReferenceCount;
	}
private:
	unsigned int m_ReferenceCount;
	void * m_Object;
};

template < typename Type >
class Reference
{
public:
	Reference(void) :
		m_Core(0)
	{
	}
	
	Reference(Type & Object) :
		m_Core(new Core(&Object))
	{
		m_Core->Reference();
	}
	
	Reference(const Reference< Type > & Reference) :
		m_Core(Reference.m_Core)
	{
		if(m_Core != 0)
		{
			m_Core->Reference();
		}
	}
	
	~Reference(void)
	{
		Cleanup();
	}
	
	Reference< Type > & operator=(const Reference< Type > & Reference)
	{
		Cleanup();
		m_Core = Reference.m_Core;
		m_Core->Reference();
		
		return *this;
	}
	
	Type * operator->(void)
	{
		return m_Core->Get();
	}
	
	const Type * operator->(void) const
	{
		return m_Core->Get();
	}
	
	bool IsValid(void) const
	{
		return ((m_Core == 0) ? (false) : (m_Core->IsValid()));
	}
	
	void Invalidate(void)
	{
		m_Core->Invalidate();
	}
	
	Type * Get(void)
	{
		return reinterpret_cast< Type * >((m_Core == 0) ? (0) : (m_Core->Get()));
	}
	
	const Type * Get(void) const
	{
		return reinterpret_cast< Type * >((m_Core == 0) ? (0) : (m_Core->Get()));
	}
private:
	void Cleanup(void)
	{
		if(m_Core != 0)
		{
			m_Core->Release();
			if(m_Core->GetReferenceCount() == 0)
			{
				delete m_Core;
			}
			m_Core = 0;
		}
	}
	
	Core * m_Core;
};

#endif
