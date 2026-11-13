/**
 * galactic-fall
 * Copyright (C) 2008  Aram Altschudjian
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__THREAT_H
#define GALACTIC_FALL__SOURCE__THREAT_H

#include <deque>

class Object;
class ThreatByObject;

class Threat
{
public:
    // destructor
    ~Threat(void);
    // getters
    Object * GetObjectWithHighestThreat(void);
    // modifiers
    void ModifyThreat(Object * ThreateningObject, float DeltaThreat);
private:
    void _OnObjectDestroying(ThreatByObject * ThreatByObject);
    std::deque< ThreatByObject * > _Threats;
};

#endif
