/**
 * galactic-fall
 * Copyright (C) 2013-2019  Hagen Möbius
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

#ifndef GRAPHICS_ORTHOGONAL_2D_PROJECTION_H
#define GRAPHICS_ORTHOGONAL_2D_PROJECTION_H

#include <algebra/matrix4f.h>

#include <graphics/projection.h>

namespace Graphics
{
	class Orthogonal2DProjection : public Projection
	{
	public:
		Orthogonal2DProjection(void);
		virtual ~Orthogonal2DProjection(void);
		// getters
		float GetBottom(void) const;
		float GetLeft(void) const;
		float GetRight(void) const;
		float GetTop(void) const;
		// setters
		void SetBottom(float Bottom);
		void SetLeft(float Left);
		void SetRight(float Right);
		void SetTop(float Top);
	protected:
		virtual Matrix4f CalculateMatrix(void) const;
	private:
		float _Bottom;
		float _Left;
		float _Right;
		float _Top;
	};
	
	inline float Orthogonal2DProjection::GetBottom(void) const
	{
		return _Bottom;
	}
	
	inline float Orthogonal2DProjection::GetLeft(void) const
	{
		return _Left;
	}
	
	inline float Orthogonal2DProjection::GetRight(void) const
	{
		return _Right;
	}
	
	inline float Orthogonal2DProjection::GetTop(void) const
	{
		return _Top;
	}
}

#endif
