/**
 * galactic-fall
 * Copyright (C) 2018  Hagen Möbius
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

#ifndef GAME_SAVER_H
#define GAME_SAVER_H

#include <unordered_map>

#include <algebra/vector3f.h>

#include <xml_stream/xml_stream.h>

#include <ostream>

class Ammunition;
class Battery;
class Character;
class CommandMind;
class Commodity;
class Galaxy;
class Generator;
class Hangar;
class Mind;
class Object;
class Ship;
class Storage;
class System;
class Turret;
class EnergyProjectileWeapon;

class GameSaver
{
public:
	GameSaver(std::ostream & OStream);
	void Save(Galaxy * Galaxy, System * CurrentSystem, float TimeWarp, const Vector3f & CameraOffset, float FieldOfViewY, CommandMind * CommandMind, Character * ObservedCharacter);
private:
	std::string _GetOrRegisterObjectIdentifier(Object * Object);
	void _WriteAmmunition(Ammunition * Ammunition);
	void _WriteBattery(Battery * Battery);
	void _WriteCharacter(Character * Character);
	void _WriteCommodity(Commodity * Commodity);
	void _WriteGenerator(Generator * Generator);
	void _WriteHangar(Hangar * Hangar);
	void _WriteMind(Mind * Mind);
	void _WriteObject(Object * Object);
	void _WriteObjectRecursive(Object * Object);
	void _WriteShip(Ship * Ship);
	void _WriteStorage(Storage * Storage);
	void _WriteTurret(Turret * Turret);
	void _WriteEnergyProjectileWeapon(EnergyProjectileWeapon * EnergyProjectileWeapon);
	std::unordered_map< Object *, std::string > _IdentifiersByObject;
	std::ostream & _OStream;
	XMLStream _XMLStream;
};

#endif
