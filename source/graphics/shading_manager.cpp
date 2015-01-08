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

#include <cassert>

#include "program.h"
#include "shader.h"
#include "shading_manager.h"

Graphics::ShadingManager::~ShadingManager(void)
{
	while(_Programs.empty() == false)
	{
		DestroyProgram(_Programs.begin()->first);
	}
	while(_Shaders.empty() == false)
	{
		DestroyShader(_Shaders.begin()->first);
	}
}

Graphics::Program * Graphics::ShadingManager::CreateProgram(const std::string & ProgramIdentifier)
{
	assert(_Programs.find(ProgramIdentifier) == _Programs.end());
	
	auto Program(new Graphics::Program(ProgramIdentifier));
	
	_Programs[Program->GetIdentifier()] = Program;
	
	return Program;
}

Graphics::Shader * Graphics::ShadingManager::CreateShader(const std::string & ShaderIdentifier)
{
	assert(_Shaders.find(ShaderIdentifier) == _Shaders.end());
	
	auto Shader(new Graphics::Shader(ShaderIdentifier));
	
	_Shaders[Shader->GetIdentifier()] = Shader;
	
	return Shader;
}

void Graphics::ShadingManager::DestroyProgram(const std::string & ProgramIdentifier)
{
	auto ProgramIterator(_Programs.find(ProgramIdentifier));
	
	assert(ProgramIterator != _Programs.end());
	delete ProgramIterator->second;
	_Programs.erase(ProgramIterator);
}

void Graphics::ShadingManager::DestroyShader(const std::string & ShaderIdentifier)
{
	auto ShaderIterator(_Shaders.find(ShaderIdentifier));
	
	assert(ShaderIterator != _Shaders.end());
	delete ShaderIterator->second;
	_Shaders.erase(ShaderIterator);
}

const Graphics::Program * Graphics::ShadingManager::GetProgram(const std::string & ProgramIdentifier) const
{
	auto ProgramIterator(_Programs.find(ProgramIdentifier));
	
	if(ProgramIterator == _Programs.end())
	{
		return nullptr;
	}
	else
	{
		return ProgramIterator->second;
	}
}

std::vector< std::string > Graphics::ShadingManager::GetProgramIdentifiers(void) const
{
	std::vector< std::string > Result;
	
	for(auto Program : _Programs)
	{
		Result.push_back(Program.first);
	}
	
	return Result;
}

const Graphics::Shader * Graphics::ShadingManager::GetShader(const std::string & ShaderIdentifier) const
{
	auto ShaderIterator(_Shaders.find(ShaderIdentifier));
	
	if(ShaderIterator == _Shaders.end())
	{
		return nullptr;
	}
	else
	{
		return ShaderIterator->second;
	}
}
