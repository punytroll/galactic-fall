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
	m_PropertyNameLabel->SetPosition(Vector2f(10.0f, 0.0f));
	m_PropertyNameLabel->SetSize(Vector2f(6.0f * (PropertyName.length() + 1), 0.0f));
	m_PropertyNameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_PropertyNameLabel->SetAnchorBottom(true);
	m_PropertyValueLabel = new Label(this, PropertyValue);
	m_PropertyValueLabel->SetPosition(Vector2f(m_PropertyNameLabel->GetPosition()[0] + m_PropertyNameLabel->GetSize()[0], 0.0f));
	m_PropertyValueLabel->SetSize(Vector2f(GetSize()[0] - m_PropertyNameLabel->GetPosition()[0] - m_PropertyNameLabel->GetSize()[0] - 10.0f, 0.0f));
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
	
	PropertyDisplay * TypeDisplay(new PropertyDisplay(this, "Type", m_Object->GetTypeIdentifier()));
	
	TypeDisplay->SetPosition(Vector2f(10.0f, 40.0f));
	TypeDisplay->SetSize(Vector2f(GetSize()[0] - 20.0f, 20.0f));
	TypeDisplay->SetAnchorRight(true);
	
	PropertyDisplay * ClassDisplay(new PropertyDisplay(this, "Class", m_Object->GetClassIdentifier()));
	
	ClassDisplay->SetPosition(Vector2f(10.0f, 60.0f));
	ClassDisplay->SetSize(Vector2f(GetSize()[0] - 20.0f, 20.0f));
	ClassDisplay->SetAnchorRight(true);
	
	PropertyDisplay * IdentifierDisplay(new PropertyDisplay(this, "Identifier", m_Object->GetObjectIdentifier()));
	
	IdentifierDisplay->SetPosition(Vector2f(10.0f, 80.0f));
	IdentifierDisplay->SetSize(Vector2f(GetSize()[0] - 20.0f, 20.0f));
	IdentifierDisplay->SetAnchorRight(true);
}

bool ObjectInformationDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		Destroy();
		
		return true;
	}
	
	return false;
}
