/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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

#ifndef CLASS_MANAGER_H
#define CLASS_MANAGER_H

#include <assert.h>

#include <map>

template< class ClassType >
class ClassManager
{
public:
	// constructor & destructor
	ClassManager(void)
	{
	}
	
	~ClassManager(void)
	{
		while(m_Classes.size() > 0)
		{
			Destroy(m_Classes.begin()->first);
		}
	}
	
	// getters
	const ClassType * Get(const std::string & Identifier) const
	{
		typename std::map< std::string, ClassType * >::const_iterator ClassIterator(m_Classes.find(Identifier));
		
		assert(ClassIterator != m_Classes.end());
		
		return ClassIterator->second;
	}
	
	const std::map< std::string, ClassType * > & GetClasses(void) const
	{
		return m_Classes;
	}
	
	// modifiers
	ClassType * Create(const std::string & Identifier)
	{
		assert(m_Classes.find(Identifier) != m_Classes.end());
		
		ClassType * NewClass(new ClassType(Identifier));
		
		m_Classes[NewClass->GetIdentifier()] = NewClass;
		
		return NewClass;
	}
	
	void Destroy(const std::string & Identifier)
	{
		typename std::map< std::string, ClassType * >::iterator ClassIterator(m_Classes.find(Identifier));
		
		if(ClassIterator != m_Classes.end())
		{
			delete ClassIterator->second;
			m_Classes.erase(ClassIterator);
		}
	}
private:
	std::map< std::string, ClassType * > m_Classes;
};

#endif
