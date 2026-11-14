/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <algorithm>

#include "commodity.h"
#include "faction.h"
#include "globals.h"
#include "graphics/system_node.h"
#include "math.h"
#include "object_aspect_object_container.h"
#include "object_aspect_position.h"
#include "object_aspect_visualization.h"
#include "planet.h"
#include "ship.h"
#include "shot.h"
#include "star.h"
#include "system.h"
#include "visualization.h"
#include "visualizations.h"

System::System(void) :
    _Star(nullptr),
    _TrafficDensity(0.0f)
{
    // initialize object aspects
    AddAspectName();
    AddAspectObjectContainer();
    GetAspectObjectContainer()->ConnectContentAddedCallback(std::bind(&System::_OnAdded, this, std::placeholders::_1));
    GetAspectObjectContainer()->ConnectContentRemovedCallback(std::bind(&System::_OnRemoved, this, std::placeholders::_1));
    AddAspectPosition();
    AddAspectVisualization();
    GetAspectVisualization()->SetUpdateVisualizationCallback(std::bind(&System::_UpdateVisualization, this, std::placeholders::_1));
}

System::~System()
{
    assert(_Commodities.empty() == true);
    assert(_Planets.empty() == true);
    assert(_Ships.empty() == true);
    assert(_Shots.empty() == true);
    assert(_Star == nullptr);
}

auto System::AddFactionInfluence(Faction * Faction, float Influence) -> void
{
    assert(Faction != nullptr);
    
    auto const & [Iterator, Success] = m_FactionInfluences.try_emplace(Faction, Faction->ConnectDestroyingCallback(std::bind(&System::m_OnFactionDestroying, this, Faction)), Influence);
    
    assert(Success == true);
}

auto System::GetFactionInfluence(Faction * Faction) const -> float
{
    assert(Faction != nullptr);
    for(auto & [FactionInSystem, FactionInfluence] : m_FactionInfluences)
    {
        if(FactionInSystem == Faction)
        {
            return FactionInfluence.m_Influence;
        }
    }
    
    return 0.0f;
}

auto System::GetRandomFactionAccordingToInfluences() const -> Faction *
{
    auto TotalInfluence = 0.0f;
    
    for(auto & [FactionInSystem, FactionInfluence] : m_FactionInfluences)
    {
        TotalInfluence += FactionInfluence.m_Influence;
    }
    
    auto Random = GetRandomFloat(0.0f, TotalInfluence);
    
    for(auto & [FactionInSystem, FactionInfluence] : m_FactionInfluences)
    {
        Random -= FactionInfluence.m_Influence;
        if(Random <= 0.0f)
        {
            return FactionInSystem;
        }
    }
    
    return nullptr;
}

auto System::IsLinkedToSystem(System const * System) const -> bool
{
    return std::find(m_LinkedSystems.begin(), m_LinkedSystems.end(), System) != m_LinkedSystems.end();
}

void System::_OnAdded(Object * Content)
{
    assert(Content != nullptr);
    if(Content->GetTypeIdentifier() == "commodity")
    {
        auto TheCommodity(dynamic_cast< Commodity * >(Content));
        
        assert(TheCommodity != nullptr);
        _Commodities.push_back(TheCommodity);
        assert(GetAspectVisualization() != nullptr);
        for(auto Visualization : GetAspectVisualization()->GetVisualizations())
        {
            assert(Visualization->GetGraphics() != nullptr);
            VisualizeObject(TheCommodity, (static_cast< Graphics::SystemNode * >(Visualization->GetGraphics()))->GetCommodityLayer());
        }
    }
    else if(Content->GetTypeIdentifier() == "planet")
    {
        auto AddedPlanet{dynamic_cast< Planet * >(Content)};
        
        assert(AddedPlanet != nullptr);
        assert(std::find_if(_Planets.begin(), _Planets.end(), [&AddedPlanet](Planet * Planet) -> auto { return AddedPlanet->GetSubTypeIdentifier() == Planet->GetSubTypeIdentifier(); }) == _Planets.end());
        _Planets.push_back(AddedPlanet);
    }
    else if(Content->GetTypeIdentifier() == "ship")
    {
        auto TheShip(dynamic_cast< Ship * >(Content));
        
        assert(TheShip != nullptr);
        _Ships.push_back(TheShip);
        assert(GetAspectVisualization() != nullptr);
        for(auto Visualization : GetAspectVisualization()->GetVisualizations())
        {
            assert(Visualization->GetGraphics() != nullptr);
            VisualizeObject(TheShip, (static_cast< Graphics::SystemNode * >(Visualization->GetGraphics()))->GetShipLayer());
        }
    }
    else if(Content->GetTypeIdentifier() == "shot")
    {
        auto TheShot(dynamic_cast< Shot * >(Content));
        
        assert(TheShot != nullptr);
        _Shots.push_back(TheShot);
        assert(GetAspectVisualization() != nullptr);
        for(auto Visualization : GetAspectVisualization()->GetVisualizations())
        {
            assert(Visualization->GetGraphics() != nullptr);
            VisualizeObject(TheShot, (static_cast< Graphics::SystemNode * >(Visualization->GetGraphics()))->GetShotLayer());
        }
    }
    else if(Content->GetTypeIdentifier() == "star")
    {
        auto TheStar(dynamic_cast< Star * >(Content));
        
        assert(TheStar != nullptr);
        assert(_Star == nullptr);
        _Star = TheStar;
    }
    else
    {
        assert(false);
    }
}

