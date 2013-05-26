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
#include "gl.h"
#include "material.h"
#include "mesh.h"
#include "model.h"
#include "model_node.h"

Graphics::ModelNode::ModelNode(void) :
	_Model(0)
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

void Graphics::ModelNode::Begin(void)
{
	Graphics::Node::Begin();
}

void Graphics::ModelNode::Draw(void)
{
	if(_Model != 0)
	{
		const std::map< std::string, const Graphics::Mesh * > & Meshes(_Model->GetMeshes());
		
		for(std::map< std::string, const Graphics::Mesh * >::const_iterator MeshIterator = Meshes.begin(); MeshIterator != Meshes.end(); ++MeshIterator)
		{
			std::map< std::string, Graphics::Material * >::const_iterator MaterialIterator(_Materials.find(MeshIterator->first));
			
			if(GetUseLighting() == true)
			{
				if(MaterialIterator == _Materials.end())
				{
					// if no material is set for this mesh
					GLMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
					GLMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
					GLMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
				}
				else
				{
					if(MaterialIterator->second->GetDiffuseColor() != 0)
					{
						GLMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialIterator->second->GetDiffuseColor()->GetColor().m_V.m_A);
					}
					else
					{
						GLMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
					}
					if(MaterialIterator->second->GetSpecularColor() != 0)
					{
						GLMaterialf(GL_FRONT, GL_SHININESS, MaterialIterator->second->GetShininess());
						GLMaterialfv(GL_FRONT, GL_SPECULAR, MaterialIterator->second->GetSpecularColor()->GetColor().m_V.m_A);
					}
					else
					{
						GLMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
						GLMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
					}
				}
			}
			else
			{
				if((MaterialIterator != _Materials.end()) && (MaterialIterator->second->GetDiffuseColor() != 0))
				{
					GLColor4fv(MaterialIterator->second->GetDiffuseColor()->GetColor().m_V.m_A);
				}
			}
			MeshIterator->second->Draw();
		}
	}
	Graphics::Node::Draw();
}

void Graphics::ModelNode::End(void)
{
	Graphics::Node::End();
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
