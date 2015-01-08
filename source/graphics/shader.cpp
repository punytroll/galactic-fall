/**
 * galactic-fall
 * Copyright (C) 2015  Hagen Möbius
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

#include "shader.h"

Graphics::Shader::Shader(const std::string & Identifier) :
	_Handle(0),
	_Identifier(Identifier),
	_Type(Graphics::Shader::Type::Undefined)
{
}

Graphics::Shader::~Shader(void)
{
	assert(_Handle == 0);
}

void Graphics::Shader::Build(void)
{
	assert(_Handle == 0);
	if(_Type == Graphics::Shader::Type::FragmentShader)
	{
		_Handle = GLCreateShader(GL_FRAGMENT_SHADER);
	}
	else if(_Type == Graphics::Shader::Type::VertexShader)
	{
		_Handle = GLCreateShader(GL_VERTEX_SHADER);
	}
	else
	{
		assert(false);
	}
	
	auto Source(_Source.c_str());
	
	GLShaderSource(_Handle, 1, &Source, nullptr);
	GLCompileShader(_Handle);
	
	GLint CompileStatus;
	
	GLGetShaderiv(_Handle, GL_COMPILE_STATUS, &CompileStatus);
	assert(CompileStatus == GL_TRUE);
}

void Graphics::Shader::Dispose(void)
{
	assert(_Handle != 0);
	GLDeleteShader(_Handle);
	_Handle = 0;
}
