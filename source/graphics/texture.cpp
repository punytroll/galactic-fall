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

#include "gl.h"
#include "texture.h"

Graphics::Texture::Texture(void) :
	_Height(0),
	_Identifier(nullptr),
	_TextureName(0),
	_Width(0)
{
}

Graphics::Texture::Texture(const std::string & Identifier) :
	_Height(0),
	_Identifier(new std::string(Identifier)),
	_TextureName(0),
	_Width(0)
{
}

Graphics::Texture::~Texture(void)
{
	if(_TextureName != 0)
	{
		GLDeleteTextures(1, &_TextureName);
		_TextureName = 0;
	}
	if(_Identifier != nullptr)
	{
		delete _Identifier;
		_Identifier = nullptr;
	}
}

void Graphics::Texture::SetData(std::uint32_t Width, std::uint32_t Height, std::uint32_t Format, const unsigned char * Data)
{
	assert(_TextureName == 0);
	GLGenTextures(1, &_TextureName);
	assert(_TextureName != 0);
	GLBindTexture(GL_TEXTURE_2D, _TextureName);
	if(Format == 1) /// RGBA one byte each
	{
		GLTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
	}
	else if(Format == 2) /// RGB one byte each
	{
		GLTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
	}
	else if(Format == 3) /// premultiplied alpha RGBO one byte each
	{
		GLTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
	}
	else
	{
		throw;
	}
	GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	_Width = Width;
	_Height = Height;
}

void Graphics::Texture::Activate(void) const
{
	assert(_TextureName != 0);
	GLBindTexture(GL_TEXTURE_2D, _TextureName);
}

void Graphics::Texture::Create(std::uint32_t Width, std::uint32_t Height, std::uint32_t Format)
{
	assert(_TextureName == 0);
	GLGenTextures(1, &_TextureName);
	assert(_TextureName != 0);
	GLBindTexture(GL_TEXTURE_2D, _TextureName);
	if(Format == 1) /// RGBA one byte each
	{
		GLTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
	else if(Format == 2) /// RGB one byte each
	{
		GLTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	}
	else if(Format == 3) /// premultiplied alpha RGBO one byte each
	{
		GLTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	}
	else
	{
		throw;
	}
	GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	GLTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	_Width = Width;
	_Height = Height;
}

void Graphics::Texture::Deactivate(void) const
{
	GLBindTexture(GL_TEXTURE_2D, 0);
}
	
const std::string & Graphics::Texture::GetIdentifier(void)
{
	assert(_Identifier != nullptr);
	
	return *_Identifier;
}
