/**
 * galactic-fall
 * Copyright (C) 2008-2018  Hagen Möbius
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
			// create components
			_Button = new UI::TextButton{};
			// initialize components
			_Button->SetLeft(constant(Indentation));
			_Button->SetTop(5.0_c);
			_Button->SetWidth(width(this) - constant(Indentation));
			_Button->SetHeight(height(this) - 2.0_c * 5.0_c);
			_Button->SetText(Object->GetObjectIdentifier());
			_Button->ConnectClickedCallback(std::bind(&UI::ObjectPropertyWidget::_OnButtonClicked, this));
			// add components
			AddSubWidget(_Button);
		}
	private:
		void _OnButtonClicked(void)
		{
			auto ObjectInformationDialog{new UI::ObjectInformationDialog{_Object}};
			
			ObjectInformationDialog->SetName("object_information(" + _Object->GetObjectIdentifier() + ")");
			ObjectInformationDialog->SetLeft(100.0_c);
			ObjectInformationDialog->SetTop(400.0_c);
			ObjectInformationDialog->SetWidth(500.0_c);
			ObjectInformationDialog->SetHeight(300.0_c);
			GetRootWidget()->AddSubWidget(ObjectInformationDialog);
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

UI::ObjectInformationDialog::ObjectInformationDialog(Object * Object) :
	_Object(Object),
	_PropertiesScrollBox(nullptr)
{
	SetTitle("Object Information");
	assert(_Object != nullptr);
	_ObjectDestroyingConnection = _Object->ConnectDestroyingCallback(std::bind(&ObjectInformationDialog::_OnObjectDestroying, this));
	ConnectDestroyingCallback(std::bind(&ObjectInformationDialog::_OnDestroying, this, std::placeholders::_1));
	
	// create components
	auto CloseButton{new UI::TextButton{}};
	auto RefreshButton{new UI::TextButton{}};
	
	_PropertiesScrollBox = new UI::ScrollBox{};
	// initialize components
	CloseButton->SetLeft(width(this) - 10.0_c - width(CloseButton));
	CloseButton->SetTop(height(this) - 10.0_c - height(CloseButton));
	CloseButton->SetWidth(100.0_c);
	CloseButton->SetHeight(20.0_c);
	CloseButton->SetText("Close");
	CloseButton->ConnectClickedCallback(std::bind(&ObjectInformationDialog::_OnCloseClicked, this));
	_PropertiesScrollBox->SetLeft(10.0_c);
	_PropertiesScrollBox->SetTop(40.0_c);
	_PropertiesScrollBox->SetWidth(width(this) - 2.0_c * left(_PropertiesScrollBox));
	_PropertiesScrollBox->SetHeight(top(CloseButton) - 10.0_c - top(_PropertiesScrollBox));
	_PropertiesScrollBox->GetContent()->SetWidth(width(_PropertiesScrollBox->GetView()));
	_PropertiesScrollBox->SetHorizontalScrollBarVisible(false);
	RefreshButton->SetLeft(left(CloseButton) - 10.0_c - width(RefreshButton));
	RefreshButton->SetTop(height(this) - 10.0_c - height(RefreshButton));
	RefreshButton->SetWidth(100.0_c);
	RefreshButton->SetHeight(20.0_c);
	RefreshButton->SetText("Refresh");
	RefreshButton->ConnectClickedCallback(std::bind(&ObjectInformationDialog::_OnRefreshClicked, this));
	// add components
	AddSubWidget(CloseButton);
	AddSubWidget(_PropertiesScrollBox);
	AddSubWidget(RefreshButton);
	_Refresh();
}

UI::Widget * UI::ObjectInformationDialog::_AddObjectProperty(UI::Widget * PreviousWidget, float Indentation, Object * Object)
{
	auto ObjectPropertyWidget{new UI::ObjectPropertyWidget{Indentation, Object}};
	
	ObjectPropertyWidget->SetLeft(10.0_c);
	if(PreviousWidget != nullptr)
	{
		ObjectPropertyWidget->SetTop(bottom(PreviousWidget));
	}
	else
	{
		ObjectPropertyWidget->SetTop(0.0_c);
	}
	ObjectPropertyWidget->SetWidth(width(_PropertiesScrollBox->GetContent()) - 2.0_c * 10.0_c);
	ObjectPropertyWidget->SetHeight(30.0_c);
	_PropertiesScrollBox->GetContent()->AddSubWidget(ObjectPropertyWidget);
	
	return ObjectPropertyWidget;
}

UI::Widget * UI::ObjectInformationDialog::_AddSeparator(UI::Widget * PreviousWidget, float Indentation, const std::string & Separator)
{
	// create components
	auto SeparatorDisplay{new UI::Widget{}};
	auto SeparatorLabel{new UI::Label{}};
	
	// initialize components
	SeparatorDisplay->SetLeft(10.0_c);
	if(PreviousWidget != nullptr)
	{
		SeparatorDisplay->SetTop(bottom(PreviousWidget));
	}
	else
	{
		SeparatorDisplay->SetTop(0.0_c);
	}
	SeparatorDisplay->SetWidth(width(_PropertiesScrollBox->GetContent()) - 2.0_c * 10.0_c);
	SeparatorDisplay->SetHeight(20.0_c);
	SeparatorLabel->SetLeft(constant(Indentation));
	SeparatorLabel->SetTop(0.0_c);
	SeparatorLabel->SetWidth(width(SeparatorDisplay));
	SeparatorLabel->SetHeight(height(SeparatorDisplay));
	SeparatorLabel->SetText(Separator);
	SeparatorLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	SeparatorLabel->SetTextColor(Graphics::ColorRGBO(0.5f, 0.8f, 1.0f, 1.0f));
	// add components
	_PropertiesScrollBox->GetContent()->AddSubWidget(SeparatorDisplay);
	SeparatorDisplay->AddSubWidget(SeparatorLabel);
	
	return SeparatorDisplay;
}

UI::Widget * UI::ObjectInformationDialog::_AddString(UI::Widget * PreviousWidget, float Indentation, const std::string & String)
{
	// create components
	auto StringDisplay{new UI::Widget{}};
	auto StringLabel{new UI::Label{}};
	
	// initialize components
	StringDisplay->SetLeft(10.0_c);
	if(PreviousWidget != nullptr)
	{
		StringDisplay->SetTop(bottom(PreviousWidget));
	}
	else
	{
		StringDisplay->SetTop(0.0_c);
	}
	StringDisplay->SetWidth(width(_PropertiesScrollBox->GetContent()) - 2.0_c * 10.0_c);
	StringDisplay->SetHeight(20.0_c);
	StringLabel->SetLeft(constant(Indentation));
	StringLabel->SetTop(0.0_c);
	StringLabel->SetWidth(width(StringDisplay));
	StringLabel->SetHeight(height(StringDisplay));
	StringLabel->SetText(String);
	StringLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	// add components
	_PropertiesScrollBox->GetContent()->AddSubWidget(StringDisplay);
	StringDisplay->AddSubWidget(StringLabel);
	
	return StringDisplay;
}

UI::Widget * UI::ObjectInformationDialog::_AddStringProperty(UI::Widget * PreviousWidget, float Indentation, const std::string & PropertyName, const std::string & PropertyValue)
{
	// create components
	auto PropertyDisplay{new UI::Widget{}};
	auto PropertyNameLabel{new UI::Label{}};
	auto PropertyValueLabel{new UI::Label{}};
	
	PropertyDisplay->SetLeft(10.0_c);
	if(PreviousWidget != nullptr)
	{
		PropertyDisplay->SetTop(bottom(PreviousWidget));
	}
	else
	{
		PropertyDisplay->SetTop(0.0_c);
	}
	PropertyDisplay->SetWidth(width(_PropertiesScrollBox->GetContent()) - 2.0_c * 10.0_c);
	PropertyDisplay->SetHeight(20.0_c);
	PropertyNameLabel->SetLeft(constant(Indentation));
	PropertyNameLabel->SetTop(0.0_c);
	PropertyNameLabel->SetWidth(constant(6.0f * (PropertyName.length() + 1)));
	PropertyNameLabel->SetHeight(height(PropertyDisplay));
	PropertyNameLabel->SetText(PropertyName + ":");
	PropertyNameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	PropertyValueLabel->SetLeft(constant(PropertyNameLabel->GetRight()));
	PropertyValueLabel->SetTop(0.0_c);
	PropertyValueLabel->SetWidth(width(PropertyDisplay) - right(PropertyNameLabel));
	PropertyValueLabel->SetHeight(height(PropertyDisplay));
	PropertyValueLabel->SetText(PropertyValue);
	PropertyValueLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
	PropertyValueLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	// add components
	_PropertiesScrollBox->GetContent()->AddSubWidget(PropertyDisplay);
	PropertyDisplay->AddSubWidget(PropertyNameLabel);
	PropertyDisplay->AddSubWidget(PropertyValueLabel);
	
	return PropertyDisplay;
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
	
	UI::Widget * PreviousWidget{nullptr};
	
	// fill the properties view
	if(_Object != nullptr)
	{
		PreviousWidget = _AddStringProperty(PreviousWidget, 0.0f, "Type", _Object->GetTypeIdentifier());
		PreviousWidget = _AddStringProperty(PreviousWidget, 0.0f, "Sub-Type", _Object->GetSubTypeIdentifier());
		PreviousWidget = _AddStringProperty(PreviousWidget, 0.0f, "Identifier", _Object->GetObjectIdentifier());
		PreviousWidget = _AddSeparator(PreviousWidget, 0.0f, "Container");
		if(_Object->GetContainer() != nullptr)
		{
			PreviousWidget = _AddObjectProperty(PreviousWidget, 20.0f, _Object->GetContainer());
		}
		else
		{
			PreviousWidget = _AddString(PreviousWidget, 20.0f, "No container.");
		}
		if(_Object->GetAspectName() != nullptr)
		{
			PreviousWidget = _AddSeparator(PreviousWidget, 0.0f, "Name Aspect");
			PreviousWidget = _AddStringProperty(PreviousWidget, 20.0f, "Name", _Object->GetAspectName()->GetName());
			SetTitle("Object Information: " + _Object->GetAspectName()->GetName());
		}
		else
		{
			SetTitle("Object Information");
		}
		if(_Object->GetAspectObjectContainer() != nullptr)
		{
			PreviousWidget = _AddSeparator(PreviousWidget, 0.0f, "Object Container Aspect");
			for(auto Content : _Object->GetAspectObjectContainer()->GetContent())
			{
				PreviousWidget = _AddObjectProperty(PreviousWidget, 20.0f, Content);
			}
		}
		if(_Object->GetAspectPosition() != nullptr)
		{
			PreviousWidget = _AddSeparator(PreviousWidget, 0.0f, "Position");
			PreviousWidget = _AddStringProperty(PreviousWidget, 20.0f, "Position", _GetPositionString(_Object->GetAspectPosition()->GetPosition()));
			PreviousWidget = _AddStringProperty(PreviousWidget, 20.0f, "Orientation", _GetOrientationString(_Object->GetAspectPosition()->GetOrientation()));
		}
		if(_Object->GetAspectPhysical() != nullptr)
		{
			PreviousWidget = _AddSeparator(PreviousWidget, 0.0f, "Physical");
			PreviousWidget = _AddStringProperty(PreviousWidget, 20.0f, "Radial Size", to_string_cast(_Object->GetAspectPhysical()->GetRadialSize()));
			PreviousWidget = _AddStringProperty(PreviousWidget, 20.0f, "Space Requirement", to_string_cast(_Object->GetAspectPhysical()->GetSpaceRequirement()));
		}
		if(_Object->GetAspectAccessory() != nullptr)
		{
			PreviousWidget = _AddSeparator(PreviousWidget, 0.0f, "Accessory");
			PreviousWidget = _AddStringProperty(PreviousWidget, 20.0f, "Slot Class Identifier", _Object->GetAspectAccessory()->GetSlotClassIdentifier());
			if(_Object->GetAspectAccessory()->GetSlot() != nullptr)
			{
				PreviousWidget = _AddString(PreviousWidget, 20.0f, "Mounted to:");
				PreviousWidget = _AddStringProperty(PreviousWidget, 40.0f, "Slot Identifier", _Object->GetAspectAccessory()->GetSlot()->GetIdentifier());
				PreviousWidget = _AddStringProperty(PreviousWidget, 40.0f, "Slot Name", _Object->GetAspectAccessory()->GetSlot()->GetName());
				PreviousWidget = _AddStringProperty(PreviousWidget, 40.0f, "Slot Class Identifier", _Object->GetAspectAccessory()->GetSlot()->GetSlotClass()->GetIdentifier());
			}
			else
			{
				PreviousWidget = _AddString(PreviousWidget, 20.0f, "Not mounted.");
			}
		}
		if(_Object->GetAspectMessages() != nullptr)
		{
			PreviousWidget = _AddSeparator(PreviousWidget, 0.0f, "Messages");
			PreviousWidget = _AddStringProperty(PreviousWidget, 20.0f, "Number of messages", to_string_cast(_Object->GetAspectMessages()->GetMessages().size()));
			for(auto Message : _Object->GetAspectMessages()->GetMessages())
			{
				PreviousWidget = _AddString(PreviousWidget, 40.0f, Message->GetTypeIdentifier());
			}
		}
		if(_Object->GetAspectOutfitting() != nullptr)
		{
			PreviousWidget = _AddSeparator(PreviousWidget, 0.0f, "Outfitting");
			PreviousWidget = _AddStringProperty(PreviousWidget, 20.0f, "Number of slots", to_string_cast(_Object->GetAspectOutfitting()->GetSlots().size()));
			for(auto SlotPair : _Object->GetAspectOutfitting()->GetSlots())
			{
				PreviousWidget = _AddStringProperty(PreviousWidget, 40.0f, SlotPair.second->GetName(), SlotPair.second->GetSlotClass()->GetIdentifier());
			}
		}
		if(_Object->GetAspectUpdate() != nullptr)
		{
			PreviousWidget = _AddSeparator(PreviousWidget, 0.0f, "Update");
		}
		if(_Object->GetAspectVisualization() != nullptr)
		{
			PreviousWidget = _AddSeparator(PreviousWidget, 0.0f, "Visualization");
			PreviousWidget = _AddStringProperty(PreviousWidget, 20.0f, "Number of visualizations", to_string_cast(_Object->GetAspectVisualization()->GetVisualizations().size()));
		}
	}
	_PropertiesScrollBox->GetContent()->SetHeight(bottom(PreviousWidget));
}
