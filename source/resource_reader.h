/**
 * galactic-fall
 * Copyright (C) 2015-2025  Hagen Möbius
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
**/

#ifndef RESOURCE_READER_H
#define RESOURCE_READER_H

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
	auto LoadArchive(std::string const & Path) -> bool;
	auto ReadAmmunitionClasses(BlueprintManager * BlueprintManager) -> void;
	auto ReadBatteryClasses(BlueprintManager * BlueprintManager) -> void;
	auto ReadCommodityClasses(BlueprintManager * BlueprintManager) -> void;
	auto ReadEnergyProjectileWeaponClasses(BlueprintManager * BlueprintManager) -> void;
	auto ReadGalaxy(std::string const & GalaxyIdentifier) -> Galaxy *;
	auto ReadGeneratorClasses(BlueprintManager * BlueprintManager) -> void;
	auto ReadMeshes() -> void;
	auto ReadModels() -> void;
	auto ReadScenarios(ScenarioManager * ScenarioManager) -> void;
	auto ReadSettings(Settings * Settings) -> void;
	auto ReadShadersAndPrograms(Graphics::ShadingManager * ShadingManager) -> void;
	auto ReadShipClasses(BlueprintManager * BlueprintManager, ClassManager<SlotClass> * SlotClassManager) -> void;
	auto ReadSlotClasses(ClassManager<SlotClass> * SlotClassManager) -> void;
	auto ReadTurretClasses(BlueprintManager * BlueprintManager) -> void;
	auto ReadTextures() -> void;
	auto ReadSavegameFromScenarioPath(std::string const & ScenarioPath) -> std::string;
    
private:
	auto m_ReadItems(std::string const & Path, std::function<void(ARX::Reference &)> ReaderFunction) -> void;
	auto m_ReadItems(ARX::Structure::Relation & Relation, std::function<void(ARX::Reference &)> ReaderFunction) -> void;
	auto m_ReadSystem(ARX::Reference & Reference, Galaxy * Galaxy, std::multimap<std::string, std::string> & SystemLinks) -> void;
    
	std::unique_ptr<ARX::Archive> m_Archive;
};

#endif
