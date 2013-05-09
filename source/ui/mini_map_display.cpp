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

#include "../commodity.h"
#include "../math/matrix4f.h"
#include "../object_aspect_position.h"
#include "../planet.h"
#include "../ship.h"
#include "../system.h"
#include "mini_map_display.h"

static void CalculatePerspectiveMatrix(float FieldOfView, float Aspect, float NearClippingPlane, float FarClippingPlane, Matrix4f & Matrix)
{
	float Right, Top;
	
	Top = NearClippingPlane * tan(FieldOfView);
	Right = Top * Aspect;
	
	Matrix.m_M[1].m_A[0] = Matrix.m_M[2].m_A[0] = Matrix.m_M[3].m_A[0] = Matrix.m_M[0].m_A[1] = Matrix.m_M[2].m_A[1] = Matrix.m_M[3].m_A[1] = Matrix.m_M[0].m_A[2] = Matrix.m_M[1].m_A[2] = Matrix.m_M[0].m_A[3] = Matrix.m_M[1].m_A[3] = Matrix.m_M[3].m_A[3] = 0.0f;
	Matrix.m_M[0].m_A[0] = NearClippingPlane / Right;
	Matrix.m_M[1].m_A[1] = NearClippingPlane / Top;
	Matrix.m_M[2].m_A[2] = -(FarClippingPlane + NearClippingPlane) / (FarClippingPlane - NearClippingPlane);
	Matrix.m_M[3].m_A[2] = -(2.0f * FarClippingPlane * NearClippingPlane) / (FarClippingPlane - NearClippingPlane);
	Matrix.m_M[2].m_A[3] = -1.0f;
}

UI::MiniMapDisplay::MiniMapDisplay(UI::Widget * SupWidget) :
	UI::Widget(SupWidget)
{
}

void UI::MiniMapDisplay::SetOwner(Reference< Ship > Owner)
{
	_Owner = Owner;
}

void UI::MiniMapDisplay::Draw(void) const
{
	UI::Widget::Draw();
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
	
	Matrix4f PerspectiveMatrix;
	
	CalculatePerspectiveMatrix(0.392699082f, 1.0f, 1.0f, 10000.0f, PerspectiveMatrix);
	glLoadMatrixf(PerspectiveMatrix.Matrix());
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	if(_Owner.IsValid() == true)
	{
		glTranslatef(-_Owner->GetAspectPosition()->GetPosition().m_V.m_A[0], -_Owner->GetAspectPosition()->GetPosition().m_V.m_A[1], -1500.0f);
	}
	glClear(GL_DEPTH_BUFFER_BIT);
	// draw mini map
	if(_Owner.IsValid() == true)
	{
		const System * CurrentSystem(dynamic_cast< const System * >(_Owner->GetContainer()));
		
		assert(CurrentSystem != 0);
		
		const std::vector< Planet * > & Planets(CurrentSystem->GetPlanets());
		const std::list< Ship * > & Ships(CurrentSystem->GetShips());
		const std::list< Commodity * > & Commodities(CurrentSystem->GetCommodities());
		
		glBegin(GL_POINTS);
		glColor3f(0.8f, 0.8f, 0.8f);
		for(std::vector< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
		{
			if(*PlanetIterator == _Owner->GetTarget().Get())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*PlanetIterator)->GetAspectPosition()->GetPosition().m_V.m_A[0], (*PlanetIterator)->GetAspectPosition()->GetPosition().m_V.m_A[1]);
			if(*PlanetIterator == _Owner->GetTarget().Get())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
		{
			if(*ShipIterator == _Owner->GetTarget().Get())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*ShipIterator)->GetAspectPosition()->GetPosition().m_V.m_A[0], (*ShipIterator)->GetAspectPosition()->GetPosition().m_V.m_A[1]);
			if(*ShipIterator == _Owner->GetTarget().Get())
			{
				glColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Commodity * >::const_iterator CommodityIterator = Commodities.begin(); CommodityIterator != Commodities.end(); ++CommodityIterator)
		{
			if(*CommodityIterator == _Owner->GetTarget().Get())
			{
				glColor3f(0.2f, 1.0f, 0.0f);
			}
			glVertex2f((*CommodityIterator)->GetAspectPosition()->GetPosition().m_V.m_A[0], (*CommodityIterator)->GetAspectPosition()->GetPosition().m_V.m_A[1]);
			if(*CommodityIterator == _Owner->GetTarget().Get())
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
