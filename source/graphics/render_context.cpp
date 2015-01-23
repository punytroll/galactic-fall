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
#include <iostream>

#include "camera.h"
#include "engine.h"
#include "light.h"
#include "node.h"
#include "program.h"
#include "projection.h"
#include "render_context.h"
#include "shading_manager.h"
#include "style.h"

Graphics::RenderContext::RenderContext(void) :
	_Camera(nullptr),
	_Engine(nullptr),
	_Light(nullptr),
	_Node(nullptr),
	_Program(nullptr),
	_Style(nullptr)
{
}

Graphics::RenderContext::~RenderContext(void)
{
	assert(_Camera == nullptr);
	assert(_Engine == nullptr);
	assert(_Light == nullptr);
	assert(_Node == nullptr);
	assert(_Program == nullptr);
	assert(_Style== nullptr);
}

void Graphics::RenderContext::ActivateProgram(void)
{
	assert(_Engine != nullptr);
	
	auto ShadingManager(_Engine->GetShadingManager());
	
	assert(_Style != nullptr);
	assert(_Program == nullptr);
	_Program = ShadingManager->GetProgram(_Style->GetProgramIdentifier());
	assert(_Program != nullptr);
	_Program->_Activate();
	
	auto Uniforms(_Program->GetUniforms());
	
	for(auto & Uniform : Uniforms)
	{
		switch(Uniform.second)
		{
		case Graphics::Program::UniformContent::ViewToMonitorMatrix4x4F:
			{
				assert(_Camera != nullptr);
				assert(_Camera->GetProjection() != nullptr);
				_Program->_SetUniform(Uniform.first, _Camera->GetProjection()->GetMatrix());
				
				break;
			}
		case Graphics::Program::UniformContent::WorldToViewMatrix4x4F:
			{
				assert(_Camera != nullptr);
				_Program->_SetUniform(Uniform.first, _Camera->GetSpacialMatrix().Inverted());
				
				break;
			}
		case Graphics::Program::UniformContent::ModelToWorldMatrix4x4F:
			{
				assert(_Node != nullptr);
				_Program->_SetUniform(Uniform.first, _Node->GetSpacialMatrix().Scaled(_Node->GetScale()));
				
				break;
			}
		case Graphics::Program::UniformContent::WorldToMonitorMatrix4x4F:
			{
				assert(_Node != nullptr);
				_Program->_SetUniform(Uniform.first, _Camera->GetProjection()->GetMatrix().Transformed(_Camera->GetSpacialMatrix().Inverted()));
				
				break;
			}
		case Graphics::Program::UniformContent::ModelToMonitorMatrix4x4F:
			{
				assert(_Node != nullptr);
				_Program->_SetUniform(Uniform.first, _Camera->GetProjection()->GetMatrix().Transformed(_Camera->GetSpacialMatrix().Inverted()).Transform(_Node->GetSpacialMatrix().Scaled(_Node->GetScale())));
				
				break;
			}
		case Graphics::Program::UniformContent::ModelToWorldNormalMatrix3x3F:
			{
				assert(_Node != nullptr);
				_Program->_SetUniform(Uniform.first, Matrix3f::CreateFromTopLeftMatrix4f(_Node->GetSpacialMatrix()).Invert().Transpose());
				
				break;
			}
		case Graphics::Program::UniformContent::CameraPositionVector3F:
			{
				assert(_Camera != nullptr);
				_Program->_SetUniform(Uniform.first, Vector3f::CreateZero().Transform(_Camera->GetSpacialMatrix().Inverted()));
				
				break;
			}
		case Graphics::Program::UniformContent::LightDirectionVector3F:
			{
				assert(_Light != nullptr);
				assert(_Light->GetType() == Graphics::Light::Type::Directional);
				_Program->_SetUniform(Uniform.first, _Light->GetDirection());
				
				break;
			}
		case Graphics::Program::UniformContent::LightColorRGB3F:
			{
				assert(_Light != nullptr);
				_Program->_SetUniform(Uniform.first, _Light->GetColor());
				
				break;
			}
		case Graphics::Program::UniformContent::MaterialColorRGB3F:
			{
				assert(_Style != nullptr);
				assert(_Style->HasDiffuseColor() == true);
				_Program->_SetUniform(Uniform.first, Graphics::ColorRGB(_Style->GetDiffuseColor()));
				
				break;
			}
		case Graphics::Program::UniformContent::MaterialColorRGBO4F:
			{
				assert(_Style != nullptr);
				assert(_Style->HasDiffuseColor() == true);
				_Program->_SetUniform(Uniform.first, _Style->GetDiffuseColor());
				
				break;
			}
		case Graphics::Program::UniformContent::MaterialShininessF:
			{
				assert(_Style != nullptr);
				assert(_Style->HasShininess() == true);
				_Program->_SetUniform(Uniform.first, _Style->GetShininess());
				
				break;
			}
		}
	}
}

void Graphics::RenderContext::DeactivateProgram(void)
{
	assert(_Program != nullptr);
	_Program->_Deactivate();
	_Program = nullptr;
}
