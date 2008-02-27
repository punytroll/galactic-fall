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

#include "button.h"
#include "color.h"
#include "globals.h"
#include "key_event_information.h"
#include "label.h"
#include "object_aspect_name.h"
#include "outfit_ship_dialog.h"
#include "scroll_box.h"
#include "ship.h"
#include "slot.h"
#include "slot_class.h"
#include "slot_class_manager.h"
#include "weapon.h"
#include "weapon_class.h"

class SlotListItem : public MouseMotionListener, public Widget
{
public:
	SlotListItem(Widget * SupWidget, Slot * Slot);
	void Update(void);
	// getters
	bool GetSelected(void) const;
	Slot * GetSlot(void);
	// setters
	void SetSelected(bool Selected);
protected:
	virtual void OnMouseEnter(Widget * EventSource);
	virtual void OnMouseLeave(Widget * EventSource);
private:
	bool m_Selected;
	Slot * m_Slot;
	Label * m_TypeOrWeaponLabel;
};

class EquipmentListItem : public MouseMotionListener, public Widget
{
public:
	EquipmentListItem(Widget * SupWidget, Weapon * Weapon);
	void Update(void);
	// getters
	bool GetSelected(void) const;
	Weapon * GetEquipment(void);
	// setters
	void SetSelected(bool Selected);
protected:
	virtual void OnMouseEnter(Widget * EventSource);
	virtual void OnMouseLeave(Widget * EventSource);
private:
	bool m_Selected;
	Weapon * m_Equipment;
};

SlotListItem::SlotListItem(Widget * SupWidget, Slot * Slot) :
	Widget(SupWidget),
	m_Selected(false),
	m_Slot(Slot)
{
	AddMouseMotionListener(this);
	// set to arbitrary design size
	SetSize(Vector2f(100.0f, 100.0f));
	
	Label * IdentifierLabel(new Label(this, Slot->GetName()));
	
	IdentifierLabel->SetPosition(Vector2f(5.0f, 5.0f));
	IdentifierLabel->SetSize(Vector2f(90.0f, 20.0f));
	IdentifierLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	IdentifierLabel->SetAnchorLeft(true);
	IdentifierLabel->SetAnchorRight(true);
	IdentifierLabel->SetAnchorTop(true);
	m_TypeOrWeaponLabel = new Label(this);
	m_TypeOrWeaponLabel->SetPosition(Vector2f(25.0f, 25.0f));
	m_TypeOrWeaponLabel->SetSize(Vector2f(70.0f, 20.0f));
	m_TypeOrWeaponLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_TypeOrWeaponLabel->SetAnchorLeft(true);
	m_TypeOrWeaponLabel->SetAnchorRight(true);
	m_TypeOrWeaponLabel->SetAnchorTop(true);
	Update();
}

void SlotListItem::Update(void)
{
	PhysicalObject * MountedObject(dynamic_cast< PhysicalObject * >(m_Slot->GetMountedObject().Get()));
	
	if(MountedObject != 0)
	{
		m_TypeOrWeaponLabel->SetForegroundColor(Color(0.6f, 0.8f, 0.6f, 1.0f));
		m_TypeOrWeaponLabel->SetString(MountedObject->GetAspectName()->GetName());
	}
	else
	{
		m_TypeOrWeaponLabel->SetForegroundColor(Color(0.8f, 0.6f, 0.6f, 1.0f));
		m_TypeOrWeaponLabel->SetString(m_Slot->GetSlotClass()->GetName());
	}
}

bool SlotListItem::GetSelected(void) const
{
	return m_Selected;
}

Slot * SlotListItem::GetSlot(void)
{
	return m_Slot;
}

void SlotListItem::SetSelected(bool Selected)
{
	m_Selected = Selected;
	if(m_Selected == false)
	{
		UnsetBackgroundColor();
	}
	else
	{
		SetBackgroundColor(Color(0.4f, 0.1f, 0.1f, 1.0f));
	}
}

