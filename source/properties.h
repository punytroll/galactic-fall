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

#include <experimental/any>
#include <list>
#include <string>
#include <unordered_map>

class VisualizationPrototype;
class SlotClass;

class Properties
{
public:
	// constructor & destructor
	Properties(void);
	virtual ~Properties(void);
	// getters
	template< typename ValueType >
	const ValueType & GetValue(const std::string & PropertyIdentifier) const
	{
		return std::experimental::any_cast< const ValueType & >(_Properties.at(PropertyIdentifier));
	}
	
	const std::list< Properties > & GetPropertyAsList(const std::string & PropertyIdentifier) const;
	const SlotClass * GetPropertyAsSlotClass(const std::string & PropertyIdentifier) const;
	const VisualizationPrototype & GetPropertyAsVisualizationPrototype(const std::string & PropertyIdentifier) const;
	// setters
	void AddList(const std::string & PropertyIdentifier, const std::list< Properties > & PropertiesList)
	{
		_Properties.insert({PropertyIdentifier, PropertiesList});
	}
	
	template< typename AnyType >
	void AddProperty(const std::string & PropertyIdentifier, const AnyType & Value)
	{
		_Properties.insert({PropertyIdentifier, Value});
	}
private:
	std::unordered_map< std::string, std::experimental::any > _Properties;
};

#endif
