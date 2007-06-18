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

#ifndef CAMERA_H
#define CAMERA_H

#include <math3d/vector3f.h>

#include "referencing.h"

class Position;

class Camera
{
public:
	Camera(void);
	void Draw(void) const;
	void SetFocus(Reference< Position > Focus);
	void SetPosition(float X, float Y);
	void SetPosition(float X, float Y, float Z);
	void SetFieldOfView(float FieldOfView);
	Reference< Position > & GetFocus(void);
	const Reference< Position > & GetFocus(void) const;
	const math3d::vector3f & GetPosition(void) const;
	float GetFieldOfView(void) const;
	void MoveIn(void);
	void MoveOut(void);
private:
	Reference< Position > m_Focus;
	math3d::vector3f m_Position;
	float m_FieldOfView;
};

inline void Camera::SetFieldOfView(float FieldOfView)
{
	m_FieldOfView = FieldOfView;
}

inline Reference< Position > & Camera::GetFocus(void)
{
	return m_Focus;
}

inline const Reference< Position > & Camera::GetFocus(void) const
{
	return m_Focus;
}

inline const math3d::vector3f & Camera::GetPosition(void) const
{
	return m_Position;
}

inline float Camera::GetFieldOfView(void) const
{
	return m_FieldOfView;
}

#endif
