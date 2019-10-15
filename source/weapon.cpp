/**
 * galactic-fall
 * Copyright (C) 2019  Hagen Möbius
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

#include <algebra/vector3f.h>

#include "visualization_prototype.h"
#include "weapon.h"

Weapon::Weapon(void) :
	_Fire{false},
	_MuzzlePosition{Vector3f::CreateZero()},
	_ShotDamage{0.0f},
	_ShotExitSpeed{0.0f},
	_ShotLifeTime{0.0f},
	_ShotVisualizationPrototype{nullptr}
{
	// initialize object aspects
	AddAspectAccessory();
	AddAspectName();
	AddAspectPhysical();
	AddAspectPosition();
	AddAspectUpdate();
	AddAspectVisualization();
}

Weapon::~Weapon(void)
{
	delete _ShotVisualizationPrototype;
	_ShotVisualizationPrototype = nullptr;
}

void Weapon::SetShotVisualizationPrototype(const VisualizationPrototype & ShotVisualizationPrototype)
{
	delete _ShotVisualizationPrototype;
	_ShotVisualizationPrototype = new VisualizationPrototype(ShotVisualizationPrototype);
}

float Weapon::GetMaximumEnergyInput(float Seconds) const
{
	auto Result{0.0f};
	
	if(_Fire == true)
	{
		Result = Physics::Energy::Device::GetMaximumEnergyInput(Seconds);
	}
	
	return Result;
}

float Weapon::GetMaximumEnergyOutput(float Seconds) const
{
	auto Result{0.0f};
	
	if(_Fire == false)
	{
		Result = Physics::Energy::Device::GetMaximumEnergyOutput(Seconds);
	}
	
	return Result;
}
