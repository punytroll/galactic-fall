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

#ifndef GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__PROJECTION_H
#define GALACTIC_FALL__LIBRARY__GRAPHICS__INCLUDE__GRAPHICS__PROJECTION_H

#include <algebra/matrix4f.h>

namespace Graphics
{
	class Projection
	{
	public:
		Projection(void);
		virtual ~Projection(void);
		// getters
		const Matrix4f & GetMatrix(void) const;
	protected:
		void InvalidateMatrix();
		virtual Matrix4f CalculateMatrix(void) const = 0;
	private:
		mutable Matrix4f _Matrix;
		mutable bool _MatrixValid;
	};
	
	inline void Projection::InvalidateMatrix(void)
	{
		_MatrixValid = false;
	}
}

#endif
