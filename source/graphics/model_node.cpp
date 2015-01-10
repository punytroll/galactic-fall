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
#include "material.h"
#include "mesh.h"
#include "model.h"
#include "model_node.h"
#include "shading_manager.h"

Graphics::ModelNode::ModelNode(void) :
	_Model(nullptr)
{
	SetUseDepthTest(true);
}

Graphics::ModelNode::~ModelNode(void)
{
	while(_Materials.empty() == false)
	{
		delete _Materials.begin()->second;
		_Materials.erase(_Materials.begin());
	}
}

void Graphics::ModelNode::Draw(void)
{
	assert(_Model != nullptr);
	for(auto MeshPart : _Model->GetMeshes())
	{
		auto MaterialIterator(_Materials.find(MeshPart.first));
		
		assert(MaterialIterator != _Materials.end());
		assert(MaterialIterator->second != nullptr);
		assert(g_GraphicsEngine != nullptr);
		assert(g_GraphicsEngine->GetShadingManager() != nullptr);
		assert(g_GraphicsEngine->GetShadingManager()->GetProgram(MaterialIterator->second->GetProgramIdentifier()) != nullptr);
		if(GetUseLighting() == true)
		{
			if(MaterialIterator->second->GetDiffuseColor() != nullptr)
			{
				GLMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialIterator->second->GetDiffuseColor()->GetColor().GetPointer());
			}
			else
			{
				GLMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).GetPointer());
			}
			if(MaterialIterator->second->GetSpecularColor() != nullptr)
			{
				GLMaterialf(GL_FRONT, GL_SHININESS, MaterialIterator->second->GetShininess());
				GLMaterialfv(GL_FRONT, GL_SPECULAR, MaterialIterator->second->GetSpecularColor()->GetColor().GetPointer());
			}
			else
			{
				GLMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
				GLMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).GetPointer());
			}
		}
		else
		{
			if(MaterialIterator->second->GetDiffuseColor() != nullptr)
			{
				GLColor4fv(MaterialIterator->second->GetDiffuseColor()->GetColor().GetPointer());
			}
		}
		MeshPart.second->Draw();
	}
}

bool Graphics::ModelNode::AddMaterial(const std::string & MeshName, Graphics::Material * Material)
{
	if(_Materials.find(MeshName) == _Materials.end())
	{
		_Materials[MeshName] = Material;
		
		return true;
	}
	else
	{
		return false;
	}
}
