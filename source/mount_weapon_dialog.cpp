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
#include "label.h"
#include "mount_weapon_dialog.h"
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

class WeaponListItem : public MouseMotionListener, public Widget
{
public:
	WeaponListItem(Widget * SupWidget, Weapon * Weapon);
	void Update(void);
	// getters
	bool GetSelected(void) const;
	Weapon * GetWeapon(void);
	// setters
	void SetSelected(bool Selected);
protected:
	virtual void OnMouseEnter(Widget * EventSource);
	virtual void OnMouseLeave(Widget * EventSource);
private:
	bool m_Selected;
	Weapon * m_Weapon;
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
		m_TypeOrWeaponLabel->SetString(MountedObject->GetName());
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
		SetBackgroundColor(Color(0.4f, 0.1f, 0.1f));
	}
}

void SlotListItem::OnMouseEnter(Widget * EventSource)
{
	if(GetSelected() == false)
	{
		SetBackgroundColor(Color(0.3f, 0.2f, 0.2f));
	}
}

void SlotListItem::OnMouseLeave(Widget * EventSource)
{
	if(GetSelected() == false)
	{
		UnsetBackgroundColor();
	}
}

WeaponListItem::WeaponListItem(Widget * SupWidget, Weapon * Weapon) :
	Widget(SupWidget),
	m_Selected(false),
	m_Weapon(Weapon)
{
	AddMouseMotionListener(this);
	// set to arbitrary design size
	SetSize(Vector2f(100.0f, 100.0f));
	
	Label * NameLabel(new Label(this, Weapon->GetName()));
	
	NameLabel->SetPosition(Vector2f(5.0f, 5.0f));
	NameLabel->SetSize(Vector2f(90.0f, 20.0f));
	NameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	NameLabel->SetAnchorLeft(true);
	NameLabel->SetAnchorRight(true);
	NameLabel->SetAnchorTop(true);
	
	Label * SlotTypeLabel(new Label(this, g_SlotClassManager->Get(Weapon->GetWeaponClass()->GetSlotClassIdentifier())->GetName()));
	
	SlotTypeLabel->SetPosition(Vector2f(25.0f, 25.0f));
	SlotTypeLabel->SetSize(Vector2f(70.0f, 20.0f));
	SlotTypeLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	SlotTypeLabel->SetAnchorLeft(true);
	SlotTypeLabel->SetAnchorRight(true);
	SlotTypeLabel->SetAnchorTop(true);
}

bool WeaponListItem::GetSelected(void) const
{
	return m_Selected;
}

Weapon * WeaponListItem::GetWeapon(void)
{
	return m_Weapon;
}

void WeaponListItem::SetSelected(bool Selected)
{
	m_Selected = Selected;
	if(m_Selected == false)
	{
		UnsetBackgroundColor();
	}
	else
	{
		SetBackgroundColor(Color(0.4f, 0.1f, 0.1f));
	}
}

void WeaponListItem::OnMouseEnter(Widget * EventSource)
{
	if(GetSelected() == false)
	{
		SetBackgroundColor(Color(0.3f, 0.2f, 0.2f));
	}
}

void WeaponListItem::OnMouseLeave(Widget * EventSource)
{
	if(GetSelected() == false)
	{
		UnsetBackgroundColor();
	}
}

MountWeaponDialog::MountWeaponDialog(Widget * SupWidget, Ship * Ship) :
	WWindow(SupWidget, "Mount Weapons"),
	m_Ship(Ship),
	m_SelectedSlotListItem(0),
	m_SelectedWeaponListItem(0)
{
	SetPosition(Vector2f(70.0f, 400.0f));
	SetSize(Vector2f(600.0f, 400.0f));
	AddKeyListener(this);
	
	Label * SlotListLabel(new Label(this, "Slots"));
	
	SlotListLabel->SetPosition(Vector2f(10.0f, 40.0f));
	SlotListLabel->SetSize(Vector2f(200.0f, 20.0f));
	SlotListLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	SlotListLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	
	Widget * SlotList(new Widget(this));
	
	SlotList->SetPosition(Vector2f(10.0f, 70.0f));
	SlotList->SetSize(Vector2f(200.0f, 320.0f));
	SlotList->SetBackgroundColor(Color(0.15f, 0.15f, 0.15f, 1.0f));
	
	float Top(5.0f);
	const std::map< std::string, Slot * > & Slots(m_Ship->GetSlots());
	
	for(std::map< std::string, Slot * >::const_iterator SlotIterator = Slots.begin(); SlotIterator != Slots.end(); ++SlotIterator)
	{
		SlotListItem * NewSlotListItem(new SlotListItem(SlotList, SlotIterator->second));
		
		NewSlotListItem->SetPosition(Vector2f(5.0f, Top));
		NewSlotListItem->SetSize(Vector2f(190.0f, 50.0f));
		NewSlotListItem->AddMouseButtonListener(this);
		Top += 55.0f;
	}
	
	Label * WeaponListLabel(new Label(this, "Weapons"));
	
	WeaponListLabel->SetPosition(Vector2f(390.0f, 40.0f));
	WeaponListLabel->SetSize(Vector2f(200.0f, 20.0f));
	WeaponListLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	WeaponListLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_WeaponList = new Widget(this);
	m_WeaponList->SetPosition(Vector2f(390.0f, 70.0f));
	m_WeaponList->SetSize(Vector2f(200.0f, 320.0f));
	m_WeaponList->SetBackgroundColor(Color(0.15f, 0.15f, 0.15f, 1.0f));
	m_MountButton = new Button(this);
	m_MountButton->SetPosition(Vector2f(220.0f, 110.0f));
	m_MountButton->SetSize(Vector2f(160.0f, 20.0f));
	m_MountButton->AddClickedListener(this);
	
	Label * MountButtonLabel(new Label(m_MountButton, "Mount"));
	
	MountButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	MountButtonLabel->SetSize(Vector2f(m_MountButton->GetSize()));
	MountButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	MountButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_UnmountButton = new Button(this);
	m_UnmountButton->SetPosition(Vector2f(220.0f, 140.0f));
	m_UnmountButton->SetSize(Vector2f(160.0f, 20.0f));
	m_UnmountButton->AddClickedListener(this);
	
	Label * UnmountButtonLabel(new Label(m_UnmountButton, "Unmount"));
	
	UnmountButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	UnmountButtonLabel->SetSize(Vector2f(m_UnmountButton->GetSize()));
	UnmountButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	UnmountButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	m_OKButton = new Button(this);
	m_OKButton->SetPosition(Vector2f(220.0f, 370.0f));
	m_OKButton->SetSize(Vector2f(160.0f, 20.0f));
	m_OKButton->AddClickedListener(this);
	
	Label * OKButtonLabel(new Label(m_OKButton, "OK"));
	
	OKButtonLabel->SetPosition(Vector2f(0.0f, 0.0f));
	OKButtonLabel->SetSize(Vector2f(m_OKButton->GetSize()));
	OKButtonLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	OKButtonLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	RebuildWeaponList();
}

