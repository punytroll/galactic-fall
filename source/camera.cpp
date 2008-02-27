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

#include "camera.h"
#include "object.h"
#include "object_aspect_position.h"

Camera::Camera(void) :
	m_Focus(0)
{
}

void Camera::Draw(void) const
{
	glTranslatef(-m_Position.m_V.m_A[0], -m_Position.m_V.m_A[1], -m_Position.m_V.m_A[2]);
	if(m_Focus.IsValid() == true)
	{
		glTranslatef(-m_Focus->GetAspectPosition()->GetPosition().m_V.m_A[0], -m_Focus->GetAspectPosition()->GetPosition().m_V.m_A[1], 0.0f);
	}
}

void Camera::SetFocus(Reference< Object > Focus)
{
	m_Focus = Focus;
}

void Camera::SetPosition(float X, float Y)
{
	m_Position.m_V.m_A[0] = X;
	m_Position.m_V.m_A[1] = Y;
}

void Camera::SetPosition(float X, float Y, float Z)
{
	m_Position.m_V.m_A[0] = X;
	m_Position.m_V.m_A[1] = Y;
	m_Position.m_V.m_A[2] = Z;
}

void Camera::MoveIn(void)
{
	m_Position.m_V.m_A[2] *= 0.95f;
}

void Camera::MoveOut(void)
{
	m_Position.m_V.m_A[2] *= 1.05f;
}
