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

#include "button.h"
#include "globals.h"
#include "label.h"
#include "object.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
#include "object_information_dialog.h"
#include "scroll_bar.h"
#include "scroll_box.h"
#include "string_cast.h"

ObjectInformationDialog::ObjectInformationDialog(Widget * SupWidget, const Reference< Object > & Object) :
	WWindow(SupWidget, "Object Information"),
	m_Object(Object),
	m_OKButton(0)
{
	SetPosition(Vector2f(100.0f, 400.0f));
	SetSize(Vector2f(500.0f, 300.0f));
	AddMouseButtonListener(this);
	// set up widgets
	m_OKButton = new Button(this);
	m_OKButton->SetSize(Vector2f(100.0f, 20.0f));
	m_OKButton->SetPosition(Vector2f(GetSize()[0] - 10.0f - m_OKButton->GetSize()[0], GetSize()[1] - 10.0f - m_OKButton->GetSize()[1]));
	m_OKButton->SetAnchorBottom(true);
	m_OKButton->SetAnchorLeft(false);
	m_OKButton->SetAnchorRight(true);
	m_OKButton->SetAnchorTop(false);
	m_OKButton->AddClickedListener(this);
	
	Label * OKButtonLabel = new Label(m_OKButton, "OK");
	
	OKButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	OKButtonLabel->SetSize(m_OKButton->GetSize());
	OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_PropertiesScrollBox = new ScrollBox(this);
	m_PropertiesScrollBox->SetPosition(Vector2f(10.0f, 40.0f));
	m_PropertiesScrollBox->SetSize(Vector2f(GetSize()[0] - 20.0f, GetSize()[1] - 80.0f));
	m_PropertiesScrollBox->SetHorizontalScrollBarVisible(false);
	m_PropertiesScrollBox->SetAnchorBottom(true);
	m_PropertiesScrollBox->SetAnchorRight(true);
	m_RefreshButton = new Button(this);
	m_RefreshButton->SetSize(Vector2f(100.0f, 20.0f));
	m_RefreshButton->SetPosition(Vector2f(m_OKButton->GetPosition()[0] - 10.0f - m_RefreshButton->GetSize()[0], GetSize()[1] - 10.0f - m_RefreshButton->GetSize()[1]));
	m_RefreshButton->SetAnchorBottom(true);
	m_RefreshButton->SetAnchorLeft(false);
	m_RefreshButton->SetAnchorRight(true);
	m_RefreshButton->SetAnchorTop(false);
	m_RefreshButton->AddClickedListener(this);
	
	Label * RefreshButtonLabel = new Label(m_RefreshButton, "Refresh");
	
	RefreshButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	RefreshButtonLabel->SetSize(m_RefreshButton->GetSize());
	RefreshButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	RefreshButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	Refresh();
}

float ObjectInformationDialog::AddObjectProperty(float Top, float Indentation, const Reference< Object > & Object)
{
	Widget * PropertyDisplay(new Widget(m_PropertiesScrollBox->GetContent()));
	
	PropertyDisplay->SetPosition(Vector2f(10.0f, Top));
	PropertyDisplay->SetSize(Vector2f(m_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 30.0f));
	PropertyDisplay->SetAnchorRight(true);
	
	Button * ObjectButton(new Button(PropertyDisplay));
	
	ObjectButton->SetPosition(Vector2f(Indentation, 5.0f));
	ObjectButton->SetSize(Vector2f(PropertyDisplay->GetSize()[0] - Indentation, 20.0f));
	ObjectButton->SetAnchorRight(true);
	ObjectButton->AddClickedListener(this);
	
	Label * ObjectButtonLabel(new Label(ObjectButton, Object->GetObjectIdentifier()));
	
	ObjectButtonLabel->SetPosition(Vector2f(Indentation, 0.0f));
	ObjectButtonLabel->SetSize(ObjectButton->GetSize());
	ObjectButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	ObjectButtonLabel->SetAnchorBottom(true);
	
	return PropertyDisplay->GetSize()[1];
}

float ObjectInformationDialog::AddSeparator(float Top, float Indentation, const std::string & Separator)
{
	Widget * SeparatorDisplay(new Widget(m_PropertiesScrollBox->GetContent()));
	
	SeparatorDisplay->SetPosition(Vector2f(10.0f, Top));
	SeparatorDisplay->SetSize(Vector2f(m_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 20.0f));
	SeparatorDisplay->SetAnchorRight(true);
	
	Label * SeparatorLabel(new Label(SeparatorDisplay, Separator));
	
	SeparatorLabel->SetPosition(Vector2f(Indentation, 0.0f));
	SeparatorLabel->SetSize(SeparatorDisplay->GetSize());
	SeparatorLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	SeparatorLabel->SetAnchorBottom(true);
	SeparatorLabel->SetTextColor(Color(0.5f, 0.8f, 1.0f, 1.0f));
	
	return SeparatorDisplay->GetSize()[1];
}

float ObjectInformationDialog::AddString(float Top, float Indentation, const std::string & String)
{
	Widget * StringDisplay(new Widget(m_PropertiesScrollBox->GetContent()));
	
	StringDisplay->SetPosition(Vector2f(10.0f, Top));
	StringDisplay->SetSize(Vector2f(m_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 20.0f));
	StringDisplay->SetAnchorRight(true);
	
	Label * StringLabel(new Label(StringDisplay, String));
	
	StringLabel->SetPosition(Vector2f(Indentation, 0.0f));
	StringLabel->SetSize(StringDisplay->GetSize());
	StringLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	StringLabel->SetAnchorBottom(true);
	
	return StringDisplay->GetSize()[1];
}

