/**
 * galactic-fall
 * Copyright (C) 2015-2018  Hagen Möbius
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

#include <Structure.h>

namespace Arxx
{
	class Archive;
	class Reference;
}

namespace Graphics
{
	class ShadingManager;
}

template< class Class >
class ClassManager;

class BlueprintManager;
class Galaxy;
class ScenarioManager;
class Settings;
class SlotClass;

class ResourceReader
{
public:
	ResourceReader(void);
	~ResourceReader(void);
	bool LoadArchive(const std::string & Path);
	void ReadAmmunitionClasses(BlueprintManager * BlueprintManager);
	void ReadBatteryClasses(BlueprintManager * BlueprintManager);
	void ReadCommodityClasses(BlueprintManager * BlueprintManager);
	void ReadEnergyProjectileWeaponClasses(BlueprintManager * BlueprintManager);
	Galaxy * ReadGalaxy(const std::string & GalaxyIdentifier);
	void ReadGeneratorClasses(BlueprintManager * BlueprintManager);
	void ReadMeshes(void);
	void ReadModels(void);
	void ReadScenarios(ScenarioManager * ScenarioManager);
	void ReadSettings(Settings * Settings);
	void ReadShadersAndPrograms(Graphics::ShadingManager * ShadingManager);
	void ReadShipClasses(BlueprintManager * BlueprintManager, ClassManager< SlotClass > * SlotClassManager);
	void ReadSlotClasses(ClassManager< SlotClass > * SlotClassManager);
	void ReadTurretClasses(BlueprintManager * BlueprintManager);
	void ReadTextures(void);
	std::string ReadSavegameFromScenarioPath(const std::string & ScenarioPath);
private:
	void _ReadItems(const std::string & Path, std::function< void (Arxx::Reference &) > ReaderFunction);
	void _ReadItems(Arxx::Structure::Relation & Relation, std::function< void (Arxx::Reference &) > ReaderFunction);
	void _ReadSystem(Arxx::Reference & Reference, Galaxy * Galaxy, std::multimap< std::string, std::string > & SystemLinks);
	Arxx::Archive * _Archive;
};

#endif
