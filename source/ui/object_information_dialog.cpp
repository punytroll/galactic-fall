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

#include "../globals.h"
#include "../graphics/color_rgbo.h"
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
	_CloseButton(nullptr),
	_PropertiesScrollBox(nullptr),
	_RefreshButton(nullptr)
{
	// set up widgets
	_CloseButton = new UI::TextButton{this, "Close"};
	_CloseButton->SetSize(Vector2f(100.0f, 20.0f));
	_CloseButton->SetLeft(GetWidth() - 10.0f - _CloseButton->GetWidth());
	_CloseButton->SetTop(GetHeight() - 10.0f - _CloseButton->GetHeight());
	_CloseButton->SetAnchorBottom(true);
	_CloseButton->SetAnchorLeft(false);
	_CloseButton->SetAnchorRight(true);
	_CloseButton->SetAnchorTop(false);
	_CloseButton->ConnectClickedCallback(std::bind(&ObjectInformationDialog::_OnCloseClicked, this));
	_PropertiesScrollBox = new UI::ScrollBox{this};
	_PropertiesScrollBox->SetLeft(10.0f);
	_PropertiesScrollBox->SetTop(40.0f);
	_PropertiesScrollBox->SetSize(Vector2f(GetWidth() - 20.0f, GetHeight() - 80.0f));
	_PropertiesScrollBox->SetHorizontalScrollBarVisible(false);
	_PropertiesScrollBox->SetAnchorBottom(true);
	_PropertiesScrollBox->SetAnchorRight(true);
	_RefreshButton = new UI::TextButton{this, "Refresh"};
	_RefreshButton->SetSize(Vector2f(100.0f, 20.0f));
	_RefreshButton->SetLeft(_CloseButton->GetLeft() - 10.0f - _RefreshButton->GetWidth());
	_RefreshButton->SetTop(GetHeight() - 10.0f - _RefreshButton->GetHeight());
	_RefreshButton->SetAnchorBottom(true);
	_RefreshButton->SetAnchorLeft(false);
	_RefreshButton->SetAnchorRight(true);
	_RefreshButton->SetAnchorTop(false);
	_RefreshButton->ConnectClickedCallback(std::bind(&ObjectInformationDialog::_OnRefreshClicked, this));
	_Refresh();
}

float UI::ObjectInformationDialog::_AddObjectProperty(float Top, float Indentation, const Reference< Object > & Object)
{
	auto PropertyDisplay{new UI::Widget{_PropertiesScrollBox->GetContent()}};
	
	PropertyDisplay->SetLeft(10.0f);
	PropertyDisplay->SetTop(Top);
	PropertyDisplay->SetSize(Vector2f(_PropertiesScrollBox->GetContent()->GetWidth() - 20.0f, 30.0f));
	PropertyDisplay->SetAnchorRight(true);
	
	auto ObjectButton{new UI::TextButton{PropertyDisplay, Object->GetObjectIdentifier()}};
	
	ObjectButton->SetLeft(Indentation);
	ObjectButton->SetTop(5.0f);
	ObjectButton->SetSize(Vector2f(PropertyDisplay->GetWidth() - Indentation, 20.0f));
	ObjectButton->SetAnchorRight(true);
	ObjectButton->ConnectClickedCallback(std::bind(&ObjectInformationDialog::_OnObjectClicked, this, Object));
	
	return PropertyDisplay->GetHeight();
}

float UI::ObjectInformationDialog::_AddSeparator(float Top, float Indentation, const std::string & Separator)
{
	auto SeparatorDisplay{new UI::Widget{_PropertiesScrollBox->GetContent()}};
	
	SeparatorDisplay->SetLeft(10.0f);
	SeparatorDisplay->SetTop(Top);
	SeparatorDisplay->SetSize(Vector2f(_PropertiesScrollBox->GetContent()->GetWidth() - 20.0f, 20.0f));
	SeparatorDisplay->SetAnchorRight(true);
	
	auto SeparatorLabel{new UI::Label{SeparatorDisplay, Separator}};
	
	SeparatorLabel->SetLeft(Indentation);
	SeparatorLabel->SetTop(0.0f);
	SeparatorLabel->SetSize(SeparatorDisplay->GetSize());
	SeparatorLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	SeparatorLabel->SetAnchorBottom(true);
	SeparatorLabel->SetTextColor(Graphics::ColorRGBO(0.5f, 0.8f, 1.0f, 1.0f));
	
	return SeparatorDisplay->GetHeight();
}

