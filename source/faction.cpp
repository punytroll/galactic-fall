/**
 * galactic-fall
 * Copyright (C) 2007-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <ranges>

#include "faction.h"

class FactionStanding
{
public:
    Signals::Connection m_DestroyingConnection;
    Faction * m_Faction;
    float m_Standing;
};

Faction::Faction()
{
    // initialize object aspects
    AddAspectName();
}

Faction::~Faction()
{
    while(m_FactionStandings.empty() == false)
    {
        auto FactionStanding = m_FactionStandings.back();
        
        assert(FactionStanding->m_Faction != nullptr);
        FactionStanding->m_Faction = nullptr;
        assert(FactionStanding->m_DestroyingConnection.IsValid() == true);
        FactionStanding->m_DestroyingConnection.Disconnect();
        assert(FactionStanding->m_DestroyingConnection.IsValid() == false);
        delete FactionStanding;
        m_FactionStandings.pop_back();
    }
}

auto Faction::AddFactionStanding(Faction * Faction, float Standing) -> void
{
    assert(Faction != nullptr);
    for(auto FactionStanding : m_FactionStandings)
    {
        assert(FactionStanding != nullptr);
        assert(FactionStanding->m_Faction != Faction);
    }
    
    auto NewFactionStanding = new FactionStanding{};
    
    NewFactionStanding->m_Faction = Faction;
    NewFactionStanding->m_Standing = Standing;
    NewFactionStanding->m_DestroyingConnection = Faction->ConnectDestroyingCallback(std::bind(&Faction::m_OnFactionDestroying, this, Faction));
    m_FactionStandings.push_back(NewFactionStanding);
}

auto Faction::GetStanding(Faction * Faction) const -> float
{
    // If we don't know the other faction, assume slightly above neutral to avoid conflict
    // This default value might later be tweaked per faction in relation to how aggressive a faction is when meeting new factions.
    auto Result = 0.6f;
    
    assert(Faction != nullptr);
    for(auto FactionStanding : m_FactionStandings)
    {
        assert(FactionStanding != nullptr);
        if(FactionStanding->m_Faction == Faction)
        {
            Result = FactionStanding->m_Standing;
            
            break;
        }
    }
    
    return Result;
}

auto Faction::m_OnFactionDestroying(Faction * Faction) -> void
{
    auto FactionStandingIterator = std::ranges::find_if(m_FactionStandings, [Faction](FactionStanding * FactionStanding)
                                                                            {
                                                                                return FactionStanding->m_Faction == Faction;
                                                                            });
    
    assert(FactionStandingIterator != m_FactionStandings.end());
    
    auto FactionStanding = *FactionStandingIterator;
    
    assert(FactionStanding->m_Faction != nullptr);
    FactionStanding->m_Faction = nullptr;
    assert(FactionStanding->m_DestroyingConnection.IsValid() == true);
    FactionStanding->m_DestroyingConnection.Disconnect();
    assert(FactionStanding->m_DestroyingConnection.IsValid() == false);
    delete FactionStanding;
    m_FactionStandings.erase(FactionStandingIterator);
}
