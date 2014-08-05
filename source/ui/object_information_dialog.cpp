/**
 * galactic-fall
 * Copyright (C) 2008  Hagen Möbius
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

#include <string_cast/string_cast.h>

#include "../color.h"
#include "../globals.h"
#include "../message.h"
#include "../object.h"
#include "../object_aspect_accessory.h"
#include "../object_aspect_messages.h"
#include "../object_aspect_name.h"
#include "../object_aspect_object_container.h"
#include "../object_aspect_outfitting.h"
#include "../object_aspect_physical.h"
#include "../object_aspect_position.h"
#include "../object_aspect_visualization.h"
#include "../slot.h"
#include "../slot_class.h"
#include "label.h"
#include "mouse_button_event.h"
#include "object_information_dialog.h"
#include "scroll_bar.h"
#include "scroll_box.h"
#include "text_button.h"

UI::ObjectInformationDialog::ObjectInformationDialog(UI::Widget * SupWidget, const Reference< Object > & Object) :
	UI::Window(SupWidget, "Object Information"),
	_Object(Object),
	_CloseButton(0),
	_RefreshButton(0)
{
	SetPosition(Vector2f(100.0f, 400.0f));
	SetSize(Vector2f(500.0f, 300.0f));
	// set up widgets
	_CloseButton = new UI::TextButton(this, "Close");
	_CloseButton->SetSize(Vector2f(100.0f, 20.0f));
	_CloseButton->SetPosition(Vector2f(GetSize()[0] - 10.0f - _CloseButton->GetSize()[0], GetSize()[1] - 10.0f - _CloseButton->GetSize()[1]));
	_CloseButton->SetAnchorBottom(true);
	_CloseButton->SetAnchorLeft(false);
	_CloseButton->SetAnchorRight(true);
	_CloseButton->SetAnchorTop(false);
	_CloseButton->ConnectClickedCallback(std::bind(&ObjectInformationDialog::_OnCloseClicked, this));
	_PropertiesScrollBox = new UI::ScrollBox(this);
	_PropertiesScrollBox->SetPosition(Vector2f(10.0f, 40.0f));
	_PropertiesScrollBox->SetSize(Vector2f(GetSize()[0] - 20.0f, GetSize()[1] - 80.0f));
	_PropertiesScrollBox->SetHorizontalScrollBarVisible(false);
	_PropertiesScrollBox->SetAnchorBottom(true);
	_PropertiesScrollBox->SetAnchorRight(true);
	_RefreshButton = new UI::TextButton(this, "Refresh");
	_RefreshButton->SetSize(Vector2f(100.0f, 20.0f));
	_RefreshButton->SetPosition(Vector2f(_CloseButton->GetPosition()[0] - 10.0f - _RefreshButton->GetSize()[0], GetSize()[1] - 10.0f - _RefreshButton->GetSize()[1]));
	_RefreshButton->SetAnchorBottom(true);
	_RefreshButton->SetAnchorLeft(false);
	_RefreshButton->SetAnchorRight(true);
	_RefreshButton->SetAnchorTop(false);
	_RefreshButton->ConnectClickedCallback(std::bind(&ObjectInformationDialog::_OnRefreshClicked, this));
	_Refresh();
}

float UI::ObjectInformationDialog::_AddObjectProperty(float Top, float Indentation, const Reference< Object > & Object)
{
	UI::Widget * PropertyDisplay(new UI::Widget(_PropertiesScrollBox->GetContent()));
	
	PropertyDisplay->SetPosition(Vector2f(10.0f, Top));
	PropertyDisplay->SetSize(Vector2f(_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 30.0f));
	PropertyDisplay->SetAnchorRight(true);
	
	UI::Button * ObjectButton(new UI::TextButton(PropertyDisplay, Object->GetObjectIdentifier()));
	
	ObjectButton->SetPosition(Vector2f(Indentation, 5.0f));
	ObjectButton->SetSize(Vector2f(PropertyDisplay->GetSize()[0] - Indentation, 20.0f));
	ObjectButton->SetAnchorRight(true);
	ObjectButton->ConnectClickedCallback(std::bind(&ObjectInformationDialog::_OnObjectClicked, this, Object));
	
	return PropertyDisplay->GetSize()[1];
}

float UI::ObjectInformationDialog::_AddSeparator(float Top, float Indentation, const std::string & Separator)
{
	UI::Widget * SeparatorDisplay(new UI::Widget(_PropertiesScrollBox->GetContent()));
	
	SeparatorDisplay->SetPosition(Vector2f(10.0f, Top));
	SeparatorDisplay->SetSize(Vector2f(_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 20.0f));
	SeparatorDisplay->SetAnchorRight(true);
	
	UI::Label * SeparatorLabel(new UI::Label(SeparatorDisplay, Separator));
	
	SeparatorLabel->SetPosition(Vector2f(Indentation, 0.0f));
	SeparatorLabel->SetSize(SeparatorDisplay->GetSize());
	SeparatorLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	SeparatorLabel->SetAnchorBottom(true);
	SeparatorLabel->SetTextColor(Color(0.5f, 0.8f, 1.0f, 1.0f));
	
	return SeparatorDisplay->GetSize()[1];
}

float UI::ObjectInformationDialog::_AddString(float Top, float Indentation, const std::string & String)
{
	UI::Widget * StringDisplay(new UI::Widget(_PropertiesScrollBox->GetContent()));
	
	StringDisplay->SetPosition(Vector2f(10.0f, Top));
	StringDisplay->SetSize(Vector2f(_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 20.0f));
	StringDisplay->SetAnchorRight(true);
	
	UI::Label * StringLabel(new UI::Label(StringDisplay, String));
	
	StringLabel->SetPosition(Vector2f(Indentation, 0.0f));
	StringLabel->SetSize(StringDisplay->GetSize());
	StringLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	StringLabel->SetAnchorBottom(true);
	
	return StringDisplay->GetSize()[1];
}

float UI::ObjectInformationDialog::_AddStringProperty(float Top, float Indentation, const std::string & PropertyName, const std::string & PropertyValue)
{
	UI::Widget * PropertyDisplay(new UI::Widget(_PropertiesScrollBox->GetContent()));
	
	PropertyDisplay->SetPosition(Vector2f(10.0f, Top));
	PropertyDisplay->SetSize(Vector2f(_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 20.0f));
	PropertyDisplay->SetAnchorRight(true);
	
	UI::Label * PropertyNameLabel(new UI::Label(PropertyDisplay, PropertyName + ":"));
	
	PropertyNameLabel->SetPosition(Vector2f(Indentation, 0.0f));
	PropertyNameLabel->SetSize(Vector2f(6.0f * (PropertyName.length() + 1), PropertyDisplay->GetSize()[1]));
	PropertyNameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	PropertyNameLabel->SetAnchorBottom(true);
	
	UI::Label * PropertyValueLabel(new UI::Label(PropertyDisplay, PropertyValue));
	
	PropertyValueLabel->SetPosition(Vector2f(PropertyNameLabel->GetPosition()[0] + PropertyNameLabel->GetSize()[0], 0.0f));
	PropertyValueLabel->SetSize(Vector2f(PropertyDisplay->GetSize()[0] - PropertyNameLabel->GetPosition()[0] - PropertyNameLabel->GetSize()[0], PropertyDisplay->GetSize()[1]));
	PropertyValueLabel->SetHorizontalAlignment(UI::Label::ALIGN_RIGHT);
	PropertyValueLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	PropertyValueLabel->SetAnchorRight(true);
	PropertyValueLabel->SetAnchorBottom(true);
	
	return PropertyDisplay->GetSize()[1];
}

std::string UI::ObjectInformationDialog::_GetPositionString(const Vector3f & Position)
{
	return "x=" + to_string_cast(Position[0]) + "; y=" + to_string_cast(Position[1]) + "; z=" + to_string_cast(Position[2]);
}

std::string UI::ObjectInformationDialog::_GetOrientationString(const Quaternion & Orientation)
{
	return "w=" + to_string_cast(Orientation[0]) + "; x=" + to_string_cast(Orientation[1]) + "; y=" + to_string_cast(Orientation[2]) + "; z=" + to_string_cast(Orientation[3]);
}

void UI::ObjectInformationDialog::_OnCloseClicked(void)
{
	Destroy();
}

void UI::ObjectInformationDialog::_OnObjectClicked(const Reference< Object > Object)
{
	new UI::ObjectInformationDialog(GetRootWidget(), Object);
}

void UI::ObjectInformationDialog::_OnRefreshClicked(void)
{
	_Refresh();
}

void UI::ObjectInformationDialog::_Refresh(void)
{
	// clear old properties
	while(_PropertiesScrollBox->GetContent()->GetSubWidgets().empty() == false)
	{
		_PropertiesScrollBox->GetContent()->GetSubWidgets().front()->Destroy();
	}
	
	float Top(0.0f);
	
	// fill the properties view
	if(_Object.IsValid() == true)
	{
		Top += _AddStringProperty(Top, 0.0f, "Type", _Object->GetTypeIdentifier());
		Top += _AddStringProperty(Top, 0.0f, "Class", _Object->GetClassIdentifier());
		Top += _AddStringProperty(Top, 0.0f, "Identifier", _Object->GetObjectIdentifier());
		Top += _AddSeparator(Top, 0.0f, "Container");
		if(_Object->GetContainer() != 0)
		{
			Top += _AddObjectProperty(Top, 20.0f, _Object->GetContainer()->GetReference());
		}
		else
		{
			Top += _AddString(Top, 20.0f, "No container.");
		}
		if(_Object->GetAspectName() != 0)
		{
			Top += _AddSeparator(Top, 0.0f, "Name Aspect");
			Top += _AddStringProperty(Top, 20.0f, "Name", _Object->GetAspectName()->GetName());
			SetTitle("Object Information: " + _Object->GetAspectName()->GetName());
		}
		else
		{
			SetTitle("Object Information");
		}
		if(_Object->GetAspectObjectContainer() != 0)
		{
			Top += _AddSeparator(Top, 0.0f, "Object Container Aspect");
			
			const std::set< Object * > & Content(_Object->GetAspectObjectContainer()->GetContent());
			
			for(std::set< Object * >::const_iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
			{
				Top += _AddObjectProperty(Top, 20.0f, (*ContentIterator)->GetReference());
			}
		}
		if(_Object->GetAspectPosition() != 0)
		{
			Top += _AddSeparator(Top, 0.0f, "Position");
			Top += _AddStringProperty(Top, 20.0f, "Position", _GetPositionString(_Object->GetAspectPosition()->GetPosition()));
			Top += _AddStringProperty(Top, 20.0f, "Orientation", _GetOrientationString(_Object->GetAspectPosition()->GetOrientation()));
		}
		if(_Object->GetAspectPhysical() != 0)
		{
			Top += _AddSeparator(Top, 0.0f, "Physical");
			Top += _AddStringProperty(Top, 20.0f, "Radial Size", to_string_cast(_Object->GetAspectPhysical()->GetRadialSize()));
			Top += _AddStringProperty(Top, 20.0f, "Space Requirement", to_string_cast(_Object->GetAspectPhysical()->GetSpaceRequirement()));
		}
		if(_Object->GetAspectAccessory() != 0)
		{
			Top += _AddSeparator(Top, 0.0f, "Accessory");
			Top += _AddStringProperty(Top, 20.0f, "Slot Class Identifier", _Object->GetAspectAccessory()->GetSlotClassIdentifier());
			if(_Object->GetAspectAccessory()->GetSlot() != 0)
			{
				Top += _AddString(Top, 20.0f, "Mounted to:");
				Top += _AddStringProperty(Top, 40.0f, "Slot Identifier", _Object->GetAspectAccessory()->GetSlot()->GetIdentifier());
				Top += _AddStringProperty(Top, 40.0f, "Slot Name", _Object->GetAspectAccessory()->GetSlot()->GetName());
				Top += _AddStringProperty(Top, 40.0f, "Slot Class Identifier", _Object->GetAspectAccessory()->GetSlot()->GetSlotClass()->GetIdentifier());
			}
			else
			{
				Top += _AddString(Top, 20.0f, "Not mounted.");
			}
		}
		if(_Object->GetAspectMessages() != 0)
		{
			Top += _AddSeparator(Top, 0.0f, "Messages");
			
			const std::deque< Message * > & Messages(_Object->GetAspectMessages()->GetMessages());
			
			Top += _AddStringProperty(Top, 20.0f, "Number of messages", to_string_cast(Messages.size()));
			for(std::deque< Message * >::const_iterator MessageIterator = Messages.begin(); MessageIterator != Messages.end(); ++MessageIterator)
			{
				Top += _AddString(Top, 40.0f, (*MessageIterator)->GetTypeIdentifier());
			}
		}
		if(_Object->GetAspectOutfitting() != 0)
		{
			Top += _AddSeparator(Top, 0.0f, "Outfitting");
			
			const std::map< std::string, Slot * > & Slots(_Object->GetAspectOutfitting()->GetSlots());
			
			Top += _AddStringProperty(Top, 20.0f, "Number of slots", to_string_cast(Slots.size()));
			for(std::map< std::string, Slot * >::const_iterator SlotIterator = Slots.begin(); SlotIterator != Slots.end(); ++SlotIterator)
			{
				Top += _AddStringProperty(Top, 40.0f, SlotIterator->second->GetName(), SlotIterator->second->GetSlotClass()->GetIdentifier());
			}
		}
		if(_Object->GetAspectUpdate() != 0)
		{
			Top += _AddSeparator(Top, 0.0f, "Update");
		}
		if(_Object->GetAspectVisualization() != 0)
		{
			Top += _AddSeparator(Top, 0.0f, "Visualization");
			Top += _AddStringProperty(Top, 20.0f, "Number of visualizations", to_string_cast(_Object->GetAspectVisualization()->GetVisualizations().size()));
		}
	}
	_PropertiesScrollBox->GetContent()->SetSize(Vector2f(_PropertiesScrollBox->GetView()->GetSize()[0], std::max(Top, _PropertiesScrollBox->GetView()->GetSize()[1])));
	_PropertiesScrollBox->GetContent()->SetAnchorRight(true);
}
