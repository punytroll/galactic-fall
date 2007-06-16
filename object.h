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

#ifndef OBJECT_H
#define OBJECT_H

#include <map>
#include <ostream>
#include <set>
#include <string>

class XMLStream;

class Object
{
public:
	Object(void);
	virtual ~Object(void);
	void SetObjectIdentifier(const std::string & ObjectIdentifier);
	void GenerateObjectIdentifier(void);
	const std::string & GetObjectIdentifier(void) const;
	
	static Object * GetObject(const std::string & ObjectIdentifier);
	static void Dump(std::ostream & OStream);
	static void Dump(XMLStream & XML);
private:
	std::string m_ObjectIdentifier;
	
	static std::set< Object * > m_Objects;
	static std::map< std::string, Object * > m_IdentifiedObjects;
};

inline const std::string & Object::GetObjectIdentifier(void) const
{
	return m_ObjectIdentifier;
}

#endif
