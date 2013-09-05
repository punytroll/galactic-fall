/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#ifndef SCENARIO_H
#define SCENARIO_H

#include <string>

class Scenario
{
public:
	// constructor & destructor
	Scenario(const std::string & Identifier);
	// getters
	const std::string & GetDescription(void) const;
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const std::string & GetResourcePath(void) const;
	// setters
	void SetDescription(const std::string & Description);
	void SetName(const std::string & Name);
	void SetResourcePath(const std::string & ResourcePath);
private:
	std::string _Description;
	std::string _Identifier;
	std::string _Name;
	std::string _ResourcePath;
};

inline const std::string & Scenario::GetDescription(void) const
{
	return _Description;
}

inline const std::string & Scenario::GetIdentifier(void) const
{
	return _Identifier;
}

inline const std::string & Scenario::GetName(void) const
{
	return _Name;
}

inline const std::string & Scenario::GetResourcePath(void) const
{
	return _ResourcePath;
}

inline void Scenario::SetDescription(const std::string & Description)
{
	_Description = Description;
}

inline void Scenario::SetName(const std::string & Name)
{
	_Name = Name;
}

inline void Scenario::SetResourcePath(const std::string & ResourcePath)
{
	_ResourcePath = ResourcePath;
}

#endif
