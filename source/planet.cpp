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
#include "globals.h"
#include "model.h"
#include "model_manager.h"
#include "planet.h"

PlanetCommodity::PlanetCommodity(const Commodity * Commodity) :
	m_Commodity(Commodity),
	m_BasePriceModifier(1.0f)
{
}

PlanetCommodity::~PlanetCommodity(void)
{
}

u4byte PlanetCommodity::GetPrice(void) const
{
	return static_cast< u4byte >(m_Commodity->GetBasePrice() * m_BasePriceModifier);
}

Planet::Planet(const std::string & Identifier) :
	m_Identifier(Identifier),
	m_Color(0)
{
}

Planet::~Planet(void)
{
	while(m_Commodities.size() > 0)
	{
		delete m_Commodities.back();
		m_Commodities.pop_back();
	}
	delete m_Color;
	m_Color = 0;
}

void Planet::SetDescription(const std::string & Description)
{
	m_Description = Description;
}

void Planet::SetSize(const float & Size)
{
	m_Size = Size;
	SetRadialSize(m_Size / 2.0f);
}

void Planet::SetColor(const Color & NewColor)
{
	delete m_Color;
	m_Color = new Color(NewColor);
}

void Planet::Draw(void) const
{
	glPushAttrib(GL_ENABLE_BIT);
	glPushMatrix();
	glTranslatef(m_Position.m_V.m_A[0], m_Position.m_V.m_A[1], 0.0f);
	glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
	if(m_Color != 0)
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Color->GetColor().m_V.m_A);
		glMaterialfv(GL_FRONT, GL_SPECULAR, (Vector4f(1.0f, 1.0f, 1.0f, 1.0f) - m_Color->GetColor()).m_V.m_A);
	}
	else
	{
		glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
		glMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
	}
	glEnable(GL_NORMALIZE);
	glScalef(GetRadialSize(), GetRadialSize(), GetRadialSize());
	g_ModelManager.Get("planet")->Draw();
	/* TODO: This code allows an athmosphere around the planet ... optimize and make usable via a planet property
	glEnable(GL_BLEND);
	glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 0.35f).m_V.m_A);
	glMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
	glScalef(1.05f, 1.05f, 1.05f);
	g_ModelManager.Get("planet")->Draw();
	*/
	glPopMatrix();
	glPopAttrib();
}

PlanetCommodity * Planet::CreateCommodity(const Commodity * Commodity)
{
	/// @todo check whether the commodity already exists
	m_Commodities.push_back(new PlanetCommodity(Commodity));
	
	return m_Commodities.back();
}
