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

#include <GL/gl.h>

#include "../math/matrix4f.h"
#include "node.h"
#include "scene.h"

Graphics::Node::Node(void) :
	_ClearDepthBuffer(false),
	_Container(0),
	_Normalize(false),
	_Orientation(1.0f, 0.0f, 0.0f, 0.0f),
	_Position(0.0f, 0.0f, 0.0f),
	_Scale(1.0f),
	_Scene(0),
	_UseBlending(false),
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
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	glPushMatrix();
	glTranslatef(_Position[0], _Position[1], _Position[2]);
	glMultMatrixf(Matrix4f(_Orientation).Transpose().Matrix());
	if(_Scale != 1.0f)
	{
		glScalef(_Scale, _Scale, _Scale);
	}
	if(_Normalize == true)
	{
		glEnable(GL_NORMALIZE);
	}
	else
	{
		glDisable(GL_NORMALIZE);
	}
	if(_UseBlending == true)
	{
		glEnable(GL_BLEND);
	}
	else
	{
		glDisable(GL_BLEND);
	}
	if(_UseDepthTest == true)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
	if(_UseLighting == true)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
	if(_Use2DTexture == true)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
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
	glPopMatrix();
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