void SlotListItem::OnMouseEnter(Widget * EventSource)
{
	if(GetSelected() == false)
	{
		SetBackgroundColor(Color(0.3f, 0.2f, 0.2f, 1.0f));
	}
}

void SlotListItem::OnMouseLeave(Widget * EventSource)
{
	if(GetSelected() == false)
	{
		UnsetBackgroundColor();
	}
}

EquipmentListItem::EquipmentListItem(Widget * SupWidget, Weapon * Equipment) :
	Widget(SupWidget),
	m_Selected(false),
	m_Equipment(Equipment)
{
	AddMouseMotionListener(this);
	// set to arbitrary design size
	SetSize(Vector2f(100.0f, 100.0f));
	
	Label * NameLabel(new Label(this, Equipment->GetAspectName()->GetName()));
	
	NameLabel->SetPosition(Vector2f(5.0f, 5.0f));
	NameLabel->SetSize(Vector2f(90.0f, 20.0f));
	NameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	NameLabel->SetAnchorLeft(true);
	NameLabel->SetAnchorRight(true);
	NameLabel->SetAnchorTop(true);
	
	Label * SlotTypeLabel(new Label(this, g_SlotClassManager->Get(Equipment->GetWeaponClass()->GetSlotClassIdentifier())->GetName()));
	
	SlotTypeLabel->SetPosition(Vector2f(25.0f, 25.0f));
	SlotTypeLabel->SetSize(Vector2f(70.0f, 20.0f));
	SlotTypeLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	SlotTypeLabel->SetAnchorLeft(true);
	SlotTypeLabel->SetAnchorRight(true);
	SlotTypeLabel->SetAnchorTop(true);
}

bool EquipmentListItem::GetSelected(void) const
{
	return m_Selected;
}

Weapon * EquipmentListItem::GetEquipment(void)
{
	return m_Equipment;
}

void EquipmentListItem::SetSelected(bool Selected)
{
	m_Selected = Selected;
	if(m_Selected == false)
	{
		UnsetBackgroundColor();
	}
	else
	{
		SetBackgroundColor(Color(0.4f, 0.1f, 0.1f, 1.0f));
	}
}

void EquipmentListItem::OnMouseEnter(Widget * EventSource)
{
	if(GetSelected() == false)
	{
		SetBackgroundColor(Color(0.3f, 0.2f, 0.2f, 1.0f));
	}
}

void EquipmentListItem::OnMouseLeave(Widget * EventSource)
{
	if(GetSelected() == false)
	{
		UnsetBackgroundColor();
	}
}

