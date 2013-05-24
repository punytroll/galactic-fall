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

#include <algorithm>

#include "../math/matrix4f.h"
#include "gl.h"
#include "node.h"
#include "scene.h"

Graphics::Node::Node(void) :
	_BlendFunctionDestinationFactor(GL_ZERO),
	_BlendFunctionSourceFactor(GL_ONE),
	_ClearColorBuffer(false),
	_ClearDepthBuffer(false),
	_Container(0),
	_Normalize(false),
	_Orientation(1.0f, 0.0f, 0.0f, 0.0f),
	_Position(0.0f, 0.0f, 0.0f),
	_Scale(1.0f),
	_Scene(0),
	_UseBlending(false),
	_UseClipPlane0(false),
	_UseClipPlane1(false),
	_UseClipPlane2(false),
	_UseClipPlane3(false),
	_UseDepthTest(false),
	_UseLighting(false),
	_Use2DTexture(false)
{
}

Graphics::Node::~Node(void)
{
	assert(_Container == 0);
	assert(_Content.empty() == true);
}

void Graphics::Node::Begin(void)
{
	if(_ClearDepthBuffer == true)
	{
		GLClear(GL_DEPTH_BUFFER_BIT);
	}
	if(_ClearColorBuffer == true)
	{
		GLClear(GL_COLOR_BUFFER_BIT);
	}
	GLPushMatrix();
	GLTranslatef(_Position[0], _Position[1], _Position[2]);
	glMultMatrixf(Matrix4f(_Orientation).Transpose().Matrix());
	if(_Scale != 1.0f)
	{
		glScalef(_Scale, _Scale, _Scale);
	}
	GLPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
	if(_Normalize == true)
	{
		GLEnable(GL_NORMALIZE);
	}
	else
	{
		glDisable(GL_NORMALIZE);
	}
	if(_UseBlending == true)
	{
		GLEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	if(_UseClipPlane0 == true)
	{
		GLEnable(GL_CLIP_PLANE0);
	}
	else
	{
		glDisable(GL_CLIP_PLANE0);
	}
	if(_UseClipPlane1 == true)
	{
		GLEnable(GL_CLIP_PLANE1);
	}
	else
	{
		glDisable(GL_CLIP_PLANE1);
	}
	if(_UseClipPlane2 == true)
	{
		GLEnable(GL_CLIP_PLANE2);
	}
	else
	{
		glDisable(GL_CLIP_PLANE2);
	}
	if(_UseClipPlane3 == true)
	{
		GLEnable(GL_CLIP_PLANE3);
	}
	else
	{
		glDisable(GL_CLIP_PLANE3);
	}
	if(_UseDepthTest == true)
	{
		GLEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	if(_UseLighting == true)
	{
		GLEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
	if(_Use2DTexture == true)
	{
		GLEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
	glBlendFunc(_BlendFunctionSourceFactor, _BlendFunctionDestinationFactor);
}

void Graphics::Node::Draw(void)
{
	for(std::vector< Graphics::Node * >::iterator ContentIterator = _Content.begin(); ContentIterator != _Content.end(); ++ContentIterator)
	{
		(*ContentIterator)->Begin();
		(*ContentIterator)->Draw();
		(*ContentIterator)->End();
	}
}

void Graphics::Node::End(void)
{
	GLPopAttrib();
	GLPopMatrix();
}

void Graphics::Node::AddNode(Graphics::Node * Content)
{
	assert(Content->_Container == 0);
	assert(_Scene != 0);
	assert(Content->_Scene == 0);
	Content->_Scene = _Scene;
	Content->_Container = this;
	_Content.push_back(Content);
}

void Graphics::Node::Destroy(void)
{
	assert(_Scene != 0);
	while(_Content.empty() == false)
	{
		_Content.back()->Destroy();
	}
	if(_Container != 0)
	{
		_Container->_Content.erase(std::find(_Container->_Content.begin(), _Container->_Content.end(), this));
		_Container = 0;
	}
	
	Graphics::Scene * Scene(_Scene);
	
	_Scene = 0;
	Scene->OnDestroy(this);
}
