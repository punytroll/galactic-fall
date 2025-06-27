/**
 * galactic-fall
 * Copyright (C) 2013-2025  Hagen Möbius
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

#ifndef GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__PERSPECTIVE_PROJECTION_H
#define GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__PERSPECTIVE_PROJECTION_H

#include <algebra/matrix4f.h>

#include <graphics/projection.h>

namespace Graphics
{
	class PerspectiveProjection : public Projection
	{
	public:
		PerspectiveProjection(void);
		virtual ~PerspectiveProjection(void);
		// getters
		float GetAspect(void) const;
		float GetFarClippingPlane(void) const;
		float GetFieldOfViewY(void) const;
		float GetNearClippingPlane(void) const;
		// setters
		void SetAspect(float Aspect);
		void SetFarClippingPlane(float FarClippingPlane);
		void SetFieldOfViewY(float FieldOfViewY);
		void SetNearClippingPlane(float NearClippingPlane);
	protected:
		virtual Matrix4f CalculateMatrix(void) const;
	private:
		float _Aspect;
		float _FarClippingPlane;
		float _FieldOfViewY;
		float _NearClippingPlane;
	};
	
	inline float PerspectiveProjection::GetAspect(void) const
	{
		return _Aspect;
	}
	
	inline float PerspectiveProjection::GetFarClippingPlane(void) const
	{
		return _FarClippingPlane;
	}
	
	inline float PerspectiveProjection::GetFieldOfViewY(void) const
	{
		return _FieldOfViewY;
	}
	
	inline float PerspectiveProjection::GetNearClippingPlane(void) const
	{
		return _NearClippingPlane;
	}
}

#endif
 
