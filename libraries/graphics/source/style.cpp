/**
 * galactic-fall
 * Copyright (C) 2015-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include <graphics/style.h>

Graphics::Style::Style(Graphics::Style const * Style) :
	m_DiffuseColor{Style->m_DiffuseColor},
	m_ProgramIdentifier{Style->m_ProgramIdentifier},
	m_Shininess{Style->m_Shininess},
	m_SpecularColor{Style->m_SpecularColor}
{
}

auto Graphics::Style::GetDiffuseColor() const -> Graphics::ColorRGBO const &
{
	assert(m_DiffuseColor.has_value() == true);
	
	return *m_DiffuseColor;
}

auto Graphics::Style::GetShininess() const -> float
{
	assert(m_Shininess.has_value() == true);
	
	return *m_Shininess;
}

auto Graphics::Style::GetSpecularColor() const -> Graphics::ColorRGBO const &
{
	assert(m_SpecularColor.has_value() == true);
	
	return *m_SpecularColor;
}