float ObjectInformationDialog::AddStringProperty(float Top, float Indentation, const std::string & PropertyName, const std::string & PropertyValue)
{
	Widget * PropertyDisplay(new Widget(m_PropertiesScrollBox->GetContent()));
	
	PropertyDisplay->SetPosition(Vector2f(10.0f, Top));
	PropertyDisplay->SetSize(Vector2f(m_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 20.0f));
	PropertyDisplay->SetAnchorRight(true);
	
	Label * PropertyNameLabel(new Label(PropertyDisplay, PropertyName + ":"));
	
	PropertyNameLabel->SetPosition(Vector2f(Indentation, 0.0f));
	PropertyNameLabel->SetSize(Vector2f(6.0f * (PropertyName.length() + 1), PropertyDisplay->GetSize()[1]));
	PropertyNameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	PropertyNameLabel->SetAnchorBottom(true);
	
	Label * PropertyValueLabel(new Label(PropertyDisplay, PropertyValue));
	
	PropertyValueLabel->SetPosition(Vector2f(PropertyNameLabel->GetPosition()[0] + PropertyNameLabel->GetSize()[0], 0.0f));
	PropertyValueLabel->SetSize(Vector2f(PropertyDisplay->GetSize()[0] - PropertyNameLabel->GetPosition()[0] - PropertyNameLabel->GetSize()[0], PropertyDisplay->GetSize()[1]));
	PropertyValueLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	PropertyValueLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	PropertyValueLabel->SetAnchorRight(true);
	PropertyValueLabel->SetAnchorBottom(true);
	
	return PropertyDisplay->GetSize()[1];
}

void ObjectInformationDialog::Refresh(void)
{
	// clear old properties
	while(m_PropertiesScrollBox->GetContent()->GetSubWidgets().empty() == false)
	{
		m_PropertiesScrollBox->GetContent()->GetSubWidgets().front()->Destroy();
	}
	
	float Top(0.0f);
	
	// fill the properties view
	if(m_Object.IsValid() == true)
	{
		Top += AddStringProperty(Top, 0.0f, "Type", m_Object->GetTypeIdentifier());
		Top += AddStringProperty(Top, 0.0f, "Class", m_Object->GetClassIdentifier());
		Top += AddStringProperty(Top, 0.0f, "Identifier", m_Object->GetObjectIdentifier());
		Top += AddSeparator(Top, 0.0f, "Container");
		if(m_Object->GetContainer() != 0)
		{
			Top += AddObjectProperty(Top, 20.0f, m_Object->GetContainer()->GetReference());
		}
		else
		{
			Top += AddString(Top, 20.0f, "No container.");
		}
		if(m_Object->GetAspectName() != 0)
		{
			Top += AddSeparator(Top, 0.0f, "Name Aspect");
			Top += AddStringProperty(Top, 20.0f, "Name", m_Object->GetAspectName()->GetName());
		}
		if(m_Object->GetAspectObjectContainer() != 0)
		{
			Top += AddSeparator(Top, 0.0f, "Object Container Aspect");
			
			const std::set< Object * > & Content(m_Object->GetAspectObjectContainer()->GetContent());
			
			for(std::set< Object * >::const_iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
			{
				Top += AddObjectProperty(Top, 20.0f, (*ContentIterator)->GetReference());
			}
		}
		if(m_Object->GetAspectPhysical() != 0)
		{
			Top += AddSeparator(Top, 0.0f, "Physical");
			Top += AddStringProperty(Top, 20.0f, "Radial Size", to_string_cast(m_Object->GetAspectPhysical()->GetRadialSize()));
			Top += AddStringProperty(Top, 20.0f, "Space Requirement", to_string_cast(m_Object->GetAspectPhysical()->GetSpaceRequirement()));
		}
	}
	m_PropertiesScrollBox->GetContent()->SetSize(Vector2f(m_PropertiesScrollBox->GetView()->GetSize()[0], std::max(Top, m_PropertiesScrollBox->GetView()->GetSize()[1])));
	m_PropertiesScrollBox->GetContent()->SetAnchorRight(true);
}

bool ObjectInformationDialog::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if(WWindow::OnMouseButton(EventSource, Button, State, X, Y) == true)
	{
		return true;
	}
	if((Button == 4 /* WHEEL_UP */) && (State == EV_DOWN))
	{
		m_PropertiesScrollBox->GetVerticalScrollBar()->StepLess();
		
		return true;
	}
	else if((Button == 5 /* WHEEL_DOWN */) && (State == EV_DOWN))
	{
		m_PropertiesScrollBox->GetVerticalScrollBar()->StepMore();
		
		return true;
	}
	
	return false;
}

bool ObjectInformationDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		Destroy();
		
		return true;
	}
	else if(EventSource == m_RefreshButton)
	{
		Refresh();
		
		return true;
	}
	else
	{
		assert(EventSource->GetSubWidgets().size() == 1);
		assert(dynamic_cast< Label * >(EventSource->GetSubWidgets().front()) != 0);
		
		Object * Object(Object::GetObject(dynamic_cast< Label * >(EventSource->GetSubWidgets().front())->GetText()));
		
		if(Object != 0)
		{
			new ObjectInformationDialog(GetRootWidget(), Object->GetReference());
		}
	}
	
	return false;
}
