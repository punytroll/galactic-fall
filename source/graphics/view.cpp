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

#include <GL/gl.h>

#include "camera.h"
#include "projection.h"
#include "render_target.h"
#include "scene.h"
#include "view.h"

Graphics::View::View(void) :
	_Camera(new Graphics::Camera()),
	_Engine(0),
	_RenderTarget(0),
	_Scene(0)
{
}

Graphics::View::~View(void)
{
	assert(_Camera != 0);
	delete _Camera;
	_Camera = 0;
	assert(_Engine == 0);
	assert(_RenderTarget == 0);
	assert(_Scene == 0);
}

void Graphics::View::Render(void)
{
	assert(_Engine != 0);
	if(_RenderTarget != 0)
	{
		_RenderTarget->Activate();
	}
	assert(_Camera != 0);
	assert(_Camera->GetProjection() != 0);
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(_Camera->GetProjection()->GetMatrix().Transposed().Matrix());
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(_Camera->GetSpacialMatrix().Inverted().Transpose().Matrix());
	if(_Scene != 0)
	{
		_Scene->Render();
	}
}

void Graphics::View::_SetEngine(Graphics::Engine * Engine)
{
	assert(_Scene == 0);
	if(Engine == 0)
	{
		assert(_Engine != 0);
		_Engine = 0;
	}
	else
	{
		assert(_Engine == 0);
		_Engine = Engine;
	}
}

void Graphics::View::SetRenderTarget(Graphics::RenderTarget * RenderTarget)
{
	if(RenderTarget == 0)
	{
		assert(_RenderTarget != 0);
		_RenderTarget = 0;
	}
	else
	{
		assert(_RenderTarget == 0);
		_RenderTarget = RenderTarget;
	}
}

void Graphics::View::SetScene(Graphics::Scene * Scene)
{
	if(Scene == 0)
	{
		assert(_Scene != 0);
		_Scene->_SetEngine(0);
		_Scene = 0;
	}
	else
	{
		assert(_Scene == 0);
		assert(Scene->GetEngine() == 0);
		assert(_Engine != 0);
		_Scene = Scene;
		_Scene->_SetEngine(_Engine);
	}
}
