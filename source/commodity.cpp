/**
 * galactic-fall
 * Copyright (C) 2006-2019  Hagen Möbius
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

#include <algebra/vector4f.h>

#include <graphics/node.h>

#include "commodity.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_aspect_visualization.h"
#include "visualization.h"

Commodity::Commodity(void) :
	m_AngularVelocity(true),
	m_Hull(20.0f),
	m_Velocity(Vector3f::CreateZero())
{
	// initialize required aspects
	AddAspectName();
	AddAspectPhysical();
	AddAspectPosition();
	AddAspectUpdate();
	GetAspectUpdate()->SetCallback(std::bind(&Commodity::Update, this, std::placeholders::_1));
	AddAspectVisualization();
	GetAspectVisualization()->SetUpdateVisualizationCallback(std::bind(&Commodity::_UpdateVisualization, this, std::placeholders::_1));
}

Commodity::~Commodity(void)
{
}

bool Commodity::Update(float Seconds)
{
	GetAspectPosition()->ModifyOrientation(Quaternion::CreateFromAxisComponentsAndAngle(m_AngularVelocity[0], m_AngularVelocity[1], m_AngularVelocity[2], m_AngularVelocity[3] * Seconds));
	GetAspectPosition()->ModifyPosition(m_Velocity * Seconds);
	
	return true;
}

void Commodity::_UpdateVisualization(Visualization * Visualization)
{
	assert(Visualization != nullptr);
	assert(Visualization->GetGraphics() != nullptr);
	assert(GetAspectPosition() != nullptr);
	if(Visualization->GetUpdateOrientation() == true)
	{
		Visualization->GetGraphics()->SetOrientation(GetAspectPosition()->GetOrientation());
	}
	if(Visualization->GetUpdatePosition() == true)
	{
		Visualization->GetGraphics()->SetPosition(GetAspectPosition()->GetPosition());
	}
}
