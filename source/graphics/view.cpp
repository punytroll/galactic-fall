/**
 * galactic-fall
 * Copyright (C) 2013  Hagen Möbius
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

#include "../color.h"
#include "camera.h"
#include "gl.h"
#include "projection.h"
#include "render_target.h"
#include "scene.h"
#include "view.h"

Graphics::View::View(void) :
	_Camera(new Graphics::Camera()),
	_ClearColor(nullptr),
	_Engine(nullptr),
	_RenderTarget(nullptr),
	_Scene(nullptr)
{
}

Graphics::View::~View(void)
{
	assert(_Camera != nullptr);
	delete _Camera;
	_Camera = nullptr;
	delete _ClearColor;
	_ClearColor = nullptr;
	assert(_Engine == nullptr);
	assert(_RenderTarget == nullptr);
	assert(_Scene == nullptr);
}

void Graphics::View::Render(void)
{
	assert(_Engine != nullptr);
	if(_RenderTarget != nullptr)
	{
		_RenderTarget->Activate();
	}
	if(_ClearColor != nullptr)
	{
		GLClearColor(_ClearColor->GetColor()[0], _ClearColor->GetColor()[1], _ClearColor->GetColor()[2], _ClearColor->GetColor()[3]);
	}
	assert(_Camera != nullptr);
	assert(_Camera->GetProjection() != nullptr);
	GLMatrixMode(GL_PROJECTION);
	GLLoadMatrixf(_Camera->GetProjection()->GetMatrix().GetPointer());
	GLMatrixMode(GL_MODELVIEW);
	GLLoadMatrixf(_Camera->GetSpacialMatrix().Inverted().GetPointer());
	if(_Scene != nullptr)
	{
		_Scene->Render();
	}
}

void Graphics::View::SetClearColor(const Color & ClearColor)
{
	delete _ClearColor;
	_ClearColor = new Color(ClearColor);
}

void Graphics::View::_SetEngine(Graphics::Engine * Engine)
{
	assert(_Scene == nullptr);
	if(Engine == nullptr)
	{
		assert(_Engine != nullptr);
		_Engine = nullptr;
	}
	else
	{
		assert(_Engine == nullptr);
		_Engine = Engine;
	}
}

void Graphics::View::SetRenderTarget(Graphics::RenderTarget * RenderTarget)
{
	if(RenderTarget == nullptr)
	{
		assert(_RenderTarget != nullptr);
		_RenderTarget = nullptr;
	}
	else
	{
		assert(_RenderTarget == nullptr);
		_RenderTarget = RenderTarget;
	}
}

void Graphics::View::SetScene(Graphics::Scene * Scene)
{
	if(Scene == nullptr)
	{
		assert(_Scene != nullptr);
		_Scene->_SetEngine(nullptr);
		_Scene = nullptr;
	}
	else
	{
		assert(_Scene == nullptr);
		assert(Scene->GetEngine() == nullptr);
		assert(_Engine != nullptr);
		_Scene = Scene;
		_Scene->_SetEngine(_Engine);
	}
}
