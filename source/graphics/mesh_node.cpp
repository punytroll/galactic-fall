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

#include "mesh.h"
#include "mesh_node.h"
#include "render_context.h"
#include "style.h"

Graphics::MeshNode::MeshNode(void) :
	_Mesh(nullptr),
	_Style(nullptr)
{
	SetUseDepthTest(true);
}

Graphics::MeshNode::~MeshNode(void)
{
	delete _Style;
	_Style = nullptr;
}

void Graphics::MeshNode::Draw(Graphics::RenderContext * RenderContext)
{
	assert(_Style != nullptr);
	RenderContext->SetStyle(_Style);
	RenderContext->ActivateProgram();
	assert(_Mesh != nullptr);
	_Mesh->Draw(RenderContext);
	RenderContext->DeactivateProgram();
	RenderContext->SetStyle(nullptr);
}

void Graphics::MeshNode::SetStyle(const Graphics::Style * Style)
{
	delete _Style;
	_Style = Style;
}
