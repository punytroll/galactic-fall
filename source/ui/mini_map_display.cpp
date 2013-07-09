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

#include <algebra/matrix4f.h>

#include "../commodity.h"
#include "../graphics/gl.h"
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
	GLPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
	// clipping is performed by the viewport
	GLDisable(GL_CLIP_PLANE0);
	GLDisable(GL_CLIP_PLANE1);
	GLDisable(GL_CLIP_PLANE2);
	GLDisable(GL_CLIP_PLANE3);
	GLEnable(GL_DEPTH_TEST);
	GLDisable(GL_BLEND);
	GLViewport(static_cast< GLint >(GetGlobalPosition()[0]), static_cast< GLint >(GetRootWidget()->GetSize()[1] - GetGlobalPosition()[1] - GetSize()[1]), static_cast< GLint >(GetSize()[0]), static_cast< GLint >(GetSize()[1]));
	GLMatrixMode(GL_PROJECTION);
	GLPushMatrix();
	
	Matrix4f PerspectiveMatrix;
	
	CalculatePerspectiveMatrix(0.392699082f, 1.0f, 1.0f, 10000.0f, PerspectiveMatrix);
	GLLoadMatrixf(PerspectiveMatrix.Matrix());
	GLMatrixMode(GL_MODELVIEW);
	GLPushMatrix();
	GLLoadIdentity();
	if(_Owner.IsValid() == true)
	{
		GLTranslatef(-_Owner->GetAspectPosition()->GetPosition()[0], -_Owner->GetAspectPosition()->GetPosition()[1], -1500.0f);
	}
	GLClear(GL_DEPTH_BUFFER_BIT);
	// draw mini map
	if(_Owner.IsValid() == true)
	{
		const System * CurrentSystem(dynamic_cast< const System * >(_Owner->GetContainer()));
		
		assert(CurrentSystem != 0);
		
		const std::vector< Planet * > & Planets(CurrentSystem->GetPlanets());
		const std::list< Ship * > & Ships(CurrentSystem->GetShips());
		const std::list< Commodity * > & Commodities(CurrentSystem->GetCommodities());
		
		GLBegin(GL_POINTS);
		GLColor3f(0.8f, 0.8f, 0.8f);
		for(std::vector< Planet * >::const_iterator PlanetIterator = Planets.begin(); PlanetIterator != Planets.end(); ++PlanetIterator)
		{
			if(*PlanetIterator == _Owner->GetTarget().Get())
			{
				GLColor3f(0.2f, 1.0f, 0.0f);
			}
			GLVertex2f((*PlanetIterator)->GetAspectPosition()->GetPosition()[0], (*PlanetIterator)->GetAspectPosition()->GetPosition()[1]);
			if(*PlanetIterator == _Owner->GetTarget().Get())
			{
				GLColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Ship * >::const_iterator ShipIterator = Ships.begin(); ShipIterator != Ships.end(); ++ShipIterator)
		{
			if(*ShipIterator == _Owner->GetTarget().Get())
			{
				GLColor3f(0.2f, 1.0f, 0.0f);
			}
			GLVertex2f((*ShipIterator)->GetAspectPosition()->GetPosition()[0], (*ShipIterator)->GetAspectPosition()->GetPosition()[1]);
			if(*ShipIterator == _Owner->GetTarget().Get())
			{
				GLColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		for(std::list< Commodity * >::const_iterator CommodityIterator = Commodities.begin(); CommodityIterator != Commodities.end(); ++CommodityIterator)
		{
			if(*CommodityIterator == _Owner->GetTarget().Get())
			{
				GLColor3f(0.2f, 1.0f, 0.0f);
			}
			GLVertex2f((*CommodityIterator)->GetAspectPosition()->GetPosition()[0], (*CommodityIterator)->GetAspectPosition()->GetPosition()[1]);
			if(*CommodityIterator == _Owner->GetTarget().Get())
			{
				GLColor3f(0.8f, 0.8f, 0.8f);
			}
		}
		GLEnd();
	}
	GLPopMatrix();
	GLMatrixMode(GL_PROJECTION);
	GLPopMatrix();
	GLPopAttrib();
}
