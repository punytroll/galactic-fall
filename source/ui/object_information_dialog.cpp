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

#include <expressions/operators.h>

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

using namespace Expressions::Operators;

namespace UI
{
	class ObjectPropertyWidget : public UI::Widget
	{
	public:
		ObjectPropertyWidget(float Indentation, Object * Object) :
			_Object(Object)
		{
			assert(_Object != nullptr);
			_ObjectDestroyingConnection = _Object->ConnectDestroyingCallback(std::bind(&UI::ObjectPropertyWidget::_OnObjectDestroying, this));
			ConnectDestroyingCallback(std::bind(&UI::ObjectPropertyWidget::_OnDestroying, this, std::placeholders::_1));
			_Button = new UI::TextButton{this, Object->GetObjectIdentifier()};
			_Button->SetLeft(constant(Indentation));
			_Button->SetTop(5.0_c);
			_Button->SetWidth(constant(GetWidth() - Indentation));
			_Button->SetHeight(constant(GetHeight() - 10.0f));
			_Button->SetAnchorBottom(true);
			_Button->SetAnchorRight(true);
			_Button->ConnectClickedCallback(std::bind(&UI::ObjectPropertyWidget::_OnButtonClicked, this));
		}
	private:
		void _OnButtonClicked(void)
		{
			auto ObjectInformationDialog{new UI::ObjectInformationDialog{GetRootWidget(), _Object}};
			
			ObjectInformationDialog->SetName("object_information(" + _Object->GetObjectIdentifier() + ")");
			ObjectInformationDialog->SetLeft(100.0_c);
			ObjectInformationDialog->SetTop(400.0_c);
			ObjectInformationDialog->SetWidth(500.0_c);
			ObjectInformationDialog->SetHeight(300.0_c);
			ObjectInformationDialog->GrabKeyFocus();
		}
		
		void _OnDestroying(UI::Event & DestroyingEvent)
		{
			if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
			{
				if(_Object != nullptr)
				{
					_Object = nullptr;
					assert(_ObjectDestroyingConnection.IsValid() == true);
					_ObjectDestroyingConnection.Disconnect();
					assert(_ObjectDestroyingConnection.IsValid() == false);
				}
			}
		}
		
		void _OnObjectDestroying(void)
		{
			assert(_Object != nullptr);
			_Object = nullptr;
			assert(_ObjectDestroyingConnection.IsValid() == true);
			_ObjectDestroyingConnection.Disconnect();
			assert(_ObjectDestroyingConnection.IsValid() == false);
			_Button->SetText("<destroyed>");
			_Button->SetEnabled(false);
		}
		
		UI::TextButton * _Button;
		Object * _Object;
		Connection _ObjectDestroyingConnection;
	};
}

