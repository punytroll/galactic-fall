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

#include "../color.h"
#include "material.h"

Graphics::Material::Material(void) :
	_DiffuseColor(nullptr),
	_Shininess(0.0f),
	_SpecularColor(nullptr)
{
}

Graphics::Material::Material(const Graphics::Material * Material) :
	_DiffuseColor((Material->_DiffuseColor != nullptr) ? (new Color(Material->_DiffuseColor)) : (nullptr)),
	_ProgramIdentifier(Material->_ProgramIdentifier),
	_Shininess(Material->_Shininess),
	_SpecularColor((Material->_SpecularColor != nullptr) ? (new Color(Material->_SpecularColor)) : (nullptr))
{
}

Graphics::Material::~Material(void)
{
	delete _DiffuseColor;
	_DiffuseColor = nullptr;
	delete _SpecularColor;
	_SpecularColor = nullptr;
}

void Graphics::Material::SetDiffuseColor(const Color & DiffuseColor)
{
	delete _DiffuseColor;
	_DiffuseColor = new Color(DiffuseColor);
}

void Graphics::Material::SetSpecularColor(const Color & SpecularColor)
{
	delete _SpecularColor;
	_SpecularColor = new Color(SpecularColor);
}
