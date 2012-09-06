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

#include <GL/gl.h>

#include "../color.h"
#include "../math/matrix4f.h"
#include "material.h"
#include "mesh.h"
#include "model.h"
#include "model_object.h"

Graphics::ModelObject::ModelObject(void) :
	m_ClearDepthBuffer(false),
	m_Model(0),
	m_Normalize(false),
	m_Scale(1.0f),
	m_UseBlending(false),
	m_UseLighting(false)
{
}

Graphics::ModelObject::~ModelObject(void)
{
	while(m_Materials.empty() == false)
	{
		delete m_Materials.begin()->second;
		m_Materials.erase(m_Materials.begin());
	}
}

void Graphics::ModelObject::Begin(void)
{
	glPushMatrix();
	glTranslatef(GetPosition()[0], GetPosition()[1], GetPosition()[2]);
	glMultMatrixf(Matrix4f(GetOrientation()).Transpose().Matrix());
	if(m_Scale != 1.0f)
	{
		glScalef(m_Scale, m_Scale, m_Scale);
	}
}

void Graphics::ModelObject::Draw(void)
{
	if(m_Model != 0)
	{
		if(m_ClearDepthBuffer == true)
		{
			glClear(GL_DEPTH_BUFFER_BIT);
		}
		if((m_Normalize || m_UseBlending || m_UseLighting) == true)
		{
			glPushAttrib(GL_ENABLE_BIT);
		}
		if(m_Normalize == true)
		{
			glEnable(GL_NORMALIZE);
		}
		if(m_UseBlending == true)
		{
			glEnable(GL_BLEND);
		}
		if(m_UseLighting == true)
		{
			glEnable(GL_LIGHTING);
		}
		
		const std::map< std::string, const Graphics::Mesh * > & Meshes(m_Model->GetMeshes());
		
		for(std::map< std::string, const Graphics::Mesh * >::const_iterator MeshIterator = Meshes.begin(); MeshIterator != Meshes.end(); ++MeshIterator)
		{
			std::map< std::string, Graphics::Material * >::const_iterator MaterialIterator(m_Materials.find(MeshIterator->first));
			
			if(m_UseLighting == true)
			{
				if(MaterialIterator == m_Materials.end())
				{
					// if no material is set for this mesh
					glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
					glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
					glMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
				}
				else
				{
					if(MaterialIterator->second->GetDiffuseColor() != 0)
					{
						glMaterialfv(GL_FRONT, GL_DIFFUSE, MaterialIterator->second->GetDiffuseColor()->GetColor().m_V.m_A);
					}
					else
					{
						glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
					}
					if(MaterialIterator->second->GetSpecularColor() != 0)
					{
						glMaterialf(GL_FRONT, GL_SHININESS, MaterialIterator->second->GetShininess());
						glMaterialfv(GL_FRONT, GL_SPECULAR, MaterialIterator->second->GetSpecularColor()->GetColor().m_V.m_A);
					}
					else
					{
						glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
						glMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
					}
				}
			}
			else
			{
				if((MaterialIterator != m_Materials.end()) && (MaterialIterator->second->GetDiffuseColor() != 0))
				{
					glColor4fv(MaterialIterator->second->GetDiffuseColor()->GetColor().m_V.m_A);
				}
			}
			MeshIterator->second->Draw();
		}
		if((m_Normalize || m_UseBlending || m_UseLighting) == true)
		{
			glPopAttrib();
		}
	}
}

void Graphics::ModelObject::End(void)
{
	glPopMatrix();
}

bool Graphics::ModelObject::AddMaterial(const std::string & MeshName, Graphics::Material * Material)
{
	if(m_Materials.find(MeshName) == m_Materials.end())
	{
		m_Materials[MeshName] = Material;
		
		return true;
	}
	else
	{
		return false;
	}
}
