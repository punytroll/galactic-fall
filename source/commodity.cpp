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

#include "callbacks.h"
#include "color.h"
#include "commodity.h"
#include "commodity_class.h"
#include "graphics_model.h"
#include "object_aspect_name.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"

Commodity::Commodity(const CommodityClass * CommodityClass) :
	m_CommodityClass(CommodityClass),
	m_Hull(20.0f),
	m_Velocity(true)
{
	// initialize required aspects
	AddAspectName();
	GetAspectName()->SetName(m_CommodityClass->GetName());
	AddAspectPhysical();
	GetAspectPhysical()->SetRadialSize(m_CommodityClass->GetModel()->GetRadialSize());
	GetAspectPhysical()->SetSpaceRequirement(m_CommodityClass->GetSpaceRequirement());
	AddAspectPosition();
	AddAspectUpdate();
	GetAspectUpdate()->SetCallback(Method(this, &Commodity::Update));
	AddAspectVisualization();
	
	// other
	Vector3f RotationAxis(static_cast< float >(random()) / RAND_MAX, static_cast< float >(random()) / RAND_MAX, static_cast< float >(random()) / RAND_MAX);
	
	RotationAxis.Normalize();
	m_AngularVelocity.Set(RotationAxis[0], RotationAxis[1], RotationAxis[2], static_cast< float >(random()) / RAND_MAX);
}

Commodity::~Commodity(void)
{
}

bool Commodity::Update(float Seconds)
{
	GetAspectPosition()->ModifyOrientation(Quaternion(Vector4f(m_AngularVelocity[0], m_AngularVelocity[1], m_AngularVelocity[2], 0.0f).m_V, m_AngularVelocity[3] * Seconds));
	GetAspectPosition()->ModifyPosition(m_Velocity * Seconds);
	
	return true;
}
