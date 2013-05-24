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

#include "object.h"
#include "object_aspect_accessory.h"
#include "object_aspect_visualization.h"
#include "slot.h"
#include "slot_class.h"
#include "visualizations.h"

Slot::Slot(const SlotClass * SlotClass, const std::string & Identifier) :
	m_SlotClass(SlotClass),
	m_Identifier(Identifier),
	m_MountedObject(0),
	_SelfReference(*this),
	_VisualizeAccessory(false)
{
}

Slot::~Slot(void)
{
	_SelfReference.Invalidate();
}

void Slot::Mount(Reference< Object > TheObject)
{
	assert(TheObject.IsValid() == true);
	assert(TheObject->GetAspectAccessory() != 0);
	assert(GetSlotClass()->AcceptsSlotClassIdentifier(TheObject->GetAspectAccessory()->GetSlotClassIdentifier()) == true);
	m_MountedObject = TheObject;
	TheObject->GetAspectAccessory()->SetSlot(this);
	if(_VisualizeAccessory == true)
	{
		assert(GetMountedObject()->GetAspectVisualization() != 0);
		assert(GetMountedObject()->GetAspectVisualization()->GetVisualization().IsValid() == false);
		assert(GetMountedObject()->GetContainer() != 0);
		assert(GetMountedObject()->GetContainer()->GetAspectVisualization() != 0);
		// only visualize if the parent object is visualized
		if(GetMountedObject()->GetContainer()->GetAspectVisualization()->GetVisualization().IsValid() == true)
		{
			VisualizeObject(GetMountedObject().Get(), GetMountedObject()->GetContainer()->GetAspectVisualization()->GetVisualization().Get());
		}
	}
}

void Slot::Unmount(void)
{
	assert(GetMountedObject().IsValid() == true);
	if(_VisualizeAccessory == true)
	{
		assert(GetMountedObject()->GetAspectVisualization() != 0);
		assert(GetMountedObject()->GetAspectVisualization()->GetVisualization().IsValid() == true);
		UnvisualizeObject(GetMountedObject().Get());
	}
	GetMountedObject()->GetAspectAccessory()->SetSlot(0);
	GetMountedObject().Clear();
}