OutfitShipDialog::OutfitShipDialog(Widget * SupWidget, Ship * Ship) :
	WWindow(SupWidget, "Outfit Ship"),
	m_Ship(Ship),
	m_SelectedSlotListItem(0),
	m_SelectedEquipmentListItem(0)
{
	AddDimensionListener(this);
	AddKeyListener(this);
	m_LeftPane = new Widget(this);
	m_LeftPane->SetPosition(Vector2f(10.0f, 40.0f));
	m_LeftPane->SetSize(Vector2f(200.0f, GetSize()[1] - 50.0f));
	m_LeftPane->SetAnchorBottom(true);
	
	Label * SlotListLabel(new Label(m_LeftPane, "Slots"));
	
	SlotListLabel->SetPosition(Vector2f(0.0f, 0.0f));
	SlotListLabel->SetSize(Vector2f(m_LeftPane->GetSize()[0], 20.0f));
	SlotListLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	SlotListLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	SlotListLabel->SetAnchorRight(true);
	m_SlotScrollBox = new ScrollBox(m_LeftPane);
	m_SlotScrollBox->SetPosition(Vector2f(0.0f, 30.0f));
	m_SlotScrollBox->SetSize(Vector2f(m_LeftPane->GetSize()[0], m_LeftPane->GetSize()[1] - 30.0f));
	m_SlotScrollBox->SetHorizontalScrollBarVisible(false);
	m_SlotScrollBox->SetAnchorBottom(true);
	m_SlotScrollBox->SetAnchorRight(true);
	
	float Top(5.0f);
	const std::map< std::string, Slot * > & Slots(m_Ship->GetSlots());
	
	for(std::map< std::string, Slot * >::const_iterator SlotIterator = Slots.begin(); SlotIterator != Slots.end(); ++SlotIterator)
	{
		SlotListItem * NewSlotListItem(new SlotListItem(m_SlotScrollBox->GetContent(), SlotIterator->second));
		
		NewSlotListItem->SetPosition(Vector2f(5.0f, Top));
		NewSlotListItem->SetSize(Vector2f(m_SlotScrollBox->GetContent()->GetSize()[0] - 10.0f, 50.0f));
		NewSlotListItem->SetAnchorRight(true);
		NewSlotListItem->AddMouseButtonListener(this);
		Top += 55.0f;
	}
	m_SlotScrollBox->GetContent()->SetSize(Vector2f(180.0f, std::max(Top, m_SlotScrollBox->GetView()->GetSize()[1])));
	m_SlotScrollBox->GetContent()->SetAnchorRight(true);
	// center pane
	m_CenterPane = new Widget(this);
	m_CenterPane->SetPosition(Vector2f(10.0f + m_LeftPane->GetSize()[0] + 10.0f, 70.0f));
	m_CenterPane->SetSize(Vector2f(160.0f, GetSize()[1] - 80.0f));
	m_CenterPane->SetAnchorBottom(true);
	m_MountButton = new Button(m_CenterPane);
	m_MountButton->SetPosition(Vector2f(0.0f, 40.0f));
	m_MountButton->SetSize(Vector2f(m_CenterPane->GetSize()[0], 20.0f));
	m_MountButton->AddClickedListener(this);
	m_MountButton->SetAnchorRight(true);
	
	Label * MountButtonLabel(new Label(m_MountButton, "Mount"));
	
	MountButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	MountButtonLabel->SetSize(Vector2f(m_MountButton->GetSize()));
	MountButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	MountButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_UnmountButton = new Button(m_CenterPane);
	m_UnmountButton->SetPosition(Vector2f(0.0f, 70.0f));
	m_UnmountButton->SetSize(Vector2f(m_CenterPane->GetSize()[0], 20.0f));
	m_UnmountButton->AddClickedListener(this);
	m_UnmountButton->SetAnchorRight(true);
	
	Label * UnmountButtonLabel(new Label(m_UnmountButton, "Unmount"));
	
	UnmountButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	UnmountButtonLabel->SetSize(Vector2f(m_UnmountButton->GetSize()));
	UnmountButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	UnmountButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_OKButton = new Button(m_CenterPane);
	m_OKButton->SetPosition(Vector2f(0.0f, m_CenterPane->GetSize()[1] - 30.0f));
	m_OKButton->SetSize(Vector2f(m_CenterPane->GetSize()[0], 20.0f));
	m_OKButton->AddClickedListener(this);
	m_OKButton->SetAnchorBottom(true);
	m_OKButton->SetAnchorRight(true);
	m_OKButton->SetAnchorTop(false);
	
	Label * OKButtonLabel(new Label(m_OKButton, "OK"));
	
	OKButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	OKButtonLabel->SetSize(Vector2f(m_OKButton->GetSize()));
	OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	// right pane
	m_RightPane = new Widget(this);
	m_RightPane->SetPosition(Vector2f(10.0f + m_LeftPane->GetSize()[0] + 10.0f + m_CenterPane->GetSize()[0] + 10.0f, 40.0f));
	m_RightPane->SetSize(Vector2f(200.0f, GetSize()[1] - 50.0f));
	m_RightPane->SetAnchorBottom(true);
	
	Label * EquipmentListLabel(new Label(m_RightPane, "Equipment"));
	
	EquipmentListLabel->SetPosition(Vector2f(0.0f, 0.0f));
	EquipmentListLabel->SetSize(Vector2f(m_RightPane->GetSize()[0], 20.0f));
	EquipmentListLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	EquipmentListLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	EquipmentListLabel->SetAnchorRight(true);
	m_EquipmentScrollBox = new ScrollBox(m_RightPane);
	m_EquipmentScrollBox->SetPosition(Vector2f(0.0f, 30.0f));
	m_EquipmentScrollBox->SetSize(Vector2f(m_RightPane->GetSize()[0], m_RightPane->GetSize()[1] - 30.0f));
	m_EquipmentScrollBox->SetHorizontalScrollBarVisible(false);
	m_EquipmentScrollBox->SetAnchorBottom(true);
	m_EquipmentScrollBox->SetAnchorRight(true);
	RebuildEquipmentList();
	UpdateButtons();
	SetPosition(Vector2f(70.0f, 400.0f));
	SetSize(Vector2f(600.0f, 400.0f));
}

