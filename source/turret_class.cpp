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

#include <cassert>

#include "visualization_prototype.h"
#include "turret_class.h"

TurretClass::TurretClass(const std::string & Identifier) :
	Template("turret", Identifier),
	_MuzzlePosition(Vector3f::CreateZero()),
	_ShotVisualizationPrototype(nullptr),
	_TurretVisualizationPrototype(nullptr)
{
}

TurretClass::~TurretClass(void)
{
	delete _ShotVisualizationPrototype;
	_ShotVisualizationPrototype = nullptr;
	delete _TurretVisualizationPrototype;
	_TurretVisualizationPrototype = nullptr;
}

void TurretClass::SetShotVisualizationPrototype(const VisualizationPrototype & ShotVisualizationPrototype)
{
	delete _ShotVisualizationPrototype;
	_ShotVisualizationPrototype = new VisualizationPrototype(ShotVisualizationPrototype);
}

void TurretClass::SetTurretVisualizationPrototype(const VisualizationPrototype & TurretVisualizationPrototype)
{
	delete _TurretVisualizationPrototype;
	_TurretVisualizationPrototype = new VisualizationPrototype(TurretVisualizationPrototype);
}
