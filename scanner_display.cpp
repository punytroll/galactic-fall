/**
 * galactic-fall
 * Copyright (C) 2006  Hagen MÃ¶bius
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

#include <math3d/vector4f.h>

#include "scanner_display.h"
#include "ship.h"
#include "star.h"
#include "system.h"

ScannerDisplay::ScannerDisplay(void) :
	Widget(),
	m_Focus(0)
{
	m_Perspective.SetAspect(1.0f);
	m_Perspective.SetNearClippingPlane(1.0f);
	m_Perspective.SetFarClippingPlane(1000.0f);
}

void ScannerDisplay::Draw(void) const
{
	Widget::Draw();
	// scanner
	if((m_Focus != 0) && (m_Focus->GetTarget() != 0))
	{
		float RadialSize(m_Focus->GetTarget()->GetRadialSize());
		float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
		float FieldOfView(asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)));
		
		glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		// TODO: 0.0f is not the real value
		glViewport(static_cast< GLint >(GetGlobalPosition().m_V.m_A[0]), static_cast< GLint >(0.0f), static_cast< GLint >(GetSize().m_V.m_A[0]), static_cast< GLint >(GetSize().m_V.m_A[1]));
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		m_Perspective.SetFieldOfView(FieldOfView);
		m_Perspective.Draw();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		m_Camera.SetFieldOfView(FieldOfView);
		m_Camera.SetPosition(0.0f, 0.0f, 4.0f * RadialSize);
		m_Camera.SetFocus(m_Focus->GetTarget());
		m_Camera.Draw();
		if((m_Focus->GetCurrentSystem() != 0) && (m_Focus->GetCurrentSystem()->GetStar() != 0))
		{
			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glLightfv(GL_LIGHT0, GL_POSITION, math3d::vector4f(m_Focus->GetCurrentSystem()->GetStar()->GetPosition().m_V.m_A[0], m_Focus->GetCurrentSystem()->GetStar()->GetPosition().m_V.m_A[1], 100.0f, 0.0f).m_V.m_A);
		}
		glClear(GL_DEPTH_BUFFER_BIT);
		m_Focus->GetTarget()->Draw();
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glPopAttrib();
	}
}

void ScannerDisplay::SetFocus(Ship * Focus)
{
	m_Focus = Focus;
}
