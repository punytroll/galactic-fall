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
	m_Color(0),
	m_Model(0),
	m_Normalize(false),
	m_Scale(1.0f)
{
}

Graphics::ModelObject::~ModelObject(void)
{
	delete m_Color;
}

void Graphics::ModelObject::Draw(void)
{
	if(m_Model != 0)
	{
		if(m_Normalize == true)
		{
			glPushAttrib(GL_ENABLE_BIT);
			glEnable(GL_NORMALIZE);
		}
		glPushMatrix();
		glTranslatef(GetPosition()[0], GetPosition()[1], GetPosition()[2]);
		glMultMatrixf(Matrix4f(GetOrientation()).Transpose().Matrix());
		if(m_Scale != 1.0f)
		{
			glScalef(m_Scale, m_Scale, m_Scale);
		}
		glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);
		if(m_Color != 0)
		{
			glMaterialfv(GL_FRONT, GL_DIFFUSE, m_Color->GetColor().m_V.m_A);
			glMaterialfv(GL_FRONT, GL_SPECULAR, (Vector4f(1.0f, 1.0f, 1.0f, 1.0f) - m_Color->GetColor()).m_V.m_A);
		}
		else
		{
			glMaterialfv(GL_FRONT, GL_DIFFUSE, Vector4f(1.0f, 1.0f, 1.0f, 1.0f).m_V.m_A);
			glMaterialfv(GL_FRONT, GL_SPECULAR, Vector4f(0.0f, 0.0f, 0.0f, 1.0f).m_V.m_A);
		}
		m_Model->Draw();
		glPopMatrix();
		if(m_Normalize == true)
		{
			glPopAttrib();
		}
	}
}

void Graphics::ModelObject::SetColor(const Color & NewColor)
{
	delete m_Color;
	m_Color = new Color(NewColor);
}
