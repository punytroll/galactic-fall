/**
 * galactic-fall
 * Copyright (C) 2015-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef RESOURCE_READER_H
#define RESOURCE_READER_H

#include <filesystem>
#include <memory>

#include <arx/structure.h>

namespace ARX
{
    class Archive;
    class Reference;
}

namespace Graphics
{
    class ShadingManager;
}

template<typename ClassType>
class ClassManager;

class BlueprintManager;
class Galaxy;
class ScenarioManager;
class Settings;
class SlotClass;

class ResourceReader
{
public:
    auto LoadArchive(std::filesystem::path const & Path) -> bool;
    auto ReadAmmunitionClasses(BlueprintManager * BlueprintManager) -> void;
    auto ReadBatteryClasses(BlueprintManager * BlueprintManager) -> void;
    auto ReadCommodityClasses(BlueprintManager * BlueprintManager) -> void;
    auto ReadGalaxy(std::string const & GalaxyIdentifier) -> Galaxy *;
    auto ReadGeneratorClasses(BlueprintManager * BlueprintManager) -> void;
    auto ReadMeshes() -> void;
    auto ReadModels() -> void;
    auto ReadProjectileWeaponClasses(BlueprintManager * BlueprintManager) -> void;
    auto ReadScenarios(ScenarioManager * ScenarioManager) -> void;
    auto ReadSettings(Settings * Settings) -> void;
    auto ReadShadersAndPrograms(Graphics::ShadingManager * ShadingManager) -> void;
    auto ReadShipClasses(BlueprintManager * BlueprintManager, ClassManager<SlotClass> * SlotClassManager) -> void;
    auto ReadSlotClasses(ClassManager<SlotClass> * SlotClassManager) -> void;
    auto ReadTurretClasses(BlueprintManager * BlueprintManager) -> void;
    auto ReadTextures() -> void;
    auto ReadSavegameFromScenarioPath(std::string const & ScenarioPath) -> std::string;
    
private:
    auto m_ReadSystem(ARX::Item & Item, Galaxy * Galaxy, std::multimap<std::string, std::string> & SystemLinks) -> void;
    
    std::unique_ptr<ARX::Archive> m_Archive;
};

#endif
