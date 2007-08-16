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

#include <algorithm>

#include "graphics_node.h"

Graphics::Node::Node(void) :
	m_Container(0)
{
}

Graphics::Node::~Node(void)
{
	if(m_Container != 0)
	{
		m_Container->RemoveNode(this);
		m_Container = 0;
	}
	Clear();
}

void Graphics::Node::Render(void)
{
	Draw();
	for(std::vector< Graphics::Node * >::iterator ContentIterator = m_Content.begin(); ContentIterator != m_Content.end(); ++ContentIterator)
	{
		(*ContentIterator)->Render();
	}
}

void Graphics::Node::Draw(void)
{
}

void Graphics::Node::AddNode(Graphics::Node * Content)
{
	m_Content.push_back(Content);
}

void Graphics::Node::RemoveNode(Graphics::Node * Content)
{
	m_Content.erase(std::find(m_Content.begin(), m_Content.end(), Content));
}

void Graphics::Node::Clear(void)
{
	while(m_Content.empty() == false)
	{
		Node * Content(m_Content.front());
		
		Content->SetContainer(0);
		delete Content;
	}
}
