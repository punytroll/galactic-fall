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
#include "label.h"
#include "object.h"
#include "object_information_dialog.h"
#include "scroll_box.h"

class PropertyDisplay : public Widget
{
public:
	PropertyDisplay(Widget * SupWidget, const std::string & PropertyName, const std::string & PropertyValue);
private:
	Label * m_PropertyNameLabel;
	Label * m_PropertyValueLabel;
};

PropertyDisplay::PropertyDisplay(Widget * SupWidget, const std::string & PropertyName, const std::string & PropertyValue) :
	Widget(SupWidget),
	m_PropertyNameLabel(0),
	m_PropertyValueLabel(0)
{
	m_PropertyNameLabel = new Label(this, PropertyName + ":");
	m_PropertyNameLabel->SetPosition(Vector2f(0.0f, 0.0f));
	m_PropertyNameLabel->SetSize(Vector2f(6.0f * (PropertyName.length() + 1), 0.0f));
	m_PropertyNameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_PropertyNameLabel->SetAnchorBottom(true);
	m_PropertyValueLabel = new Label(this, PropertyValue);
	m_PropertyValueLabel->SetPosition(Vector2f(m_PropertyNameLabel->GetPosition()[0] + m_PropertyNameLabel->GetSize()[0], 0.0f));
	m_PropertyValueLabel->SetSize(Vector2f(GetSize()[0] - m_PropertyNameLabel->GetPosition()[0] - m_PropertyNameLabel->GetSize()[0], 0.0f));
	m_PropertyValueLabel->SetHorizontalAlignment(Label::ALIGN_RIGHT);
	m_PropertyValueLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_PropertyValueLabel->SetAnchorRight(true);
	m_PropertyValueLabel->SetAnchorBottom(true);
}

ObjectInformationDialog::ObjectInformationDialog(Widget * SupWidget, const Reference< Object > & Object) :
	WWindow(SupWidget, "Object Information"),
	m_Object(Object),
	m_OKButton(0)
{
	SetPosition(Vector2f(100.0f, 400.0f));
	SetSize(Vector2f(500.0f, 300.0f));
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

void ObjectInformationDialog::Refresh(void)
{
	// clear old properties
	while(m_PropertiesScrollBox->GetContent()->GetSubWidgets().empty() == false)
	{
		m_PropertiesScrollBox->GetContent()->GetSubWidgets().front()->Destroy();
	}
	// fill the properties view
	if(m_Object.IsValid() == true)
	{
		PropertyDisplay * TypeDisplay(new PropertyDisplay(m_PropertiesScrollBox->GetContent(), "Type", m_Object->GetTypeIdentifier()));
		
		TypeDisplay->SetPosition(Vector2f(10.0f, 0.0f));
		TypeDisplay->SetSize(Vector2f(m_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 20.0f));
		TypeDisplay->SetAnchorRight(true);
		
		PropertyDisplay * ClassDisplay(new PropertyDisplay(m_PropertiesScrollBox->GetContent(), "Class", m_Object->GetClassIdentifier()));
		
		ClassDisplay->SetPosition(Vector2f(10.0f, 20.0f));
		ClassDisplay->SetSize(Vector2f(m_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 20.0f));
		ClassDisplay->SetAnchorRight(true);
		
		PropertyDisplay * IdentifierDisplay(new PropertyDisplay(m_PropertiesScrollBox->GetContent(), "Identifier", m_Object->GetObjectIdentifier()));
		
		IdentifierDisplay->SetPosition(Vector2f(10.0f, 40.0f));
		IdentifierDisplay->SetSize(Vector2f(m_PropertiesScrollBox->GetContent()->GetSize()[0] - 20.0f, 20.0f));
		IdentifierDisplay->SetAnchorRight(true);
	}
	m_PropertiesScrollBox->GetContent()->SetSize(Vector2f(m_PropertiesScrollBox->GetView()->GetSize()[0], std::max(60.0f, m_PropertiesScrollBox->GetView()->GetSize()[1])));
	m_PropertiesScrollBox->GetContent()->SetAnchorRight(true);
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
	
	return false;
}
