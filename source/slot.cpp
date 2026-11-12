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
#include "visualization.h"
#include "visualizations.h"

Slot::Slot(const SlotClass * SlotClass, const std::string & Identifier) :
	_Identifier(Identifier),
	_MountedObject(nullptr),
	_SlotClass(SlotClass),
	_VisualizeAccessory(false)
{
}

Slot::~Slot(void)
{
	assert(_MountedObject == nullptr);
	assert(m_MountedObjectDestroyingConnection.IsValid() == false);
	m_DestroyingSignal.Emit();
}

void Slot::Mount(Object * TheObject)
{
	assert(_MountedObject == nullptr);
	assert(m_MountedObjectDestroyingConnection.IsValid() == false);
	assert(TheObject != nullptr);
	assert(TheObject->GetAspectAccessory() != 0);
	assert(_SlotClass->AcceptsSlotClassIdentifier(TheObject->GetAspectAccessory()->GetSlotClassIdentifier()) == true);
	_MountedObject = TheObject;
	m_MountedObjectDestroyingConnection = _MountedObject->ConnectDestroyingCallback(std::bind(&Slot::_OnMountedObjectDestroying, this));
	_MountedObject->GetAspectAccessory()->SetSlot(this);
	if(_VisualizeAccessory == true)
	{
		assert(_MountedObject->GetAspectVisualization() != 0);
		assert(_MountedObject->GetContainer() != 0);
		assert(_MountedObject->GetContainer()->GetAspectVisualization() != 0);
		// only visualize if the parent object is visualized
		for(auto Visualization : _MountedObject->GetContainer()->GetAspectVisualization()->GetVisualizations())
		{
			VisualizeObject(_MountedObject, Visualization->GetGraphics());
		}
	}
}

void Slot::_OnMountedObjectDestroying(void)
{
	assert(_MountedObject != nullptr);
	_MountedObject = nullptr;
	assert(m_MountedObjectDestroyingConnection.IsValid() == true);
	m_MountedObjectDestroyingConnection.Disconnect();
	assert(m_MountedObjectDestroyingConnection.IsValid() == false);
}

void Slot::Unmount(void)
{
	assert(_MountedObject != nullptr);
	if(_VisualizeAccessory == true)
	{
		assert(_MountedObject->GetAspectVisualization() != 0);
		assert(_MountedObject->GetContainer() != 0);
		assert(_MountedObject->GetContainer()->GetAspectVisualization() != 0);
		for(auto Visualization : _MountedObject->GetContainer()->GetAspectVisualization()->GetVisualizations())
		{
			_MountedObject->GetAspectVisualization()->DestroyVisualization(Visualization->GetGraphics());
		}
	}
	assert(m_MountedObjectDestroyingConnection.IsValid() == true);
	m_MountedObjectDestroyingConnection.Disconnect();
	assert(m_MountedObjectDestroyingConnection.IsValid() == false);
	_MountedObject->GetAspectAccessory()->SetSlot(0);
	_MountedObject = nullptr;
}

auto Slot::ConnectDestroyingCallback(std::function<void()> Callback) -> Signals::Connection
{
	return m_DestroyingSignal.Connect(Callback);
}
