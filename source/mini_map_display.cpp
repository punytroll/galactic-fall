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

#include "commodity.h"
#include "mini_map_display.h"
#include "object_aspect_position.h"
#include "planet.h"
#include "ship.h"
#include "system.h"

MiniMapDisplay::MiniMapDisplay(Widget * SupWidget) :
	Viewport(SupWidget)
{
	GetCamera()->SetPosition(0.0f, 0.0f, 1500.0f);
	GetPerspective()->SetAspect(1.0f);
	GetPerspective()->SetFieldOfView(0.392699082f);
	GetPerspective()->SetNearClippingPlane(1.0f);
	GetPerspective()->SetFarClippingPlane(10000.0f);
}

void MiniMapDisplay::SetOwner(Reference< Ship > Owner)
{
	m_Owner = Owner;
	GetCamera()->SetFocus(Owner);
}

void MiniMapDisplay::DrawInViewport(void) const
{
	// draw mini map
	if((m_Owner.IsValid() == true) && (m_Owner->GetCurrentSystem() != 0))
	{
		
		const std::vector< Planet * > & Planets(m_Owner->GetCurrentSystem()->GetPlanets());
		const std::list< Ship * > & Ships(m_Owner->GetCurrentSystem()->GetShips());
		const std::list< Commodity * > & Commodities(m_Owner->GetCurrentSystem()->GetCommodities());
		
		glBegin(GL_POINTS);
		glColor3f(0.8f, 0.8f, 0.8f);
		for(std::vector< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
		{
			if(*PlanetIterator == m_Owner->GetTarget().Get())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*PlanetIterator)->GetAspectPosition()->GetPosition().m_V.m_A[0], (*PlanetIterator)->GetAspectPosition()->GetPosition().m_V.m_A[1]);
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
			glVertex2f((*ShipIterator)->GetAspectPosition()->GetPosition().m_V.m_A[0], (*ShipIterator)->GetAspectPosition()->GetPosition().m_V.m_A[1]);
			if(*ShipIterator == m_Owner->GetTarget().Get())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Commodity * >::const_iterator CommodityIterator = Commodities.begin(); CommodityIterator != Commodities.end(); ++CommodityIterator)
		{
			if(*CommodityIterator == m_Owner->GetTarget().Get())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*CommodityIterator)->GetAspectPosition()->GetPosition().m_V.m_A[0], (*CommodityIterator)->GetAspectPosition()->GetPosition().m_V.m_A[1]);
			if(*CommodityIterator == m_Owner->GetTarget().Get())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		glEnd();
	}
}
