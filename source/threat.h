/**
 * galactic-fall
 * Copyright (C) 2008  Aram Altschudjian
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__THREAT_H
#define GALACTIC_FALL__SOURCE__THREAT_H

#include <list>

class Object;

class Threat
{
public:
    class ThreatByObject;
    
    ~Threat();
    auto GetObjectWithHighestThreat() const -> Object *;
    auto ModifyThreat(Object * ThreateningObject, float DeltaThreat) -> void;
    
private:
    auto m_OnObjectDestroying(ThreatByObject * ThreatByObject) -> void;
    
    std::list<Threat::ThreatByObject *> m_Threats;
};

#endif
