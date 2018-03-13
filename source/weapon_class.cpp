/**
 * galactic-fall
 * Copyright (C) 2007-2018  Hagen Möbius
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

#include <cassert>

#include "visualization_prototype.h"
#include "weapon_class.h"

WeaponClass::WeaponClass(const std::string & Identifier) :
	Template("weapon", Identifier),
	_ShotVisualizationPrototype(nullptr),
	_WeaponVisualizationPrototype(nullptr)
{
}

WeaponClass::~WeaponClass(void)
{
	delete _ShotVisualizationPrototype;
	_ShotVisualizationPrototype = nullptr;
	delete _WeaponVisualizationPrototype;
	_WeaponVisualizationPrototype = nullptr;
}

void WeaponClass::SetShotVisualizationPrototype(const VisualizationPrototype & ShotVisualizationPrototype)
{
	delete _ShotVisualizationPrototype;
	_ShotVisualizationPrototype = new VisualizationPrototype(ShotVisualizationPrototype);
}

void WeaponClass::SetWeaponVisualizationPrototype(const VisualizationPrototype & WeaponVisualizationPrototype)
{
	delete _WeaponVisualizationPrototype;
	_WeaponVisualizationPrototype = new VisualizationPrototype(WeaponVisualizationPrototype);
}