UI::ObjectInformationDialog::ObjectInformationDialog(UI::Widget * SupWidget, Object * Object) :
	UI::Window(SupWidget, "Object Information"),
	_Object(Object),
	_PropertiesScrollBox(nullptr)
{
	assert(_Object != nullptr);
	_ObjectDestroyingConnection = _Object->ConnectDestroyingCallback(std::bind(&ObjectInformationDialog::_OnObjectDestroying, this));
	ConnectDestroyingCallback(std::bind(&ObjectInformationDialog::_OnDestroying, this, std::placeholders::_1));
	
	// set up widgets
	auto CloseButton{new UI::TextButton{this, "Close"}};
	
	CloseButton->SetLeft(constant(GetWidth() - 110.0f));
	CloseButton->SetTop(constant(GetHeight() - 30.0f));
	CloseButton->SetWidth(100.0_c);
	CloseButton->SetHeight(20.0_c);
	CloseButton->SetAnchorBottom(true);
	CloseButton->SetAnchorLeft(false);
	CloseButton->SetAnchorRight(true);
	CloseButton->SetAnchorTop(false);
	CloseButton->ConnectClickedCallback(std::bind(&ObjectInformationDialog::_OnCloseClicked, this));
	_PropertiesScrollBox = new UI::ScrollBox{this};
	_PropertiesScrollBox->SetLeft(10.0_c);
	_PropertiesScrollBox->SetTop(40.0_c);
	_PropertiesScrollBox->SetWidth(constant(GetWidth() - 20.0f));
	_PropertiesScrollBox->SetHeight(constant(GetHeight() - 80.0f));
	_PropertiesScrollBox->SetHorizontalScrollBarVisible(false);
	_PropertiesScrollBox->SetAnchorBottom(true);
	_PropertiesScrollBox->SetAnchorRight(true);
	_PropertiesScrollBox->GetContent()->SetWidth(constant(_PropertiesScrollBox->GetView()->GetWidth()));
	_PropertiesScrollBox->GetContent()->SetAnchorRight(true);
	
	auto RefreshButton{new UI::TextButton{this, "Refresh"}};
	
	RefreshButton->SetLeft(constant(GetWidth() - 220.0f));
	RefreshButton->SetTop(constant(GetHeight() - 30.0f));
	RefreshButton->SetWidth(100.0_c);
	RefreshButton->SetHeight(20.0_c);
	RefreshButton->SetAnchorBottom(true);
	RefreshButton->SetAnchorLeft(false);
	RefreshButton->SetAnchorRight(true);
	RefreshButton->SetAnchorTop(false);
	RefreshButton->ConnectClickedCallback(std::bind(&ObjectInformationDialog::_OnRefreshClicked, this));
	_Refresh();
}

float UI::ObjectInformationDialog::_AddObjectProperty(float Top, float Indentation, Object * Object)
{
	auto ObjectPropertyWidget{new UI::ObjectPropertyWidget{Indentation, Object}};
	
	ObjectPropertyWidget->SetLeft(10.0_c);
	ObjectPropertyWidget->SetTop(constant(Top));
	ObjectPropertyWidget->SetWidth(constant(_PropertiesScrollBox->GetContent()->GetWidth() - 20.0f));
	ObjectPropertyWidget->SetHeight(30.0_c);
	ObjectPropertyWidget->SetAnchorRight(true);
	_PropertiesScrollBox->GetContent()->AddSubWidget(ObjectPropertyWidget);
	
	return ObjectPropertyWidget->GetHeight();
}

float UI::ObjectInformationDialog::_AddSeparator(float Top, float Indentation, const std::string & Separator)
{
	auto SeparatorDisplay{new UI::Widget{_PropertiesScrollBox->GetContent()}};
	
	SeparatorDisplay->SetLeft(10.0_c);
	SeparatorDisplay->SetTop(constant(Top));
	SeparatorDisplay->SetWidth(constant(_PropertiesScrollBox->GetContent()->GetWidth() - 20.0f));
	SeparatorDisplay->SetHeight(20.0_c);
	SeparatorDisplay->SetAnchorRight(true);
	
	auto SeparatorLabel{new UI::Label{SeparatorDisplay, Separator}};
	
	SeparatorLabel->SetLeft(constant(Indentation));
	SeparatorLabel->SetTop(0.0_c);
	SeparatorLabel->SetWidth(constant(SeparatorDisplay->GetWidth()));
	SeparatorLabel->SetHeight(constant(SeparatorDisplay->GetHeight()));
	SeparatorLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	SeparatorLabel->SetAnchorBottom(true);
	SeparatorLabel->SetAnchorRight(true);
	SeparatorLabel->SetTextColor(Graphics::ColorRGBO(0.5f, 0.8f, 1.0f, 1.0f));
	
	return SeparatorDisplay->GetHeight();
}

