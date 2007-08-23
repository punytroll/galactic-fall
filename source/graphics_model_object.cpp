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

#include "color.h"
#include "graphics_model_object.h"
#include "math/matrix4f.h"
#include "model.h"

Graphics::ModelObject::ModelObject(void) :
	m_ClearDepthBuffer(false),
	m_DiffuseColor(0),
	m_Model(0),
	m_Normalize(false),
	m_Scale(1.0f),
	m_Shininess(0.0f),
	m_SpecularColor(0),
	m_UseBlending(false),
	m_UseLighting(false)
{
}

Graphics::ModelObject::~ModelObject(void)
{
	delete m_DiffuseColor;
	delete m_SpecularColor;
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
		glPushMatrix();
		glTranslatef(GetPosition()[0], GetPosition()[1], GetPosition()[2]);
		glMultMatrixf(Matrix4f(GetOrientation()).Transpose().Matrix());
		if(m_Scale != 1.0f)
		{
			glScalef(m_Scale, m_Scale, m_Scale);
		}
		if(m_UseLighting == true)
		{
			if(m_DiffuseColor != 0)
			{
				glMaterialfv(GL_FRONT, GL_DIFFUSE, m_DiffuseColor->GetColor().m_V.m_A);
			}
			else
			{
				glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
			}
			if(m_SpecularColor != 0)
			{
				glMaterialf(GL_FRONT, GL_SHININESS, m_Shininess);
				glMaterialfv(GL_FRONT, GL_SPECULAR, m_SpecularColor->GetColor().m_V.m_A);
			}
			else
			{
				glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
				glMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
			}
		}
		else
		{
			if(m_DiffuseColor != 0)
			{
				glColor4fv(m_DiffuseColor->GetColor().m_V.m_A);
			}
		}
		m_Model->Draw();
		glPopMatrix();
		if((m_Normalize || m_UseBlending || m_UseLighting) == true)
		{
			glPopAttrib();
		}
	}
}

void Graphics::ModelObject::SetDiffuseColor(const Color & DiffuseColor)
{
	delete m_DiffuseColor;
	m_DiffuseColor = new Color(DiffuseColor);
}

void Graphics::ModelObject::SetSpecularColor(const Color & SpecularColor)
{
	delete m_SpecularColor;
	m_SpecularColor = new Color(SpecularColor);
}
