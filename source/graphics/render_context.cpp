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
#include "gl.h"
#include "light.h"
#include "node.h"
#include "program.h"
#include "projection.h"
#include "render_context.h"
#include "shading_manager.h"
#include "style.h"
#include "texture.h"

Graphics::RenderContext::RenderContext(void) :
	_Camera(nullptr),
	_Clipping(nullptr),
	_Engine(nullptr),
	_Light(nullptr),
	_Node(nullptr),
	_Program(nullptr),
	_Style(nullptr),
	_Texture(nullptr)
{
}

Graphics::RenderContext::~RenderContext(void)
{
	assert(_Camera == nullptr);
	assert(_Clipping == nullptr);
	assert(_Engine == nullptr);
	assert(_Light == nullptr);
	assert(_Node == nullptr);
	assert(_Program == nullptr);
	assert(_Style == nullptr);
	assert(_Texture == nullptr);
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
		case Graphics::Program::UniformContent::Clipping4F:
			{
				assert(_Clipping != nullptr);
				_Program->_SetUniform(Uniform.first, *_Clipping);
				
				break;
			}
		case Graphics::Program::UniformContent::Texture:
			{
				assert(_Texture != nullptr);
				_Program->_SetUniform(Uniform.first, 0);
				GLActiveTexture(GL_TEXTURE0);
				_Texture->Activate();
				
				break;
			}
		}
	}
}

void Graphics::RenderContext::DeactivateProgram(void)
{
	if(_Texture != nullptr)
	{
		GLActiveTexture(GL_TEXTURE0);
		_Texture->Deactivate();
	}
	assert(_Program != nullptr);
	_Program->_Deactivate();
	_Program = nullptr;
}

void Graphics::RenderContext::SetCamera(Graphics::Camera * Camera)
{
	assert((_Camera == nullptr) || (Camera == nullptr));
	_Camera = Camera;
}

void Graphics::RenderContext::SetClipping(float Left, float Top, float Bottom, float Right)
{
	UnsetClipping();
	_Clipping = new Vector4f(Left, Top, Bottom, Right);
}

void Graphics::RenderContext::SetEngine(Graphics::Engine * Engine)
{
	assert((_Engine == nullptr) || (Engine == nullptr));
	_Engine = Engine;
}

void Graphics::RenderContext::SetLight(Graphics::Light * Light)
{
	assert((_Light == nullptr) || (Light == nullptr));
	_Light = Light;
}

void Graphics::RenderContext::SetNode(Graphics::Node * Node)
{
	assert((_Node == nullptr) || (Node == nullptr));
	_Node = Node;
}

void Graphics::RenderContext::SetStyle(Graphics::Style * Style)
{
	assert((_Style == nullptr) || (Style == nullptr));
	_Style = Style;
}

void Graphics::RenderContext::SetTexture(const Graphics::Texture * Texture)
{
	assert((_Texture == nullptr) || (Texture == nullptr));
	_Texture = Texture;
}

void Graphics::RenderContext::UnsetClipping(void)
{
	if(_Clipping != nullptr)
	{
		delete _Clipping;
		_Clipping = nullptr;
	}
}
