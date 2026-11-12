/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__FACTION_H
#define GALACTIC_FALL__SOURCE__FACTION_H

#include <vector>

#include "graphics/color_rgbo.h"
#include "object.h"

class FactionStanding;

class Faction : public Object
{
public:
    Faction();
    ~Faction() override;
    auto GetColor() const -> const Graphics::ColorRGBO &;
    auto GetStanding(Faction * Faction) const -> float;
    auto AddFactionStanding(Faction * Faction, float Standing) -> void;
    auto SetColor(Graphics::ColorRGBO const & Color) -> void;
    
private:
    auto m_OnFactionDestroying(Faction * Faction) -> void;
    
    Graphics::ColorRGBO m_Color;
    std::vector<FactionStanding *> m_FactionStandings;
};

inline auto Faction::GetColor() const -> Graphics::ColorRGBO const &
{
    return m_Color;
}

inline auto Faction::SetColor(Graphics::ColorRGBO const & Color) -> void
{
    m_Color = Color;
}

#endif
