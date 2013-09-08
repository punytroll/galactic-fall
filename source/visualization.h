/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "referencing.h"

namespace Graphics
{
	class Node;
}

class Visualization
{
public:
	// destructor
	~Visualization(void);
	// getters
	Reference< Graphics::Node > & GetGraphics(void);
	// setters
	void SetGraphics(Reference< Graphics::Node > & Graphics);
private:
	Reference< Graphics::Node > _Graphics;
};

inline Reference< Graphics::Node > & Visualization::GetGraphics(void)
{
	return _Graphics;
}

inline void Visualization::SetGraphics(Reference< Graphics::Node > & Graphics)
{
	_Graphics = Graphics;
}

#endif
