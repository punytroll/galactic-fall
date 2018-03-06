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

#include <cassert>

#include "ammunition_class.h"
#include "visualization_prototype.h"

AmmunitionClass::AmmunitionClass(const std::string & Identifier) :
	_CartridgeSize(0),
	_Identifier(Identifier),
	_SpaceRequirement(0),
	_VisualizationPrototype(nullptr)
{
}

AmmunitionClass::~AmmunitionClass(void)
{
	delete _VisualizationPrototype;
	_VisualizationPrototype = nullptr;
}

void AmmunitionClass::SetVisualizationPrototype(const VisualizationPrototype & NewVisualizationPrototype)
{
	delete _VisualizationPrototype;
	_VisualizationPrototype = new VisualizationPrototype(NewVisualizationPrototype);
}
