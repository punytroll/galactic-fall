/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#ifndef SLOT_CLASS_H
#define SLOT_CLASS_H

#include <string>

class SlotClass
{
public:
	SlotClass(const std::string & Identifier);
	~SlotClass(void);
	// getters
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	// setters
	void SetName(const std::string & Name);
private:
	std::string m_Identifier;
	std::string m_Name;
};

inline const std::string & SlotClass::GetIdentifier(void) const
{
	return m_Identifier;
}

inline const std::string & SlotClass::GetName(void) const
{
	return m_Name;
}

inline void SlotClass::SetName(const std::string & Name)
{
	m_Name = Name;
}

#endif
