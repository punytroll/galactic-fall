/**
 * galactic-fall
 * Copyright (C) 2008  Aram Altschudjian
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
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
    Signals::Connection m_ObjectDestroyingConnection;
};

Threat::~Threat(void)
{
    while(_Threats.empty() == false)
    {
        auto ThreatByObject{_Threats.front()};
        
        assert(ThreatByObject != nullptr);
        assert(ThreatByObject->_Object != nullptr);
        ThreatByObject->_Object = nullptr;
        assert(ThreatByObject->m_ObjectDestroyingConnection.IsValid() == true);
        ThreatByObject->m_ObjectDestroyingConnection.Disconnect();
        assert(ThreatByObject->m_ObjectDestroyingConnection.IsValid() == false);
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
        NewThreatByObject->m_ObjectDestroyingConnection = ThreateningObject->ConnectDestroyingCallback(std::bind(&Threat::_OnObjectDestroying, this, NewThreatByObject));
        NewThreatByObject->_Threat = DeltaThreat;
        _Threats.push_back(NewThreatByObject);
    }
}

void Threat::_OnObjectDestroying(ThreatByObject * ThreatByObject)
{
    assert(ThreatByObject != nullptr);
    assert(ThreatByObject->m_ObjectDestroyingConnection.IsValid() == true);
    ThreatByObject->m_ObjectDestroyingConnection.Disconnect();
    assert(ThreatByObject->m_ObjectDestroyingConnection.IsValid() == false);
    ThreatByObject->_Object = nullptr;
    
    auto Iterator{std::find(_Threats.begin(), _Threats.end(), ThreatByObject)};
    
    assert(Iterator != _Threats.end());
    _Threats.erase(Iterator);
}
