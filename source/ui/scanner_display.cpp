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

#include "../graphics/gl.h"
#include "../object_aspect_physical.h"
#include "../object_aspect_position.h"
#include "../ship.h"
#include "scanner_display.h"

static void CalculatePerspectiveMatrix(float FieldOfView, float Aspect, float NearClippingPlane, float FarClippingPlane, Matrix4f & Matrix)
{
	float Top(NearClippingPlane * tan(FieldOfView));
	float Right(Top * Aspect);
	
	Matrix[0] = NearClippingPlane / Right;
	Matrix[1] = 0.0f;
	Matrix[2] = 0.0f;
	Matrix[3] = 0.0f;
	Matrix[4] = 0.0f;
	Matrix[5] = NearClippingPlane / Top;
	Matrix[6] = 0.0f;
	Matrix[7] = 0.0f;
	Matrix[8] = 0.0f;
	Matrix[9] = 0.0f;
	Matrix[10] = -(FarClippingPlane + NearClippingPlane) / (FarClippingPlane - NearClippingPlane);
	Matrix[11] = -1.0f;
	Matrix[12] = 0.0f;
	Matrix[13] = 0.0f;
	Matrix[14] = -(2.0f * FarClippingPlane * NearClippingPlane) / (FarClippingPlane - NearClippingPlane);
	Matrix[15] = 0.0f;
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
	
	CalculatePerspectiveMatrix(_FieldOfView, 1.0f, 1.0f, 1000.0f, PerspectiveMatrix);
	
	GLLoadMatrixf(PerspectiveMatrix.GetPointer());
	GLMatrixMode(GL_MODELVIEW);
	GLPushMatrix();
	GLLoadIdentity();
	if((_Owner.IsValid() == true) && (_Owner->GetTarget().IsValid() == true))
	{
		assert(_Owner->GetTarget()->GetAspectPosition() != 0);
		GLTranslatef(-_Owner->GetTarget()->GetAspectPosition()->GetPosition()[0], -_Owner->GetTarget()->GetAspectPosition()->GetPosition()[1], -_CameraZ);
	}
	GLClear(GL_DEPTH_BUFFER_BIT);
	// draw viewport content
	// TODO: draw scanner content
	GLPopMatrix();
	GLMatrixMode(GL_PROJECTION);
	GLPopMatrix();
	GLPopAttrib();
}
