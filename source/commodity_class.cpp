/**
 * galactic-fall
 * Copyright (C) 2007  Hagen Möbius
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

#include "color.h"
#include "commodity_class.h"

CommodityClass::CommodityClass(const std::string & Identifier) :
	m_Color(0),
	m_Identifier(Identifier),
	m_Model(0),
	m_SpaceRequirement(0)
{
}

CommodityClass::~CommodityClass(void)
{
	delete m_Color;
	m_Color = 0;
}

void CommodityClass::SetColor(const Color & NewColor)
{
	m_Color = new Color(NewColor);
}