void OutfitShipDialog::RebuildEquipmentList(void)
{
	// empty the weapon list first
	// save the selected weapon so we can reselect it if it is available after the rebuild
	Weapon * SelectedEquipment(0);
	
	if(m_SelectedEquipmentListItem != 0)
	{
		SelectedEquipment = m_SelectedEquipmentListItem->GetEquipment();
		m_SelectedEquipmentListItem = 0;
	}
	while(m_EquipmentScrollBox->GetContent()->GetSubWidgets().empty() == false)
	{
		m_EquipmentScrollBox->GetContent()->GetSubWidgets().front()->Destroy();
	}
	
	// now fill the weapon list
	float Top(5.0f);
	const std::set< Object * > & Content(m_Ship->GetContent());
	
	for(std::set< Object * >::iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
	{
		Weapon * ContentEquipment(dynamic_cast< Weapon * >(*ContentIterator));
		
		if((ContentEquipment != 0) && (ContentEquipment->GetSlot() == 0))
		{
			EquipmentListItem * NewEquipmentListItem(new EquipmentListItem(m_EquipmentScrollBox->GetContent(), ContentEquipment));
			
			NewEquipmentListItem->SetPosition(Vector2f(5.0f, Top));
			NewEquipmentListItem->SetSize(Vector2f(m_EquipmentScrollBox->GetContent()->GetSize()[0] - 10.0f, 50.0f));
			NewEquipmentListItem->SetAnchorRight(true);
			NewEquipmentListItem->AddMouseButtonListener(this);
			if(ContentEquipment == SelectedEquipment)
			{
				m_SelectedEquipmentListItem = NewEquipmentListItem;
				m_SelectedEquipmentListItem->SetSelected(true);
			}
			Top += 55.0f;
		}
	}
	m_EquipmentScrollBox->GetContent()->SetSize(Vector2f(180.0f, std::max(Top, m_EquipmentScrollBox->GetView()->GetSize()[1])));
	m_EquipmentScrollBox->GetContent()->SetAnchorRight(true);
}

void OutfitShipDialog::UpdateButtons(void)
{
	m_UnmountButton->SetEnabled((m_SelectedSlotListItem != 0) && (m_SelectedSlotListItem->GetSlot()->GetMountedObject().IsValid() == true));
	m_MountButton->SetEnabled((m_SelectedSlotListItem != 0) && (m_SelectedSlotListItem->GetSlot()->GetMountedObject().IsValid() == false) && (m_SelectedEquipmentListItem != 0) && (m_SelectedSlotListItem->GetSlot()->GetSlotClass()->AcceptsSlotClassIdentifier(m_SelectedEquipmentListItem->GetEquipment()->GetWeaponClass()->GetSlotClassIdentifier()) == true));
}

bool OutfitShipDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		Destroy();
		
		return true;
	}
	else if(EventSource == m_MountButton)
	{
		if((m_SelectedSlotListItem != 0) && (m_SelectedEquipmentListItem != 0) && (m_SelectedSlotListItem->GetSlot()->GetMountedObject().IsValid() == false))
		{
			m_Ship->Mount(m_SelectedEquipmentListItem->GetEquipment(), m_SelectedSlotListItem->GetSlot()->GetIdentifier());
			m_SelectedSlotListItem->Update();
			RebuildEquipmentList();
			UpdateButtons();
			
			return true;
		}
	}
	else if(EventSource == m_UnmountButton)
	{
		if((m_SelectedSlotListItem != 0) && (m_SelectedSlotListItem->GetSlot()->GetMountedObject().IsValid() == true))
		{
			m_Ship->Unmount(m_SelectedSlotListItem->GetSlot()->GetIdentifier());
			m_SelectedSlotListItem->Update();
			RebuildEquipmentList();
			UpdateButtons();
			
			return true;
		}
	}
	
	return false;
}

