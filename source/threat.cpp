/**
 * galactic-fall
 * Copyright (C) 2008  Aram Altschudjian
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <ranges>

#include "object.h"
#include "threat.h"

class Threat::ThreatByObject
{
public:
    Object * m_Object;
    float m_Threat;
    Signals::Connection m_ObjectDestroyingConnection;
};

Threat::~Threat()
{
    while(m_Threats.empty() == false)
    {
        auto ThreatByObject = m_Threats.front();
        
        assert(ThreatByObject != nullptr);
        assert(ThreatByObject->m_Object != nullptr);
        ThreatByObject->m_Object = nullptr;
        assert(ThreatByObject->m_ObjectDestroyingConnection.IsValid() == true);
        ThreatByObject->m_ObjectDestroyingConnection.Disconnect();
        assert(ThreatByObject->m_ObjectDestroyingConnection.IsValid() == false);
        m_Threats.erase(m_Threats.begin());
    }
}

auto Threat::GetObjectWithHighestThreat() const -> Object *
{
    auto Result = static_cast<Object *>(nullptr);
    auto HighestThreat = 0.0f;
    
    for(auto & ThreatByObject : m_Threats)
    {
        if(ThreatByObject->m_Threat > HighestThreat)
        {
            Result = ThreatByObject->m_Object;
            HighestThreat = ThreatByObject->m_Threat;
        }
    }
    
    return Result;
}

auto Threat::ModifyThreat(Object * ThreateningObject, float DeltaThreat) -> void
{
    for(auto ThreatByObject : m_Threats)
    {
        if(ThreatByObject->m_Object == ThreateningObject)
        {
            ThreatByObject->m_Threat += DeltaThreat;
            
            return;
        }
    }
    
    auto NewThreatByObject = new ThreatByObject{};
    
    NewThreatByObject->m_Object = ThreateningObject;
    NewThreatByObject->m_ObjectDestroyingConnection = ThreateningObject->ConnectDestroyingCallback(std::bind(&Threat::m_OnObjectDestroying, this, NewThreatByObject));
    NewThreatByObject->m_Threat = DeltaThreat;
    m_Threats.push_back(NewThreatByObject);
}

auto Threat::m_OnObjectDestroying(ThreatByObject * ThreatByObject) -> void
{
    assert(ThreatByObject != nullptr);
    assert(ThreatByObject->m_ObjectDestroyingConnection.IsValid() == true);
    ThreatByObject->m_ObjectDestroyingConnection.Disconnect();
    assert(ThreatByObject->m_ObjectDestroyingConnection.IsValid() == false);
    ThreatByObject->m_Object = nullptr;
    
    auto ThreatByObjectIterator = std::ranges::find(m_Threats, ThreatByObject);
    
    assert(ThreatByObjectIterator != m_Threats.end());
    m_Threats.erase(ThreatByObjectIterator);
}
