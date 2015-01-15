/**
 * galactic-fall
 * Copyright (C) 2015  Hagen Möbius
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

namespace Arxx
{
	class Archive;
	class Reference;
}

namespace Graphics
{
	class ShadingManager;
}

class ScenarioManager;
class Settings;

class ResourceReader
{
public:
	ResourceReader(const std::string & DataDirectoryPath);
	~ResourceReader(void);
	bool LoadArchive(const std::string & Path);
	void ReadAssetClasses(void);
	void ReadBatteryClasses(void);
	void ReadCommodityClasses(void);
	void ReadFactions(void);
	void ReadGeneratorClasses(void);
	void ReadMeshes(void);
	void ReadModels(void);
	void ReadScenarios(ScenarioManager * ScenarioManager);
	void ReadSettings(Settings * Settings);
	void ReadShadersAndPrograms(Graphics::ShadingManager * ShadingManager);
	void ReadShipClasses(void);
	void ReadSlotClasses(void);
	void ReadSystems(void);
	void ReadSystemLinks(void);
	void ReadTextures(void);
	void ReadWeaponClasses(void);
	std::string ReadSavegameFromScenarioPath(const std::string & ScenarioPath);
private:
	void _ReadItems(const std::string & Path, std::function< void (Arxx::Reference &) > ReaderFunction);
	Arxx::Archive * _Archive;
};

#endif
