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

#ifndef MINI_MAP_H
#define MINI_MAP_H

#include "camera.h"
#include "perspective.h"
#include "widget.h"

class Ship;

class MiniMap : public Widget
{
public:
	MiniMap(void);
	void SetFocus(Ship * Ship);
	virtual void Draw(void) const;
private:
	Ship * m_Focus;
	Camera m_Camera;
	Perspective m_Perspective;
};

#endif