bool OutfitShipDialog::OnKey(Widget * EventSource, const KeyEventInformation & KeyEventInformation)
{
	if(((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) || (KeyEventInformation.GetKeyCode() == 36 /* RETURN */) || (KeyEventInformation.GetKeyCode() == 25 /* W */)) && (KeyEventInformation.IsDown() == true))
	{
		Destroy();
	}
	// eat all other input
	return true;
}

bool OutfitShipDialog::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	bool Result(false);
	
	Result = WWindow::OnMouseButton(EventSource, Button, State, X, Y) == true;
	if(Result == false)
	{
		if((Button == 1 /* LEFT */) && (State == EV_DOWN))
		{
			if(dynamic_cast< SlotListItem * >(EventSource) != 0)
			{
				SlotListItem * SelectedSlotListItem(dynamic_cast< SlotListItem * >(EventSource));
				
				if(m_SelectedSlotListItem != 0)
				{
					m_SelectedSlotListItem->SetSelected(false);
				}
				m_SelectedSlotListItem = SelectedSlotListItem;
				m_SelectedSlotListItem->SetSelected(true);
				Result = true;
			}
			else if(dynamic_cast< EquipmentListItem * >(EventSource) != 0)
			{
				EquipmentListItem * SelectedEquipmentListItem(dynamic_cast< EquipmentListItem * >(EventSource));
				
				if(m_SelectedEquipmentListItem != 0)
				{
					m_SelectedEquipmentListItem->SetSelected(false);
				}
				m_SelectedEquipmentListItem = SelectedEquipmentListItem;
				m_SelectedEquipmentListItem->SetSelected(true);
				Result = true;
			}
		}
		if(Result == true)
		{
			UpdateButtons();
		}
	}
	
	return Result;
}

void OutfitShipDialog::OnSizeChanged(Widget * EventSource)
{
	float AvailableWidth(GetSize()[0]);
	
	// substract 10.0f for each border: left of left, between left and center, between center and right, right of right
	AvailableWidth -= 10.0f + 10.0f + 10.0f + 10.0f;
	AvailableWidth -= m_CenterPane->GetSize()[0];
	AvailableWidth /= 2.0f;
	m_LeftPane->SetSize(Vector2f(AvailableWidth, m_LeftPane->GetSize()[1]));
	m_CenterPane->SetPosition(Vector2f(10.0f + m_LeftPane->GetSize()[0] + 10.0f, m_CenterPane->GetPosition()[1]));
	m_RightPane->SetSize(Vector2f(AvailableWidth, m_RightPane->GetSize()[1]));
	m_RightPane->SetPosition(Vector2f(10.0f + m_LeftPane->GetSize()[0] + 10.0f + m_CenterPane->GetSize()[0] + 10.0f, m_RightPane->GetPosition()[1]));
}
