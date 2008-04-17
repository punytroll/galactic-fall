/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#include <assert.h>
#include <float.h>

#include "color.h"
#include "visualization_prototype.h"
#include "weapon_class.h"

WeaponClass::WeaponClass(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_ReloadTime(FLT_MAX),
	m_ParticleExitSpeed(0.0f),
	m_ParticleDamage(0.0f),
	m_ParticleLifeTime(0.0f),
	m_ParticleVisualizationPrototype(0),
	m_VisualizationPrototype(0)
{
}

WeaponClass::~WeaponClass(void)
{
	delete m_ParticleVisualizationPrototype;
	m_ParticleVisualizationPrototype = 0;
	delete m_VisualizationPrototype;
	m_VisualizationPrototype = 0;
}

void WeaponClass::AddParticleVisualizationPrototype(void)
{
	assert(m_ParticleVisualizationPrototype == 0);
	m_ParticleVisualizationPrototype = new VisualizationPrototype();
}

void WeaponClass::AddVisualizationPrototype(void)
{
	assert(m_VisualizationPrototype == 0);
	m_VisualizationPrototype = new VisualizationPrototype();
}
