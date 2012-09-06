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

#include <assert.h>

#include <iostream>

#include "texture.h"

Graphics::Texture::Texture(const std::string & Identifier) :
	_Identifier(Identifier),
	_Texture(0)
{
}

Graphics::Texture::~Texture(void)
{
	if(_Texture != 0)
	{
		glDeleteTextures(1, &_Texture);
		_Texture = 0;
	}
}

void Graphics::Texture::SetData(unsigned_numeric Width, unsigned_numeric Height, unsigned_numeric Format, const unsigned char * Data)
{
	assert(_Texture == 0);
	glGenTextures(1, &_Texture);
	glBindTexture(GL_TEXTURE_2D, _Texture);
	if(Format == 1) /// RGBA one byte each
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 4, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if(Format == 2) /// RGB one byte each
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 4, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		throw;
	}
}

void Graphics::Texture::Activate(void) const
{
	assert(_Texture != 0);
	glBindTexture(GL_TEXTURE_2D, _Texture);
}
