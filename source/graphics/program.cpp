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

#include <iostream>

#include "../globals.h"
#include "gl.h"
#include "program.h"
#include "shader.h"
#include "shading_manager.h"

Graphics::Program::Program(const std::string & Identifier) :
	_Handle(0),
	_Identifier(Identifier)
{
}

Graphics::Program::~Program(void)
{
	assert(_Handle == 0);
}

void Graphics::Program::AddShaderIdentifier(const std::string & ShaderIdentifier)
{
	_ShaderIdentifiers.push_back(ShaderIdentifier);
}

void Graphics::Program::Build(Graphics::ShadingManager * ShadingManager)
{
	ON_DEBUG(std::cout << "Building program \"" << _Identifier << "\"." << std::endl);
	assert(ShadingManager != nullptr);
	assert(_Handle == 0);
	_Handle = GLCreateProgram();
	assert(_Handle != 0);
	for(auto & ShaderIdentifier : _ShaderIdentifiers)
	{
		auto Shader(ShadingManager->GetShader(ShaderIdentifier));
		
		if(Shader->IsBuilt() == false)
		{
			Shader->Build();
		}
		assert(Shader->GetHandle() != 0);
		GLAttachShader(_Handle, Shader->GetHandle());
	}
	GLLinkProgram(_Handle);
	
	GLint LinkStatus;
	
	GLGetProgramiv(_Handle, GL_LINK_STATUS, &LinkStatus);
	assert(LinkStatus == GL_TRUE);
	assert(_Uniforms.empty() == true);
	
	GLint ActiveAttributes;
	
	GLGetProgramiv(_Handle, GL_ACTIVE_ATTRIBUTES, &ActiveAttributes);
	ON_DEBUG(std::cout << "    Linked program has " << ActiveAttributes << " active attributes." << std::endl);
	for(auto AttributeIndex = 0; AttributeIndex < ActiveAttributes; ++AttributeIndex)
	{
		GLchar Name[1024];
		GLsizei NameLength;
		GLint Size;
		GLenum Type;
		
		GLGetActiveAttrib(_Handle, AttributeIndex, 1024, &NameLength, &Size, &Type, Name);
		ON_DEBUG(std::cout << "        Attribute \"" << Name << "\" with size " << Size << " and type " << Type << "." << std::endl);
	}
	
	GLint ActiveUniforms;
	
	GLGetProgramiv(_Handle, GL_ACTIVE_UNIFORMS, &ActiveUniforms);
	ON_DEBUG(std::cout << "    Linked program has " << ActiveUniforms << " active uniforms." << std::endl);
	for(auto UniformIndex = 0; UniformIndex < ActiveUniforms; ++UniformIndex)
	{
		GLchar Name[1024];
		GLsizei NameLength;
		GLint Size;
		GLenum Type;
		
		GLGetActiveUniform(_Handle, UniformIndex, 1024, &NameLength, &Size, &Type, Name);
		
		GLint Location(GLGetUniformLocation(_Handle, Name));
		
		ON_DEBUG(std::cout << "        Uniform \"" << Name << "\" with size " << Size << " and type " << Type << " at location " << Location << "." << std::endl);
		if((Name == std::string("in_ProjectionMatrix")) && (Type == GL_FLOAT_MAT4) && (Size == 1))
		{
			_Uniforms[Location] = Graphics::Program::UniformContent::ProjectionMatrix4x4F;
		}
		else if((Name == std::string("in_ViewMatrix")) && (Type == GL_FLOAT_MAT4) && (Size == 1))
		{
			_Uniforms[Location] = Graphics::Program::UniformContent::ViewMatrix4x4F;
		}
		else if((Name == std::string("in_ModelMatrix")) && (Type == GL_FLOAT_MAT4) && (Size == 1))
		{
			_Uniforms[Location] = Graphics::Program::UniformContent::ModelMatrix4x4F;
		}
		else if((Name == std::string("in_NormalMatrix")) && (Type == GL_FLOAT_MAT3) && (Size == 1))
		{
			_Uniforms[Location] = Graphics::Program::UniformContent::NormalMatrix3x3F;
		}
		else if((Name == std::string("in_CameraPosition")) && (Type == GL_FLOAT_VEC3) && (Size == 1))
		{
			_Uniforms[Location] = Graphics::Program::UniformContent::CameraPositionVector3F;
		}
		else if((Name == std::string("in_LightDirection")) && (Type == GL_FLOAT_VEC3) && (Size == 1))
		{
			_Uniforms[Location] = Graphics::Program::UniformContent::LightDirectionVector3F;
		}
		else if((Name == std::string("in_LightColor")) && (Type == GL_FLOAT_VEC3) && (Size == 1))
		{
			_Uniforms[Location] = Graphics::Program::UniformContent::LightColorVector3F;
		}
		else if((Name == std::string("in_MaterialColor")) && (Type == GL_FLOAT_VEC3) && (Size == 1))
		{
			_Uniforms[Location] = Graphics::Program::UniformContent::MaterialColorVector3F;
		}
		else if((Name == std::string("in_MaterialShininess")) && (Type == GL_FLOAT) && (Size == 1))
		{
			_Uniforms[Location] = Graphics::Program::UniformContent::MaterialShininessF;
		}
		else
		{
			ON_DEBUG(std::cout << "        => no recognized combination." << std::endl);
		}
	}
}

void Graphics::Program::Dispose(void)
{
	assert(_Handle != 0);
	GLDeleteProgram(_Handle);
	_Handle = 0;
	_Uniforms.clear();
}
