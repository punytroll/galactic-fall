/**
 * galactic-fall
 * Copyright (C) 2006  Hagen MÃ¶bius
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
