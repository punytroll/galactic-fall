/**
 * galactic-fall
 * Copyright (C) 2018  Hagen Möbius
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

#ifndef TEMPLATE_H
#define TEMPLATE_H

#include <experimental/any>
#include <string>
#include <unordered_map>

#include <algebra/vector3f.h>

class Template
{
public:
	// constructor & destructor
	Template(const std::string & TypeIdentifier, const std::string & SubTypeIdentifier);
	virtual ~Template(void);
	// getters
	const std::experimental::any & GetField(const std::string & FieldIdentifier) const;
	float GetFieldAsFloat(const std::string & FieldIdentifier) const;
	const std::string & GetFieldAsString(const std::string & FieldIdentifier) const;
	std::uint32_t GetFieldAsUnsignedInteger32Bit(const std::string & FieldIdentifier) const;
	const Vector3f & GetFieldAsVector3f(const std::string & FieldIdentifier) const;
	const std::string & GetSubTypeIdentifier(void) const;
	const std::string & GetTypeIdentifier(void) const;
	// setters
	template< typename AnyType >
	void SetField(const std::string & FieldIdentifier, const AnyType & Value)
	{
		_Fields.insert({FieldIdentifier, Value});
	}
private:
	std::unordered_map< std::string, std::experimental::any > _Fields;
	std::string _SubTypeIdentifier;
	std::string _TypeIdentifier;
};

inline const std::string & Template::GetSubTypeIdentifier(void) const
{
	return _SubTypeIdentifier;
}

inline const std::string & Template::GetTypeIdentifier(void) const
{
	return _TypeIdentifier;
}

#endif
