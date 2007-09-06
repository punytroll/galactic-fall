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
#include "graphics_material.h"

Graphics::Material::Material(void) :
	m_DiffuseColor(0),
	m_Shininess(0.0f),
	m_SpecularColor(0)
{
}

Graphics::Material::Material(const Graphics::Material & Material) :
	m_DiffuseColor(0),
	m_Shininess(Material.GetShininess()),
	m_SpecularColor(0)
{
	if(Material.GetDiffuseColor() != 0)
	{
		SetDiffuseColor(*(Material.GetDiffuseColor()));
	}
	if(Material.GetSpecularColor() != 0)
	{
		SetSpecularColor(*(Material.GetSpecularColor()));
	}
}

Graphics::Material::~Material(void)
{
	delete m_DiffuseColor;
	m_DiffuseColor = 0;
	delete m_SpecularColor;
	m_SpecularColor = 0;
}

void Graphics::Material::SetDiffuseColor(const Color & DiffuseColor)
{
	delete m_DiffuseColor;
	m_DiffuseColor = new Color(DiffuseColor);
}

void Graphics::Material::SetSpecularColor(const Color & SpecularColor)
{
	delete m_SpecularColor;
	m_SpecularColor = new Color(SpecularColor);
}