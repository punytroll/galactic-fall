/**
 * galactic-fall
 * Copyright (C) 2008  Aram Altschudjian
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

#include <float.h>

#include <algorithm>

#include "object.h"
#include "threat.h"

class ThreatByObject
{
public:
	Object * _Object;
	float _Threat;
	Connection _ObjectDestroyingConnection;
};

Threat::~Threat(void)
{
	while(_Threats.empty() == false)
	{
		auto ThreatByObject{_Threats.front()};
		
		assert(ThreatByObject != nullptr);
		assert(ThreatByObject->_Object != nullptr);
		ThreatByObject->_Object = nullptr;
		assert(ThreatByObject->_ObjectDestroyingConnection.IsValid() == true);
		ThreatByObject->_ObjectDestroyingConnection.Disconnect();
		assert(ThreatByObject->_ObjectDestroyingConnection.IsValid() == false);
		_Threats.erase(_Threats.begin());
	}
}

Object * Threat::GetObjectWithHighestThreat(void)
{
	Object * Result(nullptr);
	float HighestThreat(FLT_MIN);
	
	for(auto & Threat : _Threats)
	{
		if(Threat->_Threat > HighestThreat)
		{
			Result = Threat->_Object;
			HighestThreat = Threat->_Threat;
		}
	}
	
	return Result;
}

void Threat::ModifyThreat(Object * ThreateningObject, float DeltaThreat)
{
	auto Found{false};
	
	for(auto ThreatByObject : _Threats)
	{
		if(ThreatByObject->_Object == ThreateningObject)
		{
			ThreatByObject->_Threat += DeltaThreat;
			Found = true;
			
			break;
		}
	}
	if(Found == false)
	{
		auto NewThreatByObject{new ThreatByObject{}};
		
		NewThreatByObject->_Object = ThreateningObject;
		NewThreatByObject->_ObjectDestroyingConnection = ThreateningObject->ConnectDestroyingCallback(std::bind(&Threat::_OnObjectDestroying, this, NewThreatByObject));
		NewThreatByObject->_Threat = DeltaThreat;
		_Threats.push_back(NewThreatByObject);
	}
}

void Threat::_OnObjectDestroying(ThreatByObject * ThreatByObject)
{
	assert(ThreatByObject != nullptr);
	assert(ThreatByObject->_ObjectDestroyingConnection.IsValid() == true);
	ThreatByObject->_ObjectDestroyingConnection.Disconnect();
	assert(ThreatByObject->_ObjectDestroyingConnection.IsValid() == false);
	ThreatByObject->_Object = nullptr;
	
	auto Iterator{std::find(_Threats.begin(), _Threats.end(), ThreatByObject)};
	
	assert(Iterator != _Threats.end());
	_Threats.erase(Iterator);
}
