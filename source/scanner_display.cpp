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

#include "object_aspect_physical.h"
#include "scanner_display.h"
#include "ship.h"
#include "star.h"
#include "system.h"

ScannerDisplay::ScannerDisplay(Widget * SupWidget) :
	Viewport(SupWidget)
{
	GetPerspective()->SetAspect(1.0f);
	GetPerspective()->SetNearClippingPlane(1.0f);
	GetPerspective()->SetFarClippingPlane(1000.0f);
}

void ScannerDisplay::Update(void)
{
	if((m_Owner.IsValid() == true) && (m_Owner->GetTarget().IsValid() == true))
	{
		assert(m_Owner->GetTarget()->GetAspectPhysical() != 0);
		
		float RadialSize(m_Owner->GetTarget()->GetAspectPhysical()->GetRadialSize());
		float ExtendedRadialSize((5.0f / 4.0f) * RadialSize);
		float FieldOfView(asinf(ExtendedRadialSize / sqrtf(ExtendedRadialSize * ExtendedRadialSize + 16 * RadialSize * RadialSize)));
		
		GetPerspective()->SetFieldOfView(FieldOfView);
		GetCamera()->SetPosition(0.0f, 0.0f, 4.0f * RadialSize);
		GetCamera()->SetFocus(m_Owner->GetTarget());
	}
}

void ScannerDisplay::DrawInViewport(void) const
{
	/// @todo draw scanner
}
