/**
 * galactic-fall
 * Copyright (C) 2007-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <ranges>

#include "faction.h"

class Faction::FactionStanding
{
public:
    FactionStanding(Faction * Faction, float Standing, std::function<auto () -> void> FactionDestroyingCallback) :
        m_Faction{Faction},
        m_Standing{Standing}
    {
        assert(m_Faction != nullptr);
        m_DestroyingConnection = m_Faction->ConnectDestroyingCallback(FactionDestroyingCallback);
    }
    
    FactionStanding(Faction::FactionStanding const & Other) = delete;
    
    FactionStanding(Faction::FactionStanding && Other) :
        m_DestroyingConnection(std::move(Other.m_DestroyingConnection)),
        m_Faction{std::exchange(Other.m_Faction, nullptr)},
        m_Standing{Other.m_Standing}
    {
    }
    
    auto operator=(Faction::FactionStanding const & Other) -> Faction::FactionStanding & = delete;
    
    auto operator=(Faction::FactionStanding && Other) -> Faction::FactionStanding &
    {
        m_DestroyingConnection = std::move(Other.m_DestroyingConnection);
        m_Faction = std::exchange(Other.m_Faction, nullptr);
        m_Standing = Other.m_Standing;
        
        return *this;
    }
    
    ~FactionStanding()
    {
        if(m_Faction != nullptr)
        {
            assert(m_DestroyingConnection.IsValid() == true);
            m_DestroyingConnection.Disconnect();
        }
        assert(m_DestroyingConnection.IsValid() == false);
    }
    
    Signals::Connection m_DestroyingConnection;
    Faction * m_Faction;
    float m_Standing;
};

Faction::Faction()
{
    // initialize object aspects
    AddAspectName();
}

auto Faction::AddFactionStanding(Faction * Faction, float Standing) -> void
{
    assert(Faction != nullptr);
    for(auto const & FactionStanding : m_FactionStandings)
    {
        assert(FactionStanding.m_Faction != Faction);
    }
    m_FactionStandings.emplace_back(Faction, Standing, std::bind(&Faction::m_OnFactionDestroying, this, Faction));
}

auto Faction::GetStanding(Faction * Faction) const -> float
{
    // If we don't know the other faction, assume slightly above neutral to avoid conflict
    // This default value might later be tweaked per faction in relation to how aggressive a faction is when meeting new factions.
    auto Result = 0.6f;
    
    assert(Faction != nullptr);
    for(auto const & FactionStanding : m_FactionStandings)
    {
        if(FactionStanding.m_Faction == Faction)
        {
            Result = FactionStanding.m_Standing;
            
            break;
        }
    }
    
    return Result;
}

auto Faction::m_OnFactionDestroying(Faction * Faction) -> void
{
    auto FactionStandingIterator = std::ranges::find_if(m_FactionStandings, [Faction](FactionStanding const & FactionStanding)
                                                                            {
                                                                                return FactionStanding.m_Faction == Faction;
                                                                            });
    
    assert(FactionStandingIterator != m_FactionStandings.end());
    m_FactionStandings.erase(FactionStandingIterator);
}
