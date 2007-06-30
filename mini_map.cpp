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

#include "cargo.h"
#include "mini_map.h"
#include "planet.h"
#include "ship.h"
#include "system.h"

MiniMap::MiniMap(void) :
	Widget()
{
	m_Camera.SetFieldOfView(0.392699082f);
	m_Camera.SetPosition(0.0f, 0.0f, 1500.0f);
	m_Perspective.SetAspect(1.0f);
	m_Perspective.SetFieldOfView(0.392699082f);
	m_Perspective.SetNearClippingPlane(1.0f);
	m_Perspective.SetFarClippingPlane(10000.0f);
}

void MiniMap::SetOwner(Reference< Ship > Owner)
{
	m_Owner = Owner;
	m_Camera.SetFocus(Owner);
}

void MiniMap::Draw(void) const
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
	// draw mini map
	if((m_Owner == true) && (m_Owner->GetCurrentSystem() != 0))
	{
		
		const std::vector< Planet * > & Planets(m_Owner->GetCurrentSystem()->GetPlanets());
		const std::list< Ship * > & Ships(m_Owner->GetCurrentSystem()->GetShips());
		const std::list< Cargo * > & Cargos(m_Owner->GetCurrentSystem()->GetCargos());
		
		glBegin(GL_POINTS);
		glColor3f(0.8f, 0.8f, 0.8f);
		for(std::vector< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
		{
			if(*PlanetIterator == m_Owner->GetTarget().Get())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*PlanetIterator)->GetPosition().m_V.m_A[0], (*PlanetIterator)->GetPosition().m_V.m_A[1]);
			if(*PlanetIterator == m_Owner->GetTarget().Get())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
		{
			if(*ShipIterator == m_Owner->GetTarget().Get())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*ShipIterator)->GetPosition().m_V.m_A[0], (*ShipIterator)->GetPosition().m_V.m_A[1]);
			if(*ShipIterator == m_Owner->GetTarget().Get())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Cargo * >::const_iterator CargoIterator = Cargos.begin(); CargoIterator != Cargos.end(); ++CargoIterator)
		{
			if(*CargoIterator == m_Owner->GetTarget().Get())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*CargoIterator)->GetPosition().m_V.m_A[0], (*CargoIterator)->GetPosition().m_V.m_A[1]);
			if(*CargoIterator == m_Owner->GetTarget().Get())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		glEnd();
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}
