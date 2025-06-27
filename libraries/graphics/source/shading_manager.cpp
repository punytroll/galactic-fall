/**
 * galactic-fall
 * Copyright (C) 2015-2025  Hagen Möbius
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

#include <graphics/program.h>
#include <graphics/shader.h>
#include <graphics/shading_manager.h>

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

void Graphics::ShadingManager::BuildPrograms(void)
{
	for(auto & Program : _Programs)
	{
		Program.second->Build(this);
	}
	for(auto & Shader : _Shaders)
	{
		if(Shader.second->IsBuilt() == true)
		{
			Shader.second->Dispose();
		}
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
	if(ProgramIterator->second->IsBuilt() == true)
	{
		ProgramIterator->second->Dispose();
	}
	delete ProgramIterator->second;
	_Programs.erase(ProgramIterator);
}

void Graphics::ShadingManager::DestroyShader(const std::string & ShaderIdentifier)
{
	auto ShaderIterator(_Shaders.find(ShaderIdentifier));
	
	assert(ShaderIterator != _Shaders.end());
	if(ShaderIterator->second->IsBuilt() == true)
	{
		ShaderIterator->second->Dispose();
	}
	delete ShaderIterator->second;
	_Shaders.erase(ShaderIterator);
}

Graphics::Program * Graphics::ShadingManager::GetProgram(const std::string & ProgramIdentifier)
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

Graphics::Shader * Graphics::ShadingManager::GetShader(const std::string & ShaderIdentifier)
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
