/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include "scanner_display.h"
#include "ship.h"
#include "star.h"
#include "system.h"

ScannerDisplay::ScannerDisplay(void) :
	Widget()
{
	m_Perspective.SetAspect(1.0f);
	m_Perspective.SetNearClippingPlane(1.0f);
	m_Perspective.SetFarClippingPlane(1000.0f);
}

void ScannerDisplay::Update(void)
{
	if((m_Owner == true) && (m_Owner->GetTarget() == true))
	{
		float RadialSize(m_Owner->GetTarget()->GetRadialSize());
		float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
		float FieldOfView(asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)));
		
		m_Perspective.SetFieldOfView(FieldOfView);
		m_Camera.SetFieldOfView(FieldOfView);
		m_Camera.SetPosition(0.0f, 0.0f, 4.0f * RadialSize);
		m_Camera.SetFocus(m_Owner->GetTarget());
	}
}

void ScannerDisplay::Draw(void) const
{
	Widget::Draw();
	glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
	// clipping is performed by the viewport
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glViewport(static_cast< GLint >(GetGlobalPosition()[0]), static_cast< GLint >(GetRootWidget()->GetSize()[1] - GetGlobalPosition()[1] - GetSize()[1]), static_cast< GLint >(GetSize()[0]), static_cast< GLint >(GetSize()[1]));
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	m_Perspective.Draw();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	m_Camera.Draw();
	glClear(GL_DEPTH_BUFFER_BIT);
	// draw scanner
	if((m_Owner == true) && (m_Owner->GetTarget() == true))
	{
		glPushAttrib(GL_ENABLE_BIT);
		if((m_Owner->GetCurrentSystem() != 0) && (m_Owner->GetCurrentSystem()->GetStar() != 0))
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0, GL_POSITION, Vector4f(m_Owner->GetCurrentSystem()->GetStar()->GetPosition()[0], m_Owner->GetCurrentSystem()->GetStar()->GetPosition()[1], 100.0f, 0.0f).m_V.m_A);
		}
		m_Owner->GetTarget()->Draw();
		glPopAttrib();
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
