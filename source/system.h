/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__SYSTEM_H
#define GALACTIC_FALL__SOURCE__SYSTEM_H

#include <list>
#include <string>
#include <vector>

#include "object.h"

class Commodity;
class Faction;
class FactionInfluence;
class Planet;
class Ship;
class Shot;
class Star;
class Visualization;

class System : public Object
{
public:
    // constructor and destructor
    System(void);
    virtual ~System(void) override;
    // queries
    const std::list< Commodity * > & GetCommodities(void) const;
    float GetFactionInfluence(Faction * Faction) const;
    auto GetLinkedSystems() const -> std::list<System *> const &;
    const std::vector< Planet * > & GetPlanets(void) const;
    Faction * GetRandomFactionAccordingToInfluences(void);
    const std::list< Ship * > & GetShips(void) const;
    const std::list< Shot * > & GetShots(void) const;
    const Star * GetStar(void) const;
    float GetTrafficDensity(void) const;
    auto IsLinkedToSystem(System const * System) const -> bool;
    // modifiers
    void AddFactionInfluence(Faction * Faction, float Influence);
    auto AddLinkedSystem(System * LinkedSystem) -> void;
    void SetTrafficDensity(float TrafficDensity);
private:
    // callbacks
    void _OnAdded(Object * Content);
    void _OnFactionDestroying(Faction * Faction);
    void _OnRemoved(Object * Content);
    void _UpdateVisualization(Visualization * Visualization);
    // member variables
    std::list< Commodity * > _Commodities;
    std::vector< FactionInfluence * > _FactionInfluences;
    std::list<System *> m_LinkedSystems;
    std::vector< Planet * > _Planets;
    std::list< Ship * > _Ships;
    std::list< Shot * > _Shots;
    Star * _Star;
    float _TrafficDensity;
};

inline const std::list< Commodity * > & System::GetCommodities(void) const
{
    return _Commodities;
}

inline auto System::GetLinkedSystems() const -> std::list<System *> const &
{
    return m_LinkedSystems;
}

inline const std::vector< Planet * > & System::GetPlanets(void) const
{
    return _Planets;
}

inline const std::list< Ship * > & System::GetShips(void) const
{
    return _Ships;
}

inline const std::list< Shot * > & System::GetShots(void) const
{
    return _Shots;
}

inline const Star * System::GetStar(void) const
{
    return _Star;
}

inline float System::GetTrafficDensity(void) const
{
    return _TrafficDensity;
}

inline void System::SetTrafficDensity(float TrafficDensity)
{
    _TrafficDensity = TrafficDensity;
}

#endif
