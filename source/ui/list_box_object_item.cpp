/**
 * galactic-fall
 * Copyright (C) 2018  Hagen Möbius
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

#include <expressions/operators.h>

#include "../object.h"
#include "../object_aspect_name.h"
#include "event.h"
#include "label.h"
#include "list_box_object_item.h"

using namespace Expressions::Operators;

UI::ListBoxObjectItem::ListBoxObjectItem(Object * Object) :
	_Object(Object)
{
	SetHeight(20.0_c);
	assert(_Object != nullptr);
	ConnectDestroyingCallback(std::bind(&UI::ListBoxObjectItem::_OnDestroying, this, std::placeholders::_1));
	_ObjectDestroyingConnection = _Object->ConnectDestroyingCallback(std::bind(&UI::ListBoxObjectItem::_OnObjectDestroying, this));
	assert(_Object->GetAspectName() != nullptr);
	
	auto NameLabel{new UI::Label{this}};
	
	NameLabel->SetText(_Object->GetAspectName()->GetName());
	NameLabel->SetLeft(5.0_c);
	NameLabel->SetTop(0.0_c);
	NameLabel->SetWidth(width(this) - 2.0_c * 5.0_c);
	NameLabel->SetHeight(height(this));
	NameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
}

void UI::ListBoxObjectItem::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		assert(_Object != nullptr);
		_Object = nullptr;
		assert(_ObjectDestroyingConnection.IsValid() == true);
		_ObjectDestroyingConnection.Disconnect();
		assert(_ObjectDestroyingConnection.IsValid() == false);
	}
}

void UI::ListBoxObjectItem::_OnObjectDestroying(void)
{
	Destroy();
}
