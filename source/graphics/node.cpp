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

#include "engine.h"
#include "node.h"
#include "scene.h"

Graphics::Node::Node(void) :
	_Container(0),
	_Orientation(1.0f, 0.0f, 0.0f, 0.0f),
	_Position(0.0f, 0.0f, 0.0f),
	_Scene(0)
{
}

Graphics::Node::~Node(void)
{
	assert(_Container == 0);
	assert(_Content.empty() == true);
}

void Graphics::Node::Render(void)
{
	Begin();
	Draw();
	for(std::vector< Graphics::Node * >::iterator ContentIterator = _Content.begin(); ContentIterator != _Content.end(); ++ContentIterator)
	{
		(*ContentIterator)->Render();
	}
	End();
}

void Graphics::Node::Begin(void)
{
}

void Graphics::Node::Draw(void)
{
}

void Graphics::Node::End(void)
{
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
	if(Scene->GetEngine() != 0)
	{
		Scene->GetEngine()->OnDestroy(this);
	}
}
