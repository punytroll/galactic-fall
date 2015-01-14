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

#include <algebra/matrix4f.h>

#include "../color.h"
#include "../globals.h"
#include "engine.h"
#include "gl.h"
#include "mesh.h"
#include "model.h"
#include "model_node.h"
#include "render_context.h"
#include "shading_manager.h"
#include "style.h"

Graphics::ModelNode::ModelNode(void) :
	_Model(nullptr)
{
	SetUseDepthTest(true);
}

Graphics::ModelNode::~ModelNode(void)
{
	while(_Styles.empty() == false)
	{
		delete _Styles.begin()->second;
		_Styles.erase(_Styles.begin());
	}
}

void Graphics::ModelNode::Draw(Graphics::RenderContext * RenderContext)
{
	assert(_Model != nullptr);
	for(auto MeshPart : _Model->GetMeshes())
	{
		auto StyleIterator(_Styles.find(MeshPart.first));
		
		assert(StyleIterator != _Styles.end());
		assert(StyleIterator->second != nullptr);
		RenderContext->SetStyle(StyleIterator->second);
		RenderContext->ActivateProgram();
		MeshPart.second->Draw(RenderContext);
		RenderContext->DeactivateProgram();
		RenderContext->SetStyle(nullptr);
	}
}

void Graphics::ModelNode::AddStyle(const std::string & MeshName, Graphics::Style * Style)
{
	assert(_Styles.find(MeshName) == _Styles.end());
	_Styles[MeshName] = Style;
}
