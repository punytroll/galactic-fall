/**
 * galactic-fall
 * Copyright (C) 2008  Aram Altschudjian
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include "object.h"
#include "threat.h"

auto Threat::GetObjectWithHighestThreat() const -> Object *
{
    auto Result = static_cast<Object *>(nullptr);
    auto HighestThreat = 0.0f;
    
    for(auto const & [Object, ThreatByObject] : m_Threats)
    {
        if(ThreatByObject.m_Threat > HighestThreat)
        {
            Result = Object;
            HighestThreat = ThreatByObject.m_Threat;
        }
    }
    
    return Result;
}

auto Threat::ModifyThreat(Object * ThreateningObject, float DeltaThreat) -> void
{
    for(auto & [Object, ThreatByObject] : m_Threats)
    {
        if(Object == ThreateningObject)
        {
            ThreatByObject.m_Threat += DeltaThreat;
            
            return;
        }
    }
    m_Threats.try_emplace(ThreateningObject, DeltaThreat, ThreateningObject->ConnectDestroyingCallback(std::bind(&Threat::m_OnObjectDestroying, this, ThreateningObject)));
}

auto Threat::m_OnObjectDestroying(Object * Object) -> void
{
    assert(Object != nullptr);
    m_Threats.erase(Object);
}