float UI::ObjectInformationDialog::_AddString(float Top, float Indentation, const std::string & String)
{
	auto StringDisplay{new UI::Widget{_PropertiesScrollBox->GetContent()}};
	
	StringDisplay->SetLeft(10.0f);
	StringDisplay->SetTop(Top);
	StringDisplay->SetSize(Vector2f(_PropertiesScrollBox->GetContent()->GetWidth() - 20.0f, 20.0f));
	StringDisplay->SetAnchorRight(true);
	
	auto StringLabel{new UI::Label{StringDisplay, String}};
	
	StringLabel->SetLeft(Indentation);
	StringLabel->SetTop(0.0f);
	StringLabel->SetSize(StringDisplay->GetSize());
	StringLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	StringLabel->SetAnchorBottom(true);
	
	return StringDisplay->GetHeight();
}

float UI::ObjectInformationDialog::_AddStringProperty(float Top, float Indentation, const std::string & PropertyName, const std::string & PropertyValue)
{
	auto PropertyDisplay{new UI::Widget{_PropertiesScrollBox->GetContent()}};
	
	PropertyDisplay->SetLeft(10.0f);
	PropertyDisplay->SetTop(Top);
	PropertyDisplay->SetSize(Vector2f(_PropertiesScrollBox->GetContent()->GetWidth() - 20.0f, 20.0f));
	PropertyDisplay->SetAnchorRight(true);
	
	auto PropertyNameLabel{new UI::Label{PropertyDisplay, PropertyName + ":"}};
	
	PropertyNameLabel->SetLeft(Indentation);
	PropertyNameLabel->SetTop(0.0f);
	PropertyNameLabel->SetSize(Vector2f(6.0f * (PropertyName.length() + 1), PropertyDisplay->GetHeight()));
	PropertyNameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	PropertyNameLabel->SetAnchorBottom(true);
	
	auto PropertyValueLabel{new UI::Label(PropertyDisplay, PropertyValue)};
	
	PropertyValueLabel->SetLeft(PropertyNameLabel->GetLeft() + PropertyNameLabel->GetWidth());
	PropertyValueLabel->SetTop(0.0f);
	PropertyValueLabel->SetSize(Vector2f(PropertyDisplay->GetWidth() - PropertyNameLabel->GetLeft() - PropertyNameLabel->GetWidth(), PropertyDisplay->GetHeight()));
	PropertyValueLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	PropertyValueLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	PropertyValueLabel->SetAnchorRight(true);
	PropertyValueLabel->SetAnchorBottom(true);
	
	return PropertyDisplay->GetHeight();
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
	auto ObjectInformationDialog{new UI::ObjectInformationDialog{GetRootWidget(), Object}};
	
	ObjectInformationDialog->SetName("object_information(" + Object->GetObjectIdentifier() + ")");
	ObjectInformationDialog->SetLeft(100.0f);
	ObjectInformationDialog->SetTop(400.0f);
	ObjectInformationDialog->SetSize(Vector2f(500.0f, 300.0f));
	ObjectInformationDialog->GrabKeyFocus();
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
	
	float Top{0.0f};
	
	// fill the properties view
	if(_Object.IsValid() == true)
	{
		Top += _AddStringProperty(Top, 0.0f, "Type", _Object->GetTypeIdentifier());
		Top += _AddStringProperty(Top, 0.0f, "Class", _Object->GetClassIdentifier());
		Top += _AddStringProperty(Top, 0.0f, "Identifier", _Object->GetObjectIdentifier());
		Top += _AddSeparator(Top, 0.0f, "Container");
		if(_Object->GetContainer() != nullptr)
		{
			Top += _AddObjectProperty(Top, 20.0f, _Object->GetContainer()->GetReference());
		}
		else
		{
			Top += _AddString(Top, 20.0f, "No container.");
		}
		if(_Object->GetAspectName() != nullptr)
		{
			Top += _AddSeparator(Top, 0.0f, "Name Aspect");
			Top += _AddStringProperty(Top, 20.0f, "Name", _Object->GetAspectName()->GetName());
			SetTitle("Object Information: " + _Object->GetAspectName()->GetName());
		}
		else
		{
			SetTitle("Object Information");
		}
		if(_Object->GetAspectObjectContainer() != nullptr)
		{
			Top += _AddSeparator(Top, 0.0f, "Object Container Aspect");
			for(auto Content : _Object->GetAspectObjectContainer()->GetContent())
			{
				Top += _AddObjectProperty(Top, 20.0f, Content->GetReference());
			}
		}
		if(_Object->GetAspectPosition() != nullptr)
		{
			Top += _AddSeparator(Top, 0.0f, "Position");
			Top += _AddStringProperty(Top, 20.0f, "Position", _GetPositionString(_Object->GetAspectPosition()->GetPosition()));
			Top += _AddStringProperty(Top, 20.0f, "Orientation", _GetOrientationString(_Object->GetAspectPosition()->GetOrientation()));
		}
		if(_Object->GetAspectPhysical() != nullptr)
		{
			Top += _AddSeparator(Top, 0.0f, "Physical");
			Top += _AddStringProperty(Top, 20.0f, "Radial Size", to_string_cast(_Object->GetAspectPhysical()->GetRadialSize()));
			Top += _AddStringProperty(Top, 20.0f, "Space Requirement", to_string_cast(_Object->GetAspectPhysical()->GetSpaceRequirement()));
		}
		if(_Object->GetAspectAccessory() != nullptr)
		{
			Top += _AddSeparator(Top, 0.0f, "Accessory");
			Top += _AddStringProperty(Top, 20.0f, "Slot Class Identifier", _Object->GetAspectAccessory()->GetSlotClassIdentifier());
			if(_Object->GetAspectAccessory()->GetSlot() != nullptr)
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
		if(_Object->GetAspectMessages() != nullptr)
		{
			Top += _AddSeparator(Top, 0.0f, "Messages");
			Top += _AddStringProperty(Top, 20.0f, "Number of messages", to_string_cast(_Object->GetAspectMessages()->GetMessages().size()));
			for(auto Message : _Object->GetAspectMessages()->GetMessages())
			{
				Top += _AddString(Top, 40.0f, Message->GetTypeIdentifier());
			}
		}
		if(_Object->GetAspectOutfitting() != nullptr)
		{
			Top += _AddSeparator(Top, 0.0f, "Outfitting");
			Top += _AddStringProperty(Top, 20.0f, "Number of slots", to_string_cast(_Object->GetAspectOutfitting()->GetSlots().size()));
			for(auto SlotPair : _Object->GetAspectOutfitting()->GetSlots())
			{
				Top += _AddStringProperty(Top, 40.0f, SlotPair.second->GetName(), SlotPair.second->GetSlotClass()->GetIdentifier());
			}
		}
		if(_Object->GetAspectUpdate() != nullptr)
		{
			Top += _AddSeparator(Top, 0.0f, "Update");
		}
		if(_Object->GetAspectVisualization() != nullptr)
		{
			Top += _AddSeparator(Top, 0.0f, "Visualization");
			Top += _AddStringProperty(Top, 20.0f, "Number of visualizations", to_string_cast(_Object->GetAspectVisualization()->GetVisualizations().size()));
		}
	}
	_PropertiesScrollBox->GetContent()->SetSize(Vector2f(_PropertiesScrollBox->GetView()->GetSize()[0], std::max(Top, _PropertiesScrollBox->GetView()->GetSize()[1])));
	_PropertiesScrollBox->GetContent()->SetAnchorRight(true);
}
