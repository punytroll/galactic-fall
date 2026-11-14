/**
 * galactic-fall
 * Copyright (C) 2007-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include "faction.h"

Faction::Faction()
{
    // initialize object aspects
    AddAspectName();
}

auto Faction::AddFactionStanding(Faction * Faction, float Standing) -> void
{
    assert(Faction != nullptr);
    
    auto const & [Iterator, Success] = m_FactionStandings.try_emplace(Faction, Faction->ConnectDestroyingCallback(std::bind(&Faction::m_OnFactionDestroying, this, Faction)), Standing);
    
    assert(Success == true);
}

auto Faction::GetStanding(Faction * Faction) const -> float
{
    assert(Faction != nullptr);
    
    auto FactionStandingIterator = m_FactionStandings.find(Faction);
    
    if(FactionStandingIterator != m_FactionStandings.end())
    {
        return FactionStandingIterator->second.m_Standing;
    }
    else
    {
        // If we don't know the other faction, assume slightly above neutral to avoid conflict
        // This default value might later be tweaked per faction in relation to how aggressive a faction is when meeting new factions.
        return 0.6f;
    }
}

auto Faction::m_OnFactionDestroying(Faction * Faction) -> void
{
    assert(Faction != nullptr);
    
    auto EraseCount = m_FactionStandings.erase(Faction);
    
    assert(EraseCount == 1);
}