void MountWeaponDialog::RebuildWeaponList(void)
{
	// empty the weapon list first
	// save the selected weapon so we can reselect it if it is available after the rebuild
	Weapon * SelectedWeapon(0);
	
	if(m_SelectedWeaponListItem != 0)
	{
		SelectedWeapon = m_SelectedWeaponListItem->GetWeapon();
		m_SelectedWeaponListItem = 0;
	}
	while(m_WeaponList->GetSubWidgets().empty() == false)
	{
		m_WeaponList->GetSubWidgets().front()->Destroy();
	}
	
	// now fill the weapon list
	float Top(5.0f);
	const std::set< Object * > & Content(m_Ship->GetContent());
	
	for(std::set< Object * >::iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
	{
		Weapon * ContentWeapon(dynamic_cast< Weapon * >(*ContentIterator));
		
		if((ContentWeapon != 0) && (ContentWeapon->GetSlot() == 0))
		{
			WeaponListItem * NewWeaponListItem(new WeaponListItem(m_WeaponList, ContentWeapon));
			
			NewWeaponListItem->SetPosition(Vector2f(5.0f, Top));
			NewWeaponListItem->SetSize(Vector2f(190.0f, 50.0f));
			NewWeaponListItem->AddMouseButtonListener(this);
			if(ContentWeapon == SelectedWeapon)
			{
				m_SelectedWeaponListItem = NewWeaponListItem;
				m_SelectedWeaponListItem->SetSelected(true);
			}
			Top += 55.0f;
		}
	}
}

bool MountWeaponDialog::OnClicked(Widget * EventSource)
{
	if(EventSource == m_OKButton)
	{
		Destroy();
		
		return true;
	}
	else if(EventSource == m_MountButton)
	{
		if((m_SelectedSlotListItem != 0) && (m_SelectedWeaponListItem != 0) && (m_SelectedSlotListItem->GetSlot()->GetMountedObject() == false))
		{
			m_Ship->Mount(m_SelectedWeaponListItem->GetWeapon(), m_SelectedSlotListItem->GetSlot()->GetIdentifier());
			m_SelectedSlotListItem->Update();
			RebuildWeaponList();
		}
	}
	else if(EventSource == m_UnmountButton)
	{
		if((m_SelectedSlotListItem != 0) && (m_SelectedSlotListItem->GetSlot()->GetMountedObject() == true))
		{
			m_Ship->Unmount(m_SelectedSlotListItem->GetSlot()->GetIdentifier());
			m_SelectedSlotListItem->Update();
			RebuildWeaponList();
		}
	}
	
	return false;
}

bool MountWeaponDialog::OnKey(Widget * EventSource, int KeyCode, int State)
{
	if(((KeyCode == 9 /* ESCAPE */) || (KeyCode == 36 /* RETURN */) || (KeyCode == 25 /* W */)) && (State == EV_DOWN))
	{
		Destroy();
	}
	// eat all other input
	return true;
}

bool MountWeaponDialog::OnMouseButton(Widget * EventSource, int Button, int State, float X, float Y)
{
	if(WWindow::OnMouseButton(EventSource, Button, State, X, Y) == true)
	{
		return true;
	}
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
			
			return true;
		}
		else if(dynamic_cast< WeaponListItem * >(EventSource) != 0)
		{
			WeaponListItem * SelectedWeaponListItem(dynamic_cast< WeaponListItem * >(EventSource));
			
			if(m_SelectedWeaponListItem != 0)
			{
				m_SelectedWeaponListItem->SetSelected(false);
			}
			m_SelectedWeaponListItem = SelectedWeaponListItem;
			m_SelectedWeaponListItem->SetSelected(true);
			
			return true;
		}
	}
	
	return false;
}
