/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#ifndef ARX_READING_H
#define ARX_READING_H

class Archive;
class AssetClassManager;
class CommodityClassManager;
class Galaxy;
class Item;
class ModelManager;
class ShipClassManager;
class UserInterface;
class WeaponClassManager;

Arxx::Item * GetItem(Arxx::Archive & Archive, Arxx::u4byte UniqueIdentifier);

void ReadAssetClasses(Arxx::Archive & Archive, AssetClassManager * Manager);
void ReadCommodityClasses(Arxx::Archive & Archive, CommodityClassManager * Manger);
void ReadModels(Arxx::Archive & Archive, ModelManager * Manager);
void ReadShipClasses(Arxx::Archive & Archive, ShipClassManager * Manager);
void ReadSystems(Arxx::Archive & Archive, Galaxy * Galaxy);
void ReadSystemLinks(Arxx::Archive & Archive, Galaxy * Galaxy);
void ReadUserInterface(Arxx::Archive & Archive, UserInterface * UserInterface);
void ReadWeaponClasses(Arxx::Archive & Archive, WeaponClassManager * Manager);

#endif
