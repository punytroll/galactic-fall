/**
 * galactic-fall
 * Copyright (C) 2015-2019  Hagen Möbius
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

#include <cassert>

#include <graphics/color_rgbo.h>
#include <graphics/style.h>

Graphics::Style::Style(void) :
	_DiffuseColor(nullptr),
	_Shininess(nullptr),
	_SpecularColor(nullptr)
{
}

Graphics::Style::Style(const Graphics::Style * Style) :
	_DiffuseColor((Style->_DiffuseColor != nullptr) ? (new Graphics::ColorRGBO(*(Style->_DiffuseColor))) : (nullptr)),
	_ProgramIdentifier(Style->_ProgramIdentifier),
	_Shininess((Style->_Shininess != nullptr) ? (new float(*Style->_Shininess)) : (nullptr)),
	_SpecularColor((Style->_SpecularColor != nullptr) ? (new Graphics::ColorRGBO(*(Style->_SpecularColor))) : (nullptr))
{
}

Graphics::Style::~Style(void)
{
	delete _DiffuseColor;
	_DiffuseColor = nullptr;
	delete _Shininess;
	_Shininess = nullptr;
	delete _SpecularColor;
	_SpecularColor = nullptr;
}

const Graphics::ColorRGBO & Graphics::Style::GetDiffuseColor(void) const
{
	assert(_DiffuseColor != nullptr);
	
	return *_DiffuseColor;
}

float Graphics::Style::GetShininess(void) const
{
	assert(_Shininess != nullptr);
	
	return *_Shininess;
}

const Graphics::ColorRGBO & Graphics::Style::GetSpecularColor(void) const
{
	assert(_SpecularColor != nullptr);
	
	return *_SpecularColor;
}

void Graphics::Style::SetDiffuseColor(const Graphics::ColorRGBO & DiffuseColor)
{
	delete _DiffuseColor;
	_DiffuseColor = new Graphics::ColorRGBO(DiffuseColor);
}

void Graphics::Style::SetShininess(float Shininess)
{
	delete _Shininess;
	_Shininess = new float(Shininess);
}

void Graphics::Style::SetSpecularColor(const Graphics::ColorRGBO & SpecularColor)
{
	delete _SpecularColor;
	_SpecularColor = new Graphics::ColorRGBO(SpecularColor);
}
