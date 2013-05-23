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

#include "../graphics/gl.h"
#include "../math/matrix4f.h"
#include "../object_aspect_physical.h"
#include "../object_aspect_position.h"
#include "../ship.h"
#include "scanner_display.h"

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

UI::ScannerDisplay::ScannerDisplay(UI::Widget * SupWidget) :
	UI::Widget(SupWidget)
{
}

void UI::ScannerDisplay::Update(void)
{
	if((_Owner.IsValid() == true) && (_Owner->GetTarget().IsValid() == true))
	{
		assert(_Owner->GetTarget()->GetAspectPhysical() != 0);
		
		float RadialSize(_Owner->GetTarget()->GetAspectPhysical()->GetRadialSize());
		float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
		
		_FieldOfView = asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize));
		_CameraZ = 4.0f * RadialSize;
	}
}

void UI::ScannerDisplay::Draw(void) const
{
	UI::Widget::Draw();
	glPushAttrib(GL_ENABLE_BIT | GL_VIEWPORT_BIT | GL_TRANSFORM_BIT);
	// clipping is performed by the viewport
	glDisable(GL_CLIP_PLANE0);
	glDisable(GL_CLIP_PLANE1);
	glDisable(GL_CLIP_PLANE2);
	glDisable(GL_CLIP_PLANE3);
	GLEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glViewport(static_cast< GLint >(GetGlobalPosition()[0]), static_cast< GLint >(GetRootWidget()->GetSize()[1] - GetGlobalPosition()[1] - GetSize()[1]), static_cast< GLint >(GetSize()[0]), static_cast< GLint >(GetSize()[1]));
	GLMatrixMode(GL_PROJECTION);
	GLPushMatrix();
	
	Matrix4f PerspectiveMatrix;
	
	CalculatePerspectiveMatrix(_FieldOfView, 1.0f, 1.0f, 1000.0f, PerspectiveMatrix);
	
	glLoadMatrixf(PerspectiveMatrix.Matrix());
	GLMatrixMode(GL_MODELVIEW);
	GLPushMatrix();
	GLLoadIdentity();
	if((_Owner.IsValid() == true) && (_Owner->GetTarget().IsValid() == true))
	{
		assert(_Owner->GetTarget()->GetAspectPosition() != 0);
		glTranslatef(-_Owner->GetTarget()->GetAspectPosition()->GetPosition().m_V.m_A[0], -_Owner->GetTarget()->GetAspectPosition()->GetPosition().m_V.m_A[1], -_CameraZ);
	}
	GLClear(GL_DEPTH_BUFFER_BIT);
	// draw viewport content
	// TODO: draw scanner content
	GLPopMatrix();
	GLMatrixMode(GL_PROJECTION);
	GLPopMatrix();
	glPopAttrib();
}