auto System::m_OnFactionDestroying(Faction * Faction) -> void
{
    assert(Faction != nullptr);
    
    auto EraseCount = m_FactionInfluences.erase(Faction);
    
    assert(EraseCount == 1);
}

void System::_OnRemoved(Object * Content)
{
    assert(Content != nullptr);
    if(Content->GetTypeIdentifier() == "commodity")
    {
        auto CommodityIterator(std::find(_Commodities.begin(), _Commodities.end(), Content));
        
        assert(CommodityIterator != _Commodities.end());
        _Commodities.erase(CommodityIterator);
        assert(GetAspectVisualization() != nullptr);
        assert(Content->GetAspectVisualization() != nullptr);
        
        // this copy is needed, as we might modify the container
        auto CommodityVisualisations = Content->GetAspectVisualization()->GetVisualizations();
        
        for(auto CommodityVisualisation : CommodityVisualisations)
        {
            for(auto SystemVisualisation : GetAspectVisualization()->GetVisualizations())
            {
                if(CommodityVisualisation->GetGraphics()->GetContainer() == (dynamic_cast<Graphics::SystemNode *>(SystemVisualisation->GetGraphics()))->GetCommodityLayer())
                {
                    Content->GetAspectVisualization()->DestroyVisualization(CommodityVisualisation);
                    
                    break;
                }
            }
        }
    }
    else if(Content->GetTypeIdentifier() == "planet")
    {
        auto PlanetIterator(std::find(_Planets.begin(), _Planets.end(), Content));
        
        assert(PlanetIterator != _Planets.end());
        _Planets.erase(PlanetIterator);
        assert(GetAspectVisualization() != nullptr);
        assert(Content->GetAspectVisualization() != nullptr);
        
        // this copy is needed, as we might modify the container
        auto PlanetVisualisations = Content->GetAspectVisualization()->GetVisualizations();
        
        for(auto PlanetVisualisation : PlanetVisualisations)
        {
            for(auto SystemVisualisation : GetAspectVisualization()->GetVisualizations())
            {
                if(PlanetVisualisation->GetGraphics()->GetContainer() == (dynamic_cast<Graphics::SystemNode *>(SystemVisualisation->GetGraphics()))->GetPlanetLayer())
                {
                    Content->GetAspectVisualization()->DestroyVisualization(PlanetVisualisation);
                    
                    break;
                }
            }
        }
    }
    else if(Content->GetTypeIdentifier() == "ship")
    {
        auto ShipIterator(std::find(_Ships.begin(), _Ships.end(), Content));
        
        assert(ShipIterator != _Ships.end());
        _Ships.erase(ShipIterator);
        assert(GetAspectVisualization() != nullptr);
        assert(Content->GetAspectVisualization() != nullptr);
        
        // this copy is needed, as we might modify the container
        auto ShipVisualisations = Content->GetAspectVisualization()->GetVisualizations();
        
        for(auto ShipVisualisation : ShipVisualisations)
        {
            for(auto SystemVisualisation : GetAspectVisualization()->GetVisualizations())
            {
                if(ShipVisualisation->GetGraphics()->GetContainer() == (dynamic_cast<Graphics::SystemNode *>(SystemVisualisation->GetGraphics()))->GetShipLayer())
                {
                    Content->GetAspectVisualization()->DestroyVisualization(ShipVisualisation);
                    
                    break;
                }
            }
        }
    }
    else if(Content->GetTypeIdentifier() == "shot")
    {
        auto ShotIterator(std::find(_Shots.begin(), _Shots.end(), Content));
        
        assert(ShotIterator != _Shots.end());
        _Shots.erase(ShotIterator);
        assert(GetAspectVisualization() != nullptr);
        assert(Content->GetAspectVisualization() != nullptr);
        
        // this copy is needed, as we might modify the container
        auto ShotVisualisations = Content->GetAspectVisualization()->GetVisualizations();
        
        for(auto ShotVisualisation : ShotVisualisations)
        {
            for(auto SystemVisualisation : GetAspectVisualization()->GetVisualizations())
            {
                if(ShotVisualisation->GetGraphics()->GetContainer() == (dynamic_cast<Graphics::SystemNode *>(SystemVisualisation->GetGraphics()))->GetShotLayer())
                {
                    Content->GetAspectVisualization()->DestroyVisualization(ShotVisualisation);
                    
                    break;
                }
            }
        }
    }
    else if(Content->GetTypeIdentifier() == "star")
    {
        assert(_Star == Content);
        _Star = nullptr;
    }
    for(auto ShipIterator : _Ships)
    {
        if(ShipIterator->GetTarget() == Content)
        {
            ShipIterator->SetTarget(nullptr);
        }
    }
}

auto System::AddLinkedSystem(System * LinkedSystem) -> void
{
    m_LinkedSystems.push_back(LinkedSystem);
}

void System::_UpdateVisualization(Visualization * Visualization)
{
    assert(Visualization != nullptr);
    assert(Visualization->GetGraphics() != nullptr);
    assert(GetAspectPosition() != nullptr);
    if(Visualization->GetUpdateOrientation() == true)
    {
        Visualization->GetGraphics()->SetOrientation(GetAspectPosition()->GetOrientation());
    }
    if(Visualization->GetUpdatePosition() == true)
    {
        Visualization->GetGraphics()->SetPosition(GetAspectPosition()->GetPosition());
    }
}
