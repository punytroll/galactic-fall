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
		assert(g_GraphicsEngine != nullptr);
		assert(g_GraphicsEngine->GetShadingManager() != nullptr);
		assert(g_GraphicsEngine->GetShadingManager()->GetProgram(StyleIterator->second->GetProgramIdentifier()) != nullptr);
		if(GetUseLighting() == true)
		{
			if(StyleIterator->second->GetDiffuseColor() != nullptr)
			{
				GLMaterialfv(GL_FRONT, GL_DIFFUSE, StyleIterator->second->GetDiffuseColor()->GetColor().GetPointer());
			}
			else
			{
				GLMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).GetPointer());
			}
			if(StyleIterator->second->GetSpecularColor() != nullptr)
			{
				GLMaterialf(GL_FRONT, GL_SHININESS, StyleIterator->second->GetShininess());
				GLMaterialfv(GL_FRONT, GL_SPECULAR, StyleIterator->second->GetSpecularColor()->GetColor().GetPointer());
			}
			else
			{
				GLMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
				GLMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).GetPointer());
			}
		}
		else
		{
			if(StyleIterator->second->GetDiffuseColor() != nullptr)
			{
				GLColor4fv(StyleIterator->second->GetDiffuseColor()->GetColor().GetPointer());
			}
		}
		MeshPart.second->Draw(RenderContext);
		RenderContext->SetStyle(nullptr);
	}
}

void Graphics::ModelNode::AddStyle(const std::string & MeshName, Graphics::Style * Style)
{
	assert(_Styles.find(MeshName) == _Styles.end());
	_Styles[MeshName] = Style;
}
