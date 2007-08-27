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

#include "graphics_engine.h"
#include "graphics_node.h"
#include "math/matrix4f.h"

Graphics::Node::Node(void) :
	m_Container(0),
	m_Engine(0),
	m_Orientation(1.0f, 0.0f, 0.0f, 0.0f),
	m_Position(0.0f, 0.0f, 0.0f)
{
}

Graphics::Node::~Node(void)
{
	assert(m_Container == 0);
	assert(m_Content.empty() == true);
}

void Graphics::Node::Render(void)
{
	Begin();
	Draw();
	for(std::vector< Graphics::Node * >::iterator ContentIterator = m_Content.begin(); ContentIterator != m_Content.end(); ++ContentIterator)
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
	assert(Content->GetContainer() == 0);
	assert(GetEngine() != 0);
	assert(Content->GetEngine() == 0);
	Content->SetEngine(GetEngine());
	Content->SetContainer(this);
	m_Content.push_back(Content);
}

void Graphics::Node::RemoveNode(Graphics::Node * Content)
{
	assert(Content->GetContainer() == this);
	m_Content.erase(std::find(m_Content.begin(), m_Content.end(), Content));
	Content->SetContainer(0);
}

void Graphics::Node::Destroy(void)
{
	assert(GetContainer() == 0);
	while(m_Content.empty() == false)
	{
		Graphics::Node * Content(m_Content.back());
		
		RemoveNode(Content);
		Content->Destroy();
	}
	GetEngine()->OnDestroy(this);
}
