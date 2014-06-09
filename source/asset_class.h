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

#ifndef ASSET_CLASS_H
#define ASSET_CLASS_H

#include <string>

class Color;
class Model;

class AssetClass
{
public:
	// constructor & destructor
	AssetClass(const std::string & Identifier);
	~AssetClass(void);
	// getters
	float GetBasePrice(void) const;
	const std::string & GetIdentifier(void) const;
	const std::string & GetName(void) const;
	const std::string & GetObjectClassIdentifier(void) const;
	const std::string & GetObjectTypeIdentifier(void) const;
	// setters
	void SetBasePrice(float BasePrice);
	void SetName(const std::string & Name);
	void SetObjectClassIdentifier(const std::string & ObjectClassIdentifier);
	void SetObjectTypeIdentifier(const std::string & ObjectTypeIdentifier);
private:
	float _BasePrice;
	std::string _Identifier;
	std::string _Name;
	std::string _ObjectClassIdentifier;
	std::string _ObjectTypeIdentifier;
};

inline float AssetClass::GetBasePrice(void) const
{
	return _BasePrice;
}

inline const std::string & AssetClass::GetIdentifier(void) const
{
	return _Identifier;
}

inline const std::string & AssetClass::GetName(void) const
{
	return _Name;
}

inline const std::string & AssetClass::GetObjectClassIdentifier(void) const
{
	return _ObjectClassIdentifier;
}

inline const std::string & AssetClass::GetObjectTypeIdentifier(void) const
{
	return _ObjectTypeIdentifier;
}

inline void AssetClass::SetBasePrice(float BasePrice)
{
	_BasePrice = BasePrice;
}

inline void AssetClass::SetName(const std::string & Name)
{
	_Name = Name;
}

inline void AssetClass::SetObjectClassIdentifier(const std::string & ObjectClassIdentifier)
{
	_ObjectClassIdentifier = ObjectClassIdentifier;
}

inline void AssetClass::SetObjectTypeIdentifier(const std::string & ObjectTypeIdentifier)
{
	_ObjectTypeIdentifier = ObjectTypeIdentifier;
}

#endif
