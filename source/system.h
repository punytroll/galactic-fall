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
    class FactionInfluence
    {
    public:
        Signals::ScopedConnection m_FactionDestroyingConnection;
        float m_Influence;
    };
    
    System(void);
    virtual ~System() override;
    auto AddFactionInfluence(Faction * Faction, float Influence) -> void;
    const std::list< Commodity * > & GetCommodities(void) const;
    auto GetFactionInfluence(Faction * Faction) const -> float;
    auto GetLinkedSystems() const -> std::list<System *> const &;
    const std::vector< Planet * > & GetPlanets(void) const;
    auto GetRandomFactionAccordingToInfluences() const -> Faction *;
    const std::list< Ship * > & GetShips(void) const;
    const std::list< Shot * > & GetShots(void) const;
    const Star * GetStar(void) const;
    float GetTrafficDensity(void) const;
    auto IsLinkedToSystem(System const * System) const -> bool;
    auto AddLinkedSystem(System * LinkedSystem) -> void;
    void SetTrafficDensity(float TrafficDensity);
    
private:
    void _OnAdded(Object * Content);
    auto m_OnFactionDestroying(Faction * Faction) -> void;
    void _OnRemoved(Object * Content);
    void _UpdateVisualization(Visualization * Visualization);
    
    std::list< Commodity * > _Commodities;
    std::unordered_map<Faction *, System::FactionInfluence> m_FactionInfluences;
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
