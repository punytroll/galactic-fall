/**
 * galactic-fall
 * Copyright (C) 2006  Hagen MÃ¶bius
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

#ifndef PERSPECTIVE_H
#define PERSPECTIVE_H

#include <math3d/matrix4f.h>

class Perspective
{
public:
	Perspective(void);
	void Draw(void) const;
	// setters
	void SetAspect(float Aspect);
	void SetFieldOfView(float FieldOfView);
	void SetNearClippingPlane(float NearClippingPlane);
	void SetFarClippingPlane(float FarClippingPlane);
private:
	mutable bool m_Initialized;
	float m_Aspect;
	float m_FieldOfView;
	float m_NearClippingPlane;
	float m_FarClippingPlane;
	mutable math3d::matrix4f m_Matrix;
};

inline void Perspective::SetAspect(float Aspect)
{
	m_Aspect = Aspect;
	m_Initialized = false;
}

inline void Perspective::SetFieldOfView(float FieldOfView)
{
	m_FieldOfView = FieldOfView;
	m_Initialized = false;
}

inline void Perspective::SetNearClippingPlane(float NearClippingPlane)
{
	m_NearClippingPlane = NearClippingPlane;
	m_Initialized = false;
}

inline void Perspective::SetFarClippingPlane(float FarClippingPlane)
{
	m_FarClippingPlane = FarClippingPlane;
	m_Initialized = false;
}


#endif
