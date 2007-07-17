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

#include <GL/gl.h>

#include "color.h"
#include "commodity.h"
#include "commodity_class.h"
#include "model.h"

Commodity::Commodity(const CommodityClass * CommodityClass) :
	m_CommodityClass(CommodityClass),
	m_Hull(20.0f),
	m_Velocity(true)
{
	SetRadialSize(m_CommodityClass->GetModel()->GetRadialSize());
	SetName(m_CommodityClass->GetName());
	m_RotationAxis.Set(static_cast< float >(random()) / RAND_MAX, static_cast< float >(random()) / RAND_MAX, static_cast< float >(random()) / RAND_MAX);
	m_RotationAxis.Normalize();
	m_AngularPosition = static_cast< float >(random()) / RAND_MAX;
	m_AngularVelocity = static_cast< float >(random()) / RAND_MAX;
}

Commodity::~Commodity(void)
{
}

void Commodity::Draw(void) const
{
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glRotatef(m_AngularPosition * 180.0f / M_PI, m_RotationAxis.m_V.m_A[0], m_RotationAxis.m_V.m_A[1], m_RotationAxis.m_V.m_A[2]);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_CommodityClass->GetColor()->GetColor().m_V.m_A);
	m_CommodityClass->GetModel()->Draw();
	glPopMatrix();
}

void Commodity::Move(float Seconds)
{
	m_Position += m_Velocity * Seconds;
	m_AngularPosition += m_AngularVelocity * Seconds;
}
