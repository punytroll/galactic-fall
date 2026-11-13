/**
 * galactic-fall
 * Copyright (C) 2008  Aram Altschudjian
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__THREAT_H
#define GALACTIC_FALL__SOURCE__THREAT_H

#include <unordered_map>

class Object;

class Threat
{
public:
    class ThreatByObject
    {
    public:
        float m_Threat;
        Signals::Connection m_ObjectDestroyingConnection;
    };
    
    ~Threat();
    auto GetObjectWithHighestThreat() const -> Object *;
    auto ModifyThreat(Object * ThreateningObject, float DeltaThreat) -> void;
    
private:
    auto m_OnObjectDestroying(Object * Object) -> void;
    
    std::unordered_map<Object *, Threat::ThreatByObject> m_Threats;
};

#endif
