/**
 * galactic-fall
 * Copyright (C) 2006-2018  Hagen Möbius
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

#include <stdexcept>
#include <typeinfo>

#include <string_cast/string_cast.h>

#include <xml_stream/xml_stream.h>

#include "message.h"
#include "object.h"
#include "object_aspect_accessory.h"
#include "object_aspect_messages.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_outfitting.h"
#include "object_aspect_physical.h"
#include "object_aspect_position.h"
#include "object_aspect_update.h"
#include "object_aspect_visualization.h"
#include "real_time.h"

Object::Object(void) :
	_AspectAccessory(nullptr),
	_AspectMessages(nullptr),
	_AspectName(nullptr),
	_AspectObjectContainer(nullptr),
	_AspectOutfitting(nullptr),
	_AspectPhysical(nullptr),
	_AspectPosition(nullptr),
	_AspectUpdate(nullptr),
	_AspectVisualization(nullptr),
	_Container(nullptr)
{
}

Object::~Object(void)
{
	// make some object hierarchy integrity checks first
	assert(_Container == nullptr);
	// aspects
	delete _AspectAccessory;
	_AspectAccessory = nullptr;
	delete _AspectMessages;
	_AspectMessages = nullptr;
	delete _AspectName;
	_AspectName = nullptr;
	delete _AspectObjectContainer;
	_AspectObjectContainer = nullptr;
	delete _AspectOutfitting;
	_AspectOutfitting = nullptr;
	delete _AspectPhysical;
	_AspectPhysical = nullptr;
	delete _AspectPosition;
	_AspectPosition = nullptr;
	delete _AspectUpdate;
	_AspectUpdate = nullptr;
	delete _AspectVisualization;
	_AspectVisualization = nullptr;
}

void Object::AddAspectAccessory(void)
{
	assert(_AspectAccessory == nullptr);
	_AspectAccessory = new ObjectAspectAccessory();
}

void Object::AddAspectMessages(void)
{
	assert(_AspectMessages == nullptr);
	_AspectMessages = new ObjectAspectMessages();
}

void Object::AddAspectName(void)
{
	assert(_AspectName == nullptr);
	_AspectName = new ObjectAspectName();
}

void Object::AddAspectObjectContainer(void)
{
	assert(_AspectObjectContainer == nullptr);
	_AspectObjectContainer = new ObjectAspectObjectContainer(this);
}

void Object::AddAspectOutfitting(void)
{
	assert(_AspectOutfitting == nullptr);
	_AspectOutfitting = new ObjectAspectOutfitting();
}

void Object::AddAspectPhysical(void)
{
	assert(_AspectPhysical == nullptr);
	_AspectPhysical = new ObjectAspectPhysical();
}

void Object::AddAspectPosition(void)
{
	assert(_AspectPosition == nullptr);
	_AspectPosition = new ObjectAspectPosition();
}

void Object::AddAspectUpdate(void)
{
	assert(_AspectUpdate == nullptr);
	_AspectUpdate = new ObjectAspectUpdate();
}

void Object::AddAspectVisualization(void)
{
	assert(_AspectVisualization == nullptr);
	_AspectVisualization = new ObjectAspectVisualization();
}

Connection Object::ConnectDestroyingCallback(std::function< void (void) > Callback)
{
	return _DestroyingEvent.Connect(Callback);
}

void Object::DisconnectDestroyingCallback(Connection & Connection)
{
	_DestroyingEvent.Disconnect(Connection);
}

void Object::SetContainer(Object * Container)
{
	assert(((_Container == nullptr) || (Container == nullptr)) && (_Container != Container));
	_Container = Container;
}

void Object::Destroy(void)
{
	// fire destroying event before aything is actually destroyed
	_DestroyingEvent();
	// call destroy on all relevant aspects
	if(_AspectVisualization != nullptr)
	{
		_AspectVisualization->Destroy();
	}
	if(_AspectObjectContainer != nullptr)
	{
		_AspectObjectContainer->Destroy();
	}
	// remove from object hierarchy
	if(_Container != nullptr)
	{
		assert(_Container->GetAspectObjectContainer() != nullptr);
		_Container->GetAspectObjectContainer()->RemoveContent(this);
	}
}
