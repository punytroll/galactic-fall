/**
 * galactic-fall
 * Copyright (C) 2018-2019  Hagen Möbius
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

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <any>
#include <list>
#include <string>
#include <unordered_map>

class Properties
{
public:
	// constructor & destructor
	Properties() = default;
	virtual ~Properties() = default;
    
	template<typename ValueType>
	auto GetValue(std::string const & PropertyIdentifier) const -> ValueType const &
	{
		return std::any_cast<ValueType const &>(m_Properties.at(PropertyIdentifier));
	}
	
	auto GetPropertyAsList(std::string const & PropertyIdentifier) const -> std::list<Properties> const &;
    
	auto AddList(std::string const & PropertyIdentifier, std::list<Properties> const & PropertiesList) -> void
	{
		m_Properties.insert({PropertyIdentifier, PropertiesList});
	}
	
	template<typename ValueType>
	auto AddProperty(std::string const & PropertyIdentifier, ValueType const & Value) -> void
	{
		m_Properties.insert({PropertyIdentifier, Value});
	}
    
private:
	std::unordered_map<std::string, std::any> m_Properties;
};

#endif