float UI::ObjectInformationDialog::_AddString(float Top, float Indentation, const std::string & String)
{
	auto StringDisplay{new UI::Widget{_PropertiesScrollBox->GetContent()}};
	
	StringDisplay->SetLeft(10.0_c);
	StringDisplay->SetTop(constant(Top));
	StringDisplay->SetWidth(constant(_PropertiesScrollBox->GetContent()->GetWidth() - 20.0f));
	StringDisplay->SetHeight(20.0_c);
	StringDisplay->SetAnchorRight(true);
	
	auto StringLabel{new UI::Label{StringDisplay, String}};
	
	StringLabel->SetLeft(constant(Indentation));
	StringLabel->SetTop(0.0_c);
	StringLabel->SetWidth(constant(StringDisplay->GetWidth()));
	StringLabel->SetHeight(constant(StringDisplay->GetHeight()));
	StringLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	StringLabel->SetAnchorBottom(true);
	
	return StringDisplay->GetHeight();
}

float UI::ObjectInformationDialog::_AddStringProperty(float Top, float Indentation, const std::string & PropertyName, const std::string & PropertyValue)
{
	auto PropertyDisplay{new UI::Widget{_PropertiesScrollBox->GetContent()}};
	
	PropertyDisplay->SetLeft(10.0_c);
	PropertyDisplay->SetTop(constant(Top));
	PropertyDisplay->SetWidth(constant(_PropertiesScrollBox->GetContent()->GetWidth() - 20.0f));
	PropertyDisplay->SetHeight(20.0_c);
	PropertyDisplay->SetAnchorRight(true);
	
	auto PropertyNameLabel{new UI::Label{PropertyDisplay, PropertyName + ":"}};
	
	PropertyNameLabel->SetLeft(constant(Indentation));
	PropertyNameLabel->SetTop(0.0_c);
	PropertyNameLabel->SetWidth(constant(6.0f * (PropertyName.length() + 1)));
	PropertyNameLabel->SetHeight(constant(PropertyDisplay->GetHeight()));
	PropertyNameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	PropertyNameLabel->SetAnchorBottom(true);
	
	auto PropertyValueLabel{new UI::Label(PropertyDisplay, PropertyValue)};
	
	PropertyValueLabel->SetLeft(constant(PropertyNameLabel->GetRight()));
	PropertyValueLabel->SetTop(0.0_c);
	PropertyValueLabel->SetWidth(constant(PropertyDisplay->GetWidth() - PropertyNameLabel->GetRight()));
	PropertyValueLabel->SetHeight(constant(PropertyDisplay->GetHeight()));
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

void UI::ObjectInformationDialog::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if(_Object != nullptr)
		{
			_Object = nullptr;
			assert(_ObjectDestroyingConnection.IsValid() == true);
			_ObjectDestroyingConnection.Disconnect();
			assert(_ObjectDestroyingConnection.IsValid() == false);
		}
	}
}

void UI::ObjectInformationDialog::_OnObjectDestroying(void)
{
	assert(_Object != nullptr);
	_Object = nullptr;
	assert(_ObjectDestroyingConnection.IsValid() == true);
	_ObjectDestroyingConnection.Disconnect();
	assert(_ObjectDestroyingConnection.IsValid() == false);
	_Refresh();
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
	
	auto Top{0.0f};
	
	// fill the properties view
	if(_Object != nullptr)
	{
		Top += _AddStringProperty(Top, 0.0f, "Type", _Object->GetTypeIdentifier());
		Top += _AddStringProperty(Top, 0.0f, "Class", _Object->GetClassIdentifier());
		Top += _AddStringProperty(Top, 0.0f, "Identifier", _Object->GetObjectIdentifier());
		Top += _AddSeparator(Top, 0.0f, "Container");
		if(_Object->GetContainer() != nullptr)
		{
			Top += _AddObjectProperty(Top, 20.0f, _Object->GetContainer());
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
				Top += _AddObjectProperty(Top, 20.0f, Content);
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
	_PropertiesScrollBox->GetContent()->SetHeight(constant(Top));
}
