/**
 * galactic-fall
 * Copyright (C) 2015-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <arpa/inet.h>

#include <functional>
#include <iostream>
#include <utility>

#include <arx/archive.h>
#include <arx/buffer_reader.h>
#include <arx/item.h>

#include <graphics/engine.h>
#include <graphics/mesh.h>
#include <graphics/mesh_manager.h>
#include <graphics/model.h>
#include <graphics/model_manager.h>
#include <graphics/program.h>
#include <graphics/shader.h>
#include <graphics/shading_manager.h>
#include <graphics/style.h>
#include <graphics/texture.h>
#include <graphics/texture_manager.h>

#include <rasteriser/rasteriser.h>
#include <rasteriser/scene.h>

#include "arx_types.h"
#include "blueprint.h"
#include "blueprint_manager.h"
#include "buffer_reading.h"
#include "class_manager.h"
#include "faction.h"
#include "galaxy.h"
#include "globals.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_position.h"
#include "object_aspect_visualization.h"
#include "object_factory.h"
#include "planet.h"
#include "planet_assets.h"
#include "resource_reader.h"
#include "scenario.h"
#include "scenario_manager.h"
#include "settings.h"
#include "slot.h"
#include "slot_class.h"
#include "star.h"
#include "system.h"
#include "visualization_prototype.h"

static auto ReadAmmunitionClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void;
static auto ReadBatteryClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void;
static auto ReadCommodityClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void;
static auto ReadEnergyProjectileWeaponClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void;
static auto ReadFaction(ARX::Item & Item, Galaxy * Galaxy, std::list<std::tuple<std::string, std::string, float>> & FactionStandings) -> void;
static auto ReadGeneratorClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void;
static auto ReadMesh(ARX::Item & Item) -> void;
static auto ReadModel(ARX::Item & Item) -> void;
static auto ReadPlanet(ARX::Item & Item, Galaxy * Galaxy, System * System) -> void;
static auto ReadProgram(ARX::Item & Item, Graphics::ShadingManager * ShadingManager) -> void;
static auto ReadScenario(ARX::Item & Item, ScenarioManager * ScenarioManager) -> void;
static auto ReadShader(ARX::Item & Item, Graphics::ShadingManager * ShadingManager) -> void;
static auto ReadShipClass(ARX::Item & Item, BlueprintManager * BlueprintManager, ClassManager<SlotClass> * SlotClassManager) -> void;
static auto ReadSlotClass(ARX::Item & Item, ClassManager<SlotClass> * SlotClassManager) -> void;
static auto ReadStar(ARX::Item & Item, System * System) -> void;
static auto ReadSystemLink(ARX::Item & Item, System * System, std::multimap<std::string, std::string> & SystemLinks) -> void;
static auto ReadTexture(ARX::Item & Item) -> void;
static auto ReadTurretClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void;
static auto ReadVectorGraphics(ARX::Item & Item) -> void;

namespace GalacticFall
{
    namespace
    {
        auto CheckItemType(ARX::Item & Item, std::uint32_t Type) -> void
        {
            if(Item.GetType() != Type)
            {
                throw std::runtime_error("Item type for texture '" + Item.GetName() + "' should be '" + std::to_string(Type) + "' not '" + std::to_string(Item.GetType()) + "'.");
            }
        }
        
        auto CheckItemTypeAndSubType(ARX::Item & Item, std::uint32_t Type, std::uint32_t SubType) -> void
        {
            GalacticFall::CheckItemType(Item, Type);
            if(Item.GetSubType() != SubType)
            {
                throw std::runtime_error("Item sub type for texture '" + Item.GetName() + "' should be '" + std::to_string(SubType) + "' not '" + std::to_string(Item.GetSubType()) + "'.");
            }
        }
        
        auto FetchAndDecompress(ARX::Item & Item) -> void
        {
            if(Item.IsFetched() == false)
            {
                if(Item.Fetch() == false)
                {
                    throw std::runtime_error("Could not fetch data for item '" + Item.GetName() + "' [" + std::to_string(Item.GetIdentifier()) + "].");
                }
            }
            assert(Item.IsFetched() == true);
            if(Item.IsCompressed() == true)
            {
                Item.Decompress();
            }
            assert(Item.IsCompressed() == false);
        }
        
        auto ReadItems(ARX::Structure::Relation & Relation, std::function<auto (ARX::Item &) -> void> ReaderFunction) -> void
        {
            for(auto & ItemReference : Relation)
            {
                if(auto Item = ItemReference.GetItem(); Item != nullptr)
                {
                    GalacticFall::FetchAndDecompress(*Item);
                    ReaderFunction(*Item);
                }
                else
                {
                    throw std::runtime_error("The item identifier '" + std::to_string(ItemReference.GetItemIdentifier()) + "' is referenced but could not be resolved.");
                }
            }
        }
        
        auto ReadItems(ARX::Archive & Archive, std::string const & Path, std::function<auto (ARX::Item &) -> void> ReaderFunction) -> void
        {
            auto Directory = Archive.GetItem(Path);
            
            if(Directory != nullptr)
            {
                if(Directory->GetStructure().HasRelation("child") == true)
                {
                    GalacticFall::ReadItems(Directory->GetStructure().GetRelation("child"), ReaderFunction);
                }
                else
                {
                    throw std::runtime_error("The item '" + Path + "' does not contain a 'child' relation.");
                }
            }
            else
            {
                throw std::runtime_error("Could not find an item at the path '" + Path + "'.");
            }
        }
    }
}

auto ResourceReader::LoadArchive(std::filesystem::path const & Path) -> bool
{
    assert(m_Archive == nullptr);
    m_Archive = std::make_unique<ARX::Archive>();
    if(m_Archive->Load(Path) == false)
    {
        std::cerr << "Could not find or open \"" << Path << "\"." << std::endl;
        
        return false;
    }
    else
    {
        return true;
    }
}

auto ResourceReader::ReadAmmunitionClasses(BlueprintManager * BlueprintManager) -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Ammunition Classes", std::bind(ReadAmmunitionClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadBatteryClasses(BlueprintManager * BlueprintManager) -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Battery Classes", std::bind(ReadBatteryClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadCommodityClasses(BlueprintManager * BlueprintManager) -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Commodity Classes", std::bind(ReadCommodityClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadEnergyProjectileWeaponClasses(BlueprintManager * BlueprintManager) -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Energy Projectile Weapon Classes", std::bind(ReadEnergyProjectileWeaponClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadGalaxy(std::string const & GalaxyIdentifier) -> Galaxy *
{
    assert(m_Archive != nullptr);
    
    auto Path = std::string{"/Galaxies"};
    auto Directory = m_Archive->GetItem(Path);
    
    if(Directory == nullptr)
    {
        throw std::runtime_error("Could not find an item at the path '" + Path + "'.");
    }
    if(Directory->GetStructure().HasRelation("child") == false)
    {
        throw std::runtime_error("The item '" + Path + "' does not contain a 'child' relation.");
    }
    for(auto & ItemReference : Directory->GetStructure().GetRelation("child"))
    {
        if(auto Item = ItemReference.GetItem(); Item != nullptr)
        {
            GalacticFall::CheckItemTypeAndSubType(*Item, DATA_TYPE_GALAXY, 0);
            GalacticFall::FetchAndDecompress(*Item);
            
            auto Reader = ARX::BufferReader{*Item};
            auto Identifier = std::string{};
            
            Reader >> Identifier;
            if(Identifier == GalaxyIdentifier)
            {
                auto NewGalaxy = dynamic_cast<Galaxy *>(g_ObjectFactory->Create("galaxy", Identifier, false));
                auto Name = std::string{};
                
                Reader >> Name;
                assert(NewGalaxy->GetAspectName() != nullptr);
                NewGalaxy->GetAspectName()->SetName(Name);
                if(Item->GetStructure().HasRelation("factions") == false)
                {
                    throw std::runtime_error("The item '" + Path + "/" + Item->GetName() + "' does not contain a 'factions' relation.");
                }
                
                auto FactionStandings = std::list<std::tuple<std::string, std::string, float>>{};
                
                GalacticFall::ReadItems(Item->GetStructure().GetRelation("factions"), std::bind(ReadFaction, std::placeholders::_1, NewGalaxy, std::ref(FactionStandings)));
                for(auto & FactionStanding : FactionStandings)
                {
                    NewGalaxy->GetFaction(std::get<0>(FactionStanding))->AddFactionStanding(NewGalaxy->GetFaction(std::get<1>(FactionStanding)), std::get<2>(FactionStanding));
                }
                
                auto SystemLinks = std::multimap<std::string, std::string>{};
                
                GalacticFall::ReadItems(Item->GetStructure().GetRelation("systems"), std::bind(&ResourceReader::m_ReadSystem, this, std::placeholders::_1, NewGalaxy, std::ref(SystemLinks)));
                for(auto & SystemLink : SystemLinks)
                {
                    NewGalaxy->GetSystem(SystemLink.first)->AddLinkedSystem(NewGalaxy->GetSystem(SystemLink.second));
                }
                
                return NewGalaxy;
            }
        }
        else
        {
            throw std::runtime_error{"The item identifier '" + std::to_string(ItemReference.GetItemIdentifier()) + "' is referenced but could not be resolved."};
        }
    }
    
    return nullptr;
}

auto ResourceReader::ReadGeneratorClasses(BlueprintManager * BlueprintManager) -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Generator Classes", std::bind(ReadGeneratorClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadMeshes() -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Meshes", ReadMesh);
}

auto ResourceReader::ReadModels() -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Models", ReadModel);
}

auto ResourceReader::ReadScenarios(ScenarioManager * ScenarioManager) -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Scenarios", std::bind(ReadScenario, std::placeholders::_1, ScenarioManager));
}

auto ResourceReader::ReadSettings(Settings * Settings) -> void
{
    if(auto SettingsItem = m_Archive->GetItem("/Settings"); SettingsItem != nullptr)
    {
        if(SettingsItem->GetStructure().HasRelation("child") == true)
        {
            auto & ChildRelation = SettingsItem->GetStructure().GetRelation("child");
            // read key binding profile
            auto KeyBindingProfileItems = ChildRelation.GetItems("Key Binding Profile");
            
            if(KeyBindingProfileItems.size() > 0)
            {
                auto KeyBindingProfileItem = KeyBindingProfileItems.front();
                
                GalacticFall::CheckItemTypeAndSubType(*KeyBindingProfileItem, DATA_TYPE_KEY_BINDING_PROFILE, 0);
                GalacticFall::FetchAndDecompress(*KeyBindingProfileItem);
                
                auto BufferReader = GalacticFall::BufferReader{*KeyBindingProfileItem};
                auto KeyBindingCount = BufferReader.ReadUnsignedInteger32Bit();
                auto KeyBindings = std::list<Settings::KeyBinding>{};
                
                for(auto KeyBindingIndex = std::uint32_t{0}; KeyBindingIndex < KeyBindingCount; ++KeyBindingIndex)
                {
                    auto Code = BufferReader.ReadUnsignedInteger32Bit();
                    auto Event = BufferReader.ReadString();
                    auto Action = BufferReader.ReadString();
                    
                    KeyBindings.push_back(Settings::KeyBinding{Code, Event, Action});
                }
                Settings->SetKeyBindings(std::move(KeyBindings));
            }
            
            // read window dimensions
            auto WindowDimensionsItems = ChildRelation.GetItems("Window Dimensions");
            
            if(WindowDimensionsItems.size() > 0)
            {
                auto WindowDimensionsItem = WindowDimensionsItems.front();
                
                GalacticFall::CheckItemTypeAndSubType(*WindowDimensionsItem, DATA_TYPE_DIMENSIONS, DATA_TYPE_DIMENSIONS_SUB_TYPE_DIMENSIONS2F);
                GalacticFall::FetchAndDecompress(*WindowDimensionsItem);
                
                auto BufferReader = GalacticFall::BufferReader{*WindowDimensionsItem};
                
                Settings->SetWindowDimensions(BufferReader.ReadVector2f());
            }
        }
        else
        {
            throw std::runtime_error("The item '" + SettingsItem->GetName() + "' does not contain a 'child' relation.");
        }
    }
    else
    {
        throw std::runtime_error("Could not find an item at the path '/Settings'.");
    }
}

auto ResourceReader::ReadShadersAndPrograms(Graphics::ShadingManager * ShadingManager) -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Shaders", std::bind(ReadShader, std::placeholders::_1, ShadingManager));
    GalacticFall::ReadItems(*m_Archive, "/Programs", std::bind(ReadProgram, std::placeholders::_1, ShadingManager));
}

auto ResourceReader::ReadShipClasses(BlueprintManager * BlueprintManager, ClassManager<SlotClass> * SlotClassManager) -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Ship Classes", std::bind(ReadShipClass, std::placeholders::_1, BlueprintManager, SlotClassManager));
}

auto ResourceReader::ReadSlotClasses(ClassManager<SlotClass> * SlotClassManager) -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Slot Classes", std::bind(ReadSlotClass, std::placeholders::_1, SlotClassManager));
}

auto ResourceReader::m_ReadSystem(ARX::Item & Item, Galaxy * Galaxy, std::multimap<std::string, std::string> & SystemLinks) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_SYSTEM, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewSystem = dynamic_cast<System *>(g_ObjectFactory->Create("system", Identifier, false));
    
    if(NewSystem == nullptr)
    {
        throw std::runtime_error("Could not create system '" + Identifier + "'.");
    }
    NewSystem->GetAspectName()->SetName(BufferReader.ReadString());
    
    auto Position = BufferReader.ReadVector2f();
    
    NewSystem->GetAspectPosition()->SetPosition(Vector3f::CreateFromComponents(Position[0], Position[1], 0.0f));
    NewSystem->SetTrafficDensity(BufferReader.ReadFloat());
    
    auto FactionInfluencesCount = BufferReader.ReadUnsignedInteger32Bit();
    
    for(auto FactionInfluenceIndex = std::uint32_t{0}; FactionInfluenceIndex < FactionInfluencesCount; ++FactionInfluenceIndex)
    {
        auto FactionIdentifier = BufferReader.ReadString();
        auto Faction = Galaxy->GetFaction(FactionIdentifier);
        
        if(Faction == nullptr)
        {
            throw std::runtime_error("Could not find faction '" + FactionIdentifier + "'.");
        }
        NewSystem->AddFactionInfluence(Faction, BufferReader.ReadFloat());
    }
    Galaxy->GetAspectObjectContainer()->AddContent(NewSystem);
    if(Item.GetStructure().HasRelation("stars") == true)
    {
        GalacticFall::ReadItems(Item.GetStructure().GetRelation("stars"), std::bind(ReadStar, std::placeholders::_1, NewSystem));
    }
    if(Item.GetStructure().HasRelation("planets") == true)
    {
        GalacticFall::ReadItems(Item.GetStructure().GetRelation("planets"), std::bind(ReadPlanet, std::placeholders::_1, Galaxy, NewSystem));
    }
    if(Item.GetStructure().HasRelation("linked-systems") == true)
    {
        GalacticFall::ReadItems(Item.GetStructure().GetRelation("linked-systems"), std::bind(ReadSystemLink, std::placeholders::_1, NewSystem, std::ref(SystemLinks)));
    }
}

auto ResourceReader::ReadTextures() -> void
{
    assert(m_Archive != nullptr);
    
    auto Directory(m_Archive->GetItem("/Textures"));
    
    if(Directory == nullptr)
    {
        throw std::runtime_error("Could not find an item at the path '/Textures'.");
    }
    GalacticFall::ReadItems(Directory->GetStructure().GetRelation("tex"), ReadTexture);
    GalacticFall::ReadItems(Directory->GetStructure().GetRelation("vg"), ReadVectorGraphics);
}

auto ResourceReader::ReadTurretClasses(BlueprintManager * BlueprintManager) -> void
{
    assert(m_Archive != nullptr);
    GalacticFall::ReadItems(*m_Archive, "/Turret Classes", std::bind(ReadTurretClass, std::placeholders::_1, BlueprintManager));
}

auto ResourceReader::ReadSavegameFromScenarioPath(std::string const & ScenarioPath) -> std::string
{
    auto Result = std::string{};
    
    if(auto Item = m_Archive->GetItem(ScenarioPath); Item != nullptr)
    {
        GalacticFall::FetchAndDecompress(*Item);
        
        auto BufferReader = GalacticFall::BufferReader{*Item};
        auto Identifier = BufferReader.ReadString();
        auto Name = BufferReader.ReadString();
        auto Description = BufferReader.ReadString();
        
        Result = BufferReader.ReadString();
    }
    
    return Result;
}

static auto ReadAmmunitionClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_AMMUNITION_CLASS, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewAmmunitionClass = BlueprintManager->Create("ammunition", Identifier);
    
    if(NewAmmunitionClass == nullptr)
    {
        throw std::runtime_error("Could not create ammunition class '" + Identifier + "'.");
    }
    NewAmmunitionClass->AddProperty("name", BufferReader.ReadString());
    NewAmmunitionClass->AddProperty("description", BufferReader.ReadString());
    NewAmmunitionClass->AddProperty("base-price", BufferReader.ReadUnsignedInteger32Bit());
    NewAmmunitionClass->AddProperty("space-requirement", BufferReader.ReadUnsignedInteger32Bit());
    NewAmmunitionClass->AddProperty("visualization-prototype", BufferReader.ReadVisualisationPrototype());
    NewAmmunitionClass->AddProperty("cartridge-size", BufferReader.ReadUnsignedInteger32Bit());
}

static auto ReadBatteryClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_BATTERY_CLASS, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewBatteryClass = BlueprintManager->Create("battery", Identifier);
    
    if(NewBatteryClass == nullptr)
    {
        throw std::runtime_error("Could not create battery class '" + Identifier + "'.");
    }
    NewBatteryClass->AddProperty("name", BufferReader.ReadString());
    NewBatteryClass->AddProperty("description", BufferReader.ReadString());
    NewBatteryClass->AddProperty("base-price", BufferReader.ReadUnsignedInteger32Bit());
    NewBatteryClass->AddProperty("space-requirement", BufferReader.ReadUnsignedInteger32Bit());
    NewBatteryClass->AddProperty("visualization-prototype", BufferReader.ReadVisualisationPrototype());
    NewBatteryClass->AddProperty("energy-capacity", BufferReader.ReadFloat());
    NewBatteryClass->AddProperty("maximum-power-input", BufferReader.ReadFloat());
    NewBatteryClass->AddProperty("maximum-power-output", BufferReader.ReadFloat());
    NewBatteryClass->AddProperty("slot-class-identifier", BufferReader.ReadString());
}

static auto ReadCommodityClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_COMMODITY_CLASS, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewCommodityClass = BlueprintManager->Create("commodity", Identifier);
    
    if(NewCommodityClass == nullptr)
    {
        throw std::runtime_error("Could not create commodity class '" + Identifier + "'.");
    }
    NewCommodityClass->AddProperty("name", BufferReader.ReadString());
    NewCommodityClass->AddProperty("description", BufferReader.ReadString());
    NewCommodityClass->AddProperty("base-price", BufferReader.ReadUnsignedInteger32Bit());
    NewCommodityClass->AddProperty("space-requirement", BufferReader.ReadUnsignedInteger32Bit());
    NewCommodityClass->AddProperty("visualization-prototype", BufferReader.ReadVisualisationPrototype());
}

static auto ReadEnergyProjectileWeaponClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_ENERGY_PROJECTILE_WEAPON_CLASS, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewEnergyProjectileWeaponBlueprint = BlueprintManager->Create("energy-projectile-weapon", Identifier);
    
    if(NewEnergyProjectileWeaponBlueprint == nullptr)
    {
        throw std::runtime_error("Could not create energy projectile weapon blueprint '" + Identifier + "'.");
    }
    NewEnergyProjectileWeaponBlueprint->AddProperty("name", BufferReader.ReadString());
    NewEnergyProjectileWeaponBlueprint->AddProperty("description", BufferReader.ReadString());
    NewEnergyProjectileWeaponBlueprint->AddProperty("base-price", BufferReader.ReadUnsignedInteger32Bit());
    NewEnergyProjectileWeaponBlueprint->AddProperty("space-requirement", BufferReader.ReadUnsignedInteger32Bit());
    
    auto EnergyProjectileWeaponVisualisationPrototype = BufferReader.ReadVisualisationPrototype();
    
    NewEnergyProjectileWeaponBlueprint->AddProperty("visualization-prototype", EnergyProjectileWeaponVisualisationPrototype);
    NewEnergyProjectileWeaponBlueprint->AddProperty("slot-class-identifier", BufferReader.ReadString());
    NewEnergyProjectileWeaponBlueprint->AddProperty("orientation", BufferReader.ReadQuaternion());
    NewEnergyProjectileWeaponBlueprint->AddProperty("maximum-power-input", BufferReader.ReadFloat());
    NewEnergyProjectileWeaponBlueprint->AddProperty("maximum-power-output", BufferReader.ReadFloat());
    NewEnergyProjectileWeaponBlueprint->AddProperty("energy-usage-per-shot", BufferReader.ReadFloat());
    
    auto MuzzlePositionPartIdentifier = BufferReader.ReadString();
    auto MuzzlePositionMarkerIdentifier = BufferReader.ReadString();
    auto MuzzlePosition = EnergyProjectileWeaponVisualisationPrototype.GetMarkerPosition(MuzzlePositionPartIdentifier, MuzzlePositionMarkerIdentifier);
    
    if(MuzzlePosition == nullptr)
    {
        throw std::runtime_error("For the energy projectile weapon '" + Identifier + "', could not find a marker or its position for the muzzle position '" + MuzzlePositionMarkerIdentifier + "' on the part '" + MuzzlePositionPartIdentifier + "'.");
    }
    NewEnergyProjectileWeaponBlueprint->AddProperty("muzzle-position", *MuzzlePosition);
    NewEnergyProjectileWeaponBlueprint->AddProperty("shot-exit-speed", BufferReader.ReadFloat());
    NewEnergyProjectileWeaponBlueprint->AddProperty("shot-damage", BufferReader.ReadFloat());
    NewEnergyProjectileWeaponBlueprint->AddProperty("shot-life-time", BufferReader.ReadFloat());
    NewEnergyProjectileWeaponBlueprint->AddProperty("shot-visualization-prototype", BufferReader.ReadVisualisationPrototype());
}

static auto ReadFaction(ARX::Item & Item, Galaxy * Galaxy, std::list<std::tuple<std::string, std::string, float>> & FactionStandings) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_FACTION, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewFaction = dynamic_cast<Faction *>(g_ObjectFactory->Create("faction", Identifier, false));
    
    if(NewFaction == nullptr)
    {
        throw std::runtime_error("Could not create faction '" + Identifier + "'.");
    }
    NewFaction->GetAspectName()->SetName(BufferReader.ReadString());
    NewFaction->SetColor(BufferReader.ReadColourRGBO());
    
    auto StandingsCount = BufferReader.ReadUnsignedInteger32Bit();
    
    for(auto StandingsIndex = std::uint32_t{0}; StandingsIndex < StandingsCount; ++StandingsIndex)
    {
        auto FactionIdentifier = BufferReader.ReadString();
        auto FactionStanding = BufferReader.ReadFloat();
        
        FactionStandings.push_back(std::make_tuple(Identifier, FactionIdentifier, FactionStanding));
    }
    Galaxy->GetAspectObjectContainer()->AddContent(NewFaction);
}

static auto ReadGeneratorClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_GENERATOR_CLASS, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewGeneratorClass = BlueprintManager->Create("generator", Identifier);
    
    if(NewGeneratorClass == nullptr)
    {
        throw std::runtime_error("Could not create generator class '" + Identifier + "'.");
    }
    NewGeneratorClass->AddProperty("name", BufferReader.ReadString());
    NewGeneratorClass->AddProperty("description", BufferReader.ReadString());
    NewGeneratorClass->AddProperty("base-price", BufferReader.ReadUnsignedInteger32Bit());
    NewGeneratorClass->AddProperty("space-requirement", BufferReader.ReadUnsignedInteger32Bit());
    NewGeneratorClass->AddProperty("visualization-prototype", BufferReader.ReadVisualisationPrototype());
    NewGeneratorClass->AddProperty("maximum-power-output", BufferReader.ReadFloat());
    NewGeneratorClass->AddProperty("slot-class-identifier", BufferReader.ReadString());
}

static auto ReadMesh(ARX::Item & Item) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_MESH, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewMesh = g_GraphicsEngine->GetMeshManager()->Create(Identifier);
    
    if(NewMesh == nullptr)
    {
        throw std::runtime_error("Could not create mesh '" + Identifier + "'.");
    }
    
    auto Points = std::map<std::string, std::vector<Vector3f>::size_type>{};
    auto PointCount = BufferReader.ReadUnsignedInteger32Bit();
    
    for(auto PointIndex = std::uint32_t{0}; PointIndex < PointCount; ++PointIndex)
    {
        auto PointIdentifier = BufferReader.ReadString();
        auto PointName = BufferReader.ReadString();
        auto PointCoordinates = BufferReader.ReadVector3f();
        
        Points[PointIdentifier] = NewMesh->AddPoint(PointCoordinates);
    }
    
    auto TrianglePoints = std::map<std::string, std::pair<std::vector<Vector3f >::size_type, Vector3f>>{};
    auto TrianglePointCount = BufferReader.ReadUnsignedInteger32Bit();
    
    for(auto TrianglePointIndex = std::uint32_t{0}; TrianglePointIndex < TrianglePointCount; ++TrianglePointIndex)
    {
        auto TrianglePointIdentifier = BufferReader.ReadString();
        auto TrianglePointNormal = BufferReader.ReadVector3f();
        auto PointIdentifier = BufferReader.ReadString();
        
        TrianglePoints[TrianglePointIdentifier] = std::make_pair(Points[PointIdentifier], TrianglePointNormal);
    }
    
    auto TriangleCount = BufferReader.ReadUnsignedInteger32Bit();
    
    for(auto TriangleIndex = std::uint32_t{0}; TriangleIndex < TriangleCount; ++TriangleIndex)
    {
        auto TriangleIdentifier = BufferReader.ReadString();
        auto TriangleName = BufferReader.ReadString();
        auto TrianglePoint1Identifier = BufferReader.ReadString();
        auto TrianglePoint2Identifier = BufferReader.ReadString();
        auto TrianglePoint3Identifier = BufferReader.ReadString();
        
        NewMesh->AddTriangle(TrianglePoints[TrianglePoint1Identifier].first, TrianglePoints[TrianglePoint1Identifier].second, TrianglePoints[TrianglePoint2Identifier].first, TrianglePoints[TrianglePoint2Identifier].second, TrianglePoints[TrianglePoint3Identifier].first, TrianglePoints[TrianglePoint3Identifier].second);
    }
    
    auto MarkerCount = BufferReader.ReadUnsignedInteger32Bit();
    
    for(auto MarkerIndex = std::uint32_t{0}; MarkerIndex < MarkerCount; ++MarkerIndex)
    {
        auto MarkerIdentifier = BufferReader.ReadString();
        auto MarkerLengthValid = BufferReader.ReadBoolean();
        auto MarkerLength = BufferReader.ReadFloat();
        auto MarkerPositionValid = BufferReader.ReadBoolean();
        auto MarkerPosition = BufferReader.ReadVector3f();
        auto MarkerOrientationValid = BufferReader.ReadBoolean();
        auto MarkerOrientation = BufferReader.ReadQuaternion();
        
        NewMesh->AddMarker(MarkerIdentifier);
        if(MarkerLengthValid == true)
        {
            NewMesh->SetMarkerLength(MarkerIdentifier, MarkerLength);
        }
        if(MarkerPositionValid == true)
        {
            NewMesh->SetMarkerPosition(MarkerIdentifier, MarkerPosition);
        }
        if(MarkerOrientationValid == true)
        {
            NewMesh->SetMarkerOrientation(MarkerIdentifier, MarkerOrientation);
        }
    }
    NewMesh->BuildVertexArray();
}

static auto ReadModel(ARX::Item & Item) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_MODEL, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewModel = g_GraphicsEngine->GetModelManager()->Create(Identifier);
    
    if(NewModel == nullptr)
    {
        throw std::runtime_error("Could not create model '" + Identifier + "'.");
    }
    
    auto PartCount = BufferReader.ReadUnsignedInteger32Bit();
    
    for(auto PartIndex = std::uint32_t{0}; PartIndex < PartCount; ++PartIndex)
    {
        auto PartIdentifier = BufferReader.ReadString();
        auto PartMeshIdentifier = BufferReader.ReadString();
        auto PartPosition = BufferReader.ReadVector3f();
        auto PartOrientation = BufferReader.ReadQuaternion();
        auto PartMesh = g_GraphicsEngine->GetMeshManager()->Get(PartMeshIdentifier);
        
        if(PartMesh == nullptr)
        {
            throw std::runtime_error("For model '" + Identifier + "' could not find the mesh '" + PartMeshIdentifier + "' for part '" + PartIdentifier + "'.");
        }
        NewModel->AddPart(PartIdentifier, PartMesh, PartPosition, PartOrientation);
    }
}

static auto ReadPlanet(ARX::Item & Item, Galaxy * Galaxy, System * System) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_PLANET, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewPlanet = dynamic_cast<Planet *>(g_ObjectFactory->Create("planet", Identifier, false));
    
    if(NewPlanet == nullptr)
    {
        throw std::runtime_error("Could not create planet '" + Identifier + "'.");
    }
    NewPlanet->GetAspectName()->SetName(BufferReader.ReadString());
    NewPlanet->SetDescription(BufferReader.ReadString());
    assert(NewPlanet->GetAspectVisualization() != nullptr);
    assert(NewPlanet->GetAspectVisualization()->GetVisualizationPrototype() == nullptr);
    NewPlanet->GetAspectVisualization()->SetVisualizationPrototype(BufferReader.ReadVisualisationPrototype());
    
    auto PlanetPosition = BufferReader.ReadVector2f();
    
    NewPlanet->GetAspectPosition()->SetPosition(Vector3f::CreateFromComponents(PlanetPosition[0], PlanetPosition[1], 0.0f));
    NewPlanet->SetSize(BufferReader.ReadFloat());
    
    auto PlanetAssetsCount = BufferReader.ReadUnsignedInteger32Bit();
    
    for(auto PlanetAssetsIndex = std::uint32_t{0}; PlanetAssetsIndex < PlanetAssetsCount; ++PlanetAssetsIndex)
    {
        auto AssetsTypeIdentifier = BufferReader.ReadString();
        auto AssetsSubTypeIdentifier = BufferReader.ReadString();
        auto BasePriceModifier = BufferReader.ReadFloat();
        
        if(g_BlueprintManager->Has(AssetsTypeIdentifier, AssetsSubTypeIdentifier) == true)
        {
            auto NewPlanetAssets = NewPlanet->CreatePlanetAssets(AssetsTypeIdentifier, AssetsSubTypeIdentifier);
            
            NewPlanetAssets->SetBasePriceModifier(BasePriceModifier);
        }
        else
        {
            throw std::runtime_error("For the assets of planet \"" + Identifier + "\", don't know a class of type '" + AssetsTypeIdentifier + "' and sub type '" + AssetsSubTypeIdentifier + "'.");
        }
    }
    NewPlanet->SetLandingFeePerSpace(BufferReader.ReadFloat() / 1000.0f);
    NewPlanet->SetOffersRecharging(BufferReader.ReadBoolean());
    NewPlanet->SetRechargingFeePerEnergy(BufferReader.ReadFloat());
    NewPlanet->SetOffersRepairing(BufferReader.ReadBoolean());
    NewPlanet->SetRepairingFeePerHull(BufferReader.ReadFloat());
    
    auto FactionIdentifier = BufferReader.ReadString();
    auto Faction = Galaxy->GetFaction(FactionIdentifier);
    
    if(Faction == nullptr)
    {
        throw std::runtime_error("Could not find faction '" + FactionIdentifier + "' for planet '" + Identifier + "' in system '" + System->GetSubTypeIdentifier() + "'.");
    }
    NewPlanet->SetFaction(Faction);
    System->GetAspectObjectContainer()->AddContent(NewPlanet);
}

static auto ReadProgram(ARX::Item & Item, Graphics::ShadingManager * ShadingManager) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_SHADER_PROGRAM, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto Program = ShadingManager->CreateProgram(Identifier);
    
    if(Program == nullptr)
    {
        throw std::runtime_error("Could not create program '" + Identifier + "'.");
    }
    
    auto ShaderCount = BufferReader.ReadUnsignedInteger32Bit();
    
    for(auto ShaderIndex = std::uint32_t{0}; ShaderIndex < ShaderCount; ++ShaderIndex)
    {
        Program->AddShaderIdentifier(BufferReader.ReadString());
    }
}

static auto ReadScenario(ARX::Item & Item, ScenarioManager * ScenarioManager) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_SCENARIO, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewScenario = ScenarioManager->Create(Identifier);
    
    if(NewScenario == nullptr)
    {
        throw std::runtime_error("Could not create scenario '" + Identifier + "'.");
    }
    NewScenario->SetResourcePath("/Scenarios/" + Item.GetName());
    NewScenario->SetName(BufferReader.ReadString());
    NewScenario->SetDescription(BufferReader.ReadString());
}

static auto ReadShader(ARX::Item & Item, Graphics::ShadingManager * ShadingManager) -> void
{
    GalacticFall::CheckItemType(Item, DATA_TYPE_SHADER);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto Shader = ShadingManager->CreateShader(Identifier);
    
    if(Shader == nullptr)
    {
        throw std::runtime_error("Could not create shader '" + Identifier + "'.");
    }
    if(Item.GetSubType() == DATA_TYPE_SHADER_SUB_TYPE_VERTEX_SHADER)
    {
        Shader->SetType(Graphics::Shader::Type::VertexShader);
    }
    else if(Item.GetSubType() == DATA_TYPE_SHADER_SUB_TYPE_FRAGMENT_SHADER)
    {
        Shader->SetType(Graphics::Shader::Type::FragmentShader);
    }
    else
    {
        throw std::runtime_error("Unknown sub type '" + std::to_string(Item.GetType()) + "' for shader '" + Item.GetName() + ".");
    }
    Shader->SetSource(BufferReader.ReadString());
}

static auto ReadShipClass(ARX::Item & Item, BlueprintManager * BlueprintManager, ClassManager<SlotClass> * SlotClassManager) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_SHIP_CLASS, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewShipClass = BlueprintManager->Create("ship", Identifier);
    
    if(NewShipClass == nullptr)
    {
        throw std::runtime_error("Could not create ship class '" + Identifier + "'.");
    }
    NewShipClass->AddProperty("name", BufferReader.ReadString());
    NewShipClass->AddProperty("description", BufferReader.ReadString());
    NewShipClass->AddProperty("base-price", BufferReader.ReadUnsignedInteger32Bit());
    NewShipClass->AddProperty("space-requirement", BufferReader.ReadUnsignedInteger32Bit());
    
    auto VisualisationPrototype = BufferReader.ReadVisualisationPrototype();
    
    NewShipClass->AddProperty("visualization-prototype", VisualisationPrototype);
    NewShipClass->AddProperty("forward-thrust", BufferReader.ReadFloat());
    NewShipClass->AddProperty("turn-speed", BufferReader.ReadFloat());
    NewShipClass->AddProperty("maximum-speed", BufferReader.ReadFloat());
    NewShipClass->AddProperty("maximum-available-space", BufferReader.ReadUnsignedInteger32Bit());
    NewShipClass->AddProperty("fuel-capacity", BufferReader.ReadFloat());
    NewShipClass->AddProperty("jump-fuel", BufferReader.ReadFloat());
    NewShipClass->AddProperty("forward-fuel", BufferReader.ReadFloat());
    NewShipClass->AddProperty("turn-fuel", BufferReader.ReadFloat());
    NewShipClass->AddProperty("hull", BufferReader.ReadFloat());
    NewShipClass->AddProperty("exhaust-marker-part-identifier", BufferReader.ReadString());
    NewShipClass->AddProperty("exhaust-marker-identifier", BufferReader.ReadString());
    
    auto SlotCount = BufferReader.ReadUnsignedInteger32Bit();
    auto Slots = std::list<Properties>{};
    
    for(auto SlotIndex = std::uint32_t{0}; SlotIndex < SlotCount; ++SlotIndex)
    {
        auto NewSlot = Properties{};
        auto SlotIdentifier = BufferReader.ReadString();
        
        NewSlot.AddProperty("identifier", SlotIdentifier);
        
        auto SlotClassIdentifier = BufferReader.ReadString();
        auto SlotClass = SlotClassManager->Get(SlotClassIdentifier);
        
        if(SlotClass == nullptr)
        {
            throw std::runtime_error("Could not get slot class '" + SlotClassIdentifier + "' for slot '" + SlotIdentifier + "' of ship class '" + Identifier + "'.");
        }
        NewSlot.AddProperty("class", SlotClass);
        NewSlot.AddProperty("name", BufferReader.ReadString());
        
        auto SlotMarkerPartIdentifier = BufferReader.ReadString();
        auto SlotMarkerIdentifier = BufferReader.ReadString();
        auto SlotPosition = VisualisationPrototype.GetMarkerPosition(SlotMarkerPartIdentifier, SlotMarkerIdentifier);

        if(SlotPosition == nullptr)
        {
            throw std::runtime_error("For the ship '" + Identifier + "' and slot '" + SlotIdentifier + "', could not find the marker or its position for the slot position '" + SlotMarkerIdentifier + "' on the part '" + SlotMarkerPartIdentifier + "'.");
        }
        NewSlot.AddProperty("position", *SlotPosition);
    
        auto SlotOrientation = VisualisationPrototype.GetMarkerOrientation(SlotMarkerPartIdentifier, SlotMarkerIdentifier);

        if(SlotOrientation == nullptr)
        {
            throw std::runtime_error("For the ship '" + Identifier + "' and slot '" + SlotIdentifier + "', could not find the marker or its orientation for the slot position '" + SlotMarkerIdentifier + "' on the part '" + SlotMarkerPartIdentifier + "'.");
        }
        NewSlot.AddProperty("orientation", *SlotOrientation);
        NewSlot.AddProperty("visualize-accessory", BufferReader.ReadBoolean());
        Slots.push_back(NewSlot);
    }
    NewShipClass->AddProperty("slots", Slots);
}

static auto ReadSlotClass(ARX::Item & Item, ClassManager<SlotClass> * SlotClassManager) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_SLOT_CLASS, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewSlotClass = SlotClassManager->Create(Identifier);
    
    if(NewSlotClass == nullptr)
    {
        throw std::runtime_error("Could not create slot class '" + Identifier + "'.");
    }
    NewSlotClass->SetName(BufferReader.ReadString());
    
    auto AcceptedSlotClassIdentifierCount = BufferReader.ReadUnsignedInteger32Bit();
    
    for(auto AcceptedSlotClassIdentifierIndex = std::uint32_t{0}; AcceptedSlotClassIdentifierIndex < AcceptedSlotClassIdentifierCount; ++AcceptedSlotClassIdentifierIndex)
    {
        NewSlotClass->AddAcceptedSlotClassIdentifier(BufferReader.ReadString());
    }
}

static auto ReadStar(ARX::Item & Item, System * System) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_STAR, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewStar = dynamic_cast<Star *>(g_ObjectFactory->Create("star", Identifier, false));
    
    if(NewStar == nullptr)
    {
        throw std::runtime_error("Could not create star '" + Identifier + "'.");
    }
    
    auto Position = BufferReader.ReadVector2f();
    
    NewStar->GetAspectPosition()->SetPosition(Vector3f::CreateFromComponents(Position[0], Position[1], 0.0f));
    NewStar->SetColor(BufferReader.ReadColourRGB());
    System->GetAspectObjectContainer()->AddContent(NewStar);
}

static auto ReadSystemLink(ARX::Item & Item, System * System, std::multimap<std::string, std::string> & SystemLinks) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_SYSTEM, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    
    SystemLinks.insert(std::make_pair(System->GetSubTypeIdentifier(), BufferReader.ReadString()));
}

static auto ReadTexture(ARX::Item & Item) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_TEXTURE, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto Texture = g_GraphicsEngine->GetTextureManager()->Create(Identifier);
    
    if(Texture == nullptr)
    {
        throw std::runtime_error("Could not create texture '" + Identifier + "'.");
    }
    
    auto Width = ntohl(BufferReader.ReadUnsignedInteger32Bit());
    auto Height = ntohl(BufferReader.ReadUnsignedInteger32Bit());
    auto Format = ntohl(BufferReader.ReadUnsignedInteger32Bit());
    
    Texture->SetData(Width, Height, Format, BufferReader.GetCurrentData());
}

static auto ReadTurretClass(ARX::Item & Item, BlueprintManager * BlueprintManager) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_TURRET_CLASS, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto Identifier = BufferReader.ReadString();
    auto NewTurretClass = BlueprintManager->Create("turret", Identifier);
    
    if(NewTurretClass == nullptr)
    {
        throw std::runtime_error("Could not create turret class '" + Identifier + "'.");
    }
    NewTurretClass->AddProperty("name", BufferReader.ReadString());
    NewTurretClass->AddProperty("description", BufferReader.ReadString());
    NewTurretClass->AddProperty("base-price", BufferReader.ReadUnsignedInteger32Bit());
    NewTurretClass->AddProperty("space-requirement", BufferReader.ReadUnsignedInteger32Bit());
    
    auto TurretVisualisationPrototype = BufferReader.ReadVisualisationPrototype();
    
    NewTurretClass->AddProperty("visualization-prototype", TurretVisualisationPrototype);
    NewTurretClass->AddProperty("slot-class-identifier", BufferReader.ReadString());
    NewTurretClass->AddProperty("orientation", BufferReader.ReadQuaternion());
    NewTurretClass->AddProperty("maximum-power-input", BufferReader.ReadFloat());
    NewTurretClass->AddProperty("maximum-power-output", BufferReader.ReadFloat());
    NewTurretClass->AddProperty("energy-usage-per-shot", BufferReader.ReadFloat());
    
    auto MuzzlePositionPartIdentifier = BufferReader.ReadString();
    auto MuzzlePositionMarkerIdentifier = BufferReader.ReadString();
    auto MuzzlePosition = TurretVisualisationPrototype.GetMarkerPosition(MuzzlePositionPartIdentifier, MuzzlePositionMarkerIdentifier);
    
    if(MuzzlePosition == nullptr)
    {
        throw std::runtime_error("For the turret '" + Identifier + "', could not find a marker or its position for the muzzle position '" + MuzzlePositionMarkerIdentifier + "' on the part '" + MuzzlePositionPartIdentifier + "'.");
    }
    NewTurretClass->AddProperty("muzzle-position", *MuzzlePosition);
    NewTurretClass->AddProperty("shot-exit-speed", BufferReader.ReadFloat());
    NewTurretClass->AddProperty("shot-damage", BufferReader.ReadFloat());
    NewTurretClass->AddProperty("shot-life-time", BufferReader.ReadFloat());
    NewTurretClass->AddProperty("shot-visualization-prototype", BufferReader.ReadVisualisationPrototype());
}

static auto ReadVectorGraphics(ARX::Item & Item) -> void
{
    GalacticFall::CheckItemTypeAndSubType(Item, DATA_TYPE_VECTOR_GRAPHICS, 0);
    
    auto BufferReader = GalacticFall::BufferReader{Item};
    auto XML = BufferReader.ReadString();
    auto Width = BufferReader.ReadUnsignedInteger32Bit();
    auto Height = BufferReader.ReadUnsignedInteger32Bit();
    auto SubPixels = BufferReader.ReadUnsignedInteger32Bit();
    auto TextureIdentifier = BufferReader.ReadString();
    auto SceneOrError = Rasteriser::ParseScene(XML);
    
    if(SceneOrError.has_value() == false)
    {
        throw std::runtime_error{"Could not parse scene for vector graphics in item '" + Item.GetName() + "'."};
    }
    
    auto ImageOrError = Rasteriser::Rasterise(SceneOrError.value(), Rasteriser::RenderOptions{Width, Height, SubPixels});
    
    if(SceneOrError.has_value() == false)
    {
        throw std::runtime_error{"Could not rasterise scene for vector graphics in item '" + Item.GetName() + "'."};
    }
    
    auto Texture = g_GraphicsEngine->GetTextureManager()->Create(TextureIdentifier);
    
    if(Texture == nullptr)
    {
        throw std::runtime_error("Could not create texture '" + TextureIdentifier + "'.");
    }
    Texture->SetData(ImageOrError->Width, ImageOrError->Height, std::to_underlying(ImageOrError->Format), ImageOrError->Data.data());
}
