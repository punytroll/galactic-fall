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
#include "object_aspect_accessory.h"
#include "object_aspect_name.h"
#include "object_aspect_object_container.h"
#include "object_aspect_physical.h"
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

class AccessoryListItem : public MouseMotionListener, public Widget
{
public:
	AccessoryListItem(Widget * SupWidget, Object * Accessory);
	void Update(void);
	// getters
	bool GetSelected(void) const;
	Object * GetAccessory(void);
	// setters
	void SetSelected(bool Selected);
protected:
	virtual void OnMouseEnter(Widget * EventSource);
	virtual void OnMouseLeave(Widget * EventSource);
private:
	bool m_Selected;
	Object * m_Accessory;
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
	Object * MountedObject(dynamic_cast< Object * >(m_Slot->GetMountedObject().Get()));
	
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

AccessoryListItem::AccessoryListItem(Widget * SupWidget, Object * Accessory) :
	Widget(SupWidget),
	m_Selected(false),
	m_Accessory(Accessory)
{
	AddMouseMotionListener(this);
	// set to arbitrary design size
	SetSize(Vector2f(100.0f, 100.0f));
	// safe-guard: only accept objects with a name aspect
	assert(Accessory->GetAspectName() != 0);
	// safe-guard: only accept objects with a accessory aspect
	assert(Accessory->GetAspectAccessory() != 0);
	
	Label * NameLabel(new Label(this, Accessory->GetAspectName()->GetName()));
	
	NameLabel->SetPosition(Vector2f(5.0f, 5.0f));
	NameLabel->SetSize(Vector2f(90.0f, 20.0f));
	NameLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	NameLabel->SetAnchorLeft(true);
	NameLabel->SetAnchorRight(true);
	NameLabel->SetAnchorTop(true);
	
	Label * SlotTypeLabel(new Label(this, g_SlotClassManager->Get(Accessory->GetAspectAccessory()->GetSlotClassIdentifier())->GetName()));
	
	SlotTypeLabel->SetPosition(Vector2f(25.0f, 25.0f));
	SlotTypeLabel->SetSize(Vector2f(70.0f, 20.0f));
	SlotTypeLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	SlotTypeLabel->SetAnchorLeft(true);
	SlotTypeLabel->SetAnchorRight(true);
	SlotTypeLabel->SetAnchorTop(true);
}

bool AccessoryListItem::GetSelected(void) const
{
	return m_Selected;
}

Object * AccessoryListItem::GetAccessory(void)
{
	return m_Accessory;
}

void AccessoryListItem::SetSelected(bool Selected)
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

void AccessoryListItem::OnMouseEnter(Widget * EventSource)
{
	if(GetSelected() == false)
	{
		SetBackgroundColor(Color(0.3f, 0.2f, 0.2f, 1.0f));
	}
}

void AccessoryListItem::OnMouseLeave(Widget * EventSource)
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
	m_SelectedAccessoryListItem(0)
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
	
	Label * AccessoryListLabel(new Label(m_RightPane, "Accessories"));
	
	AccessoryListLabel->SetPosition(Vector2f(0.0f, 0.0f));
	AccessoryListLabel->SetSize(Vector2f(m_RightPane->GetSize()[0], 20.0f));
	AccessoryListLabel->SetHorizontalAlignment(Label::ALIGN_HORIZONTAL_CENTER);
	AccessoryListLabel->SetVerticalAlignment(Label::ALIGN_VERTICAL_CENTER);
	AccessoryListLabel->SetAnchorRight(true);
	m_AccessoryScrollBox = new ScrollBox(m_RightPane);
	m_AccessoryScrollBox->SetPosition(Vector2f(0.0f, 30.0f));
	m_AccessoryScrollBox->SetSize(Vector2f(m_RightPane->GetSize()[0], m_RightPane->GetSize()[1] - 30.0f));
	m_AccessoryScrollBox->SetHorizontalScrollBarVisible(false);
	m_AccessoryScrollBox->SetAnchorBottom(true);
	m_AccessoryScrollBox->SetAnchorRight(true);
	RebuildAccessoryList();
	UpdateButtons();
	SetPosition(Vector2f(70.0f, 400.0f));
	SetSize(Vector2f(600.0f, 400.0f));
}

void OutfitShipDialog::RebuildAccessoryList(void)
{
	// empty the weapon list first
	// save the selected weapon so we can reselect it if it is available after the rebuild
	Object * SelectedAccessory(0);
	
	if(m_SelectedAccessoryListItem != 0)
	{
		SelectedAccessory = m_SelectedAccessoryListItem->GetAccessory();
		m_SelectedAccessoryListItem = 0;
	}
	while(m_AccessoryScrollBox->GetContent()->GetSubWidgets().empty() == false)
	{
		m_AccessoryScrollBox->GetContent()->GetSubWidgets().front()->Destroy();
	}
	
	// now fill the weapon list
	float Top(5.0f);
	if(m_Ship->GetAspectObjectContainer() != 0)
	{
		const std::set< Object * > & Content(m_Ship->GetAspectObjectContainer()->GetContent());
		
		for(std::set< Object * >::iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
		{
			Object * ContentObject(*ContentIterator);
			
			if((ContentObject->GetAspectAccessory() != 0) && (ContentObject->GetAspectAccessory()->GetSlot() == 0))
			{
				AccessoryListItem * NewAccessoryListItem(new AccessoryListItem(m_AccessoryScrollBox->GetContent(), ContentObject));
				
				NewAccessoryListItem->SetPosition(Vector2f(5.0f, Top));
				NewAccessoryListItem->SetSize(Vector2f(m_AccessoryScrollBox->GetContent()->GetSize()[0] - 10.0f, 50.0f));
				NewAccessoryListItem->SetAnchorRight(true);
				NewAccessoryListItem->AddMouseButtonListener(this);
				if(ContentObject == SelectedAccessory)
				{
					m_SelectedAccessoryListItem = NewAccessoryListItem;
					m_SelectedAccessoryListItem->SetSelected(true);
				}
				Top += 55.0f;
			}
		}
	}
	m_AccessoryScrollBox->GetContent()->SetSize(Vector2f(m_AccessoryScrollBox->GetView()->GetSize()[0], std::max(Top, m_AccessoryScrollBox->GetView()->GetSize()[1])));
	m_AccessoryScrollBox->GetContent()->SetAnchorRight(true);
}

void OutfitShipDialog::UpdateButtons(void)
{
	m_UnmountButton->SetEnabled(false);
	m_MountButton->SetEnabled(false);
	if(m_SelectedSlotListItem != 0)
	{
		if(m_SelectedSlotListItem->GetSlot()->GetMountedObject().IsValid() == false)
		{
			m_MountButton->SetEnabled((m_SelectedAccessoryListItem != 0) && (m_SelectedSlotListItem->GetSlot()->GetSlotClass()->AcceptsSlotClassIdentifier(m_SelectedAccessoryListItem->GetAccessory()->GetAspectAccessory()->GetSlotClassIdentifier()) == true));
		}
		else
		{
			assert(m_SelectedSlotListItem->GetSlot()->GetMountedObject()->GetAspectPhysical() != 0);
			m_UnmountButton->SetEnabled((m_Ship->GetAvailableSpace() >= m_SelectedSlotListItem->GetSlot()->GetMountedObject()->GetAspectPhysical()->GetSpaceRequirement()));
		}
	}
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
		assert(m_SelectedAccessoryListItem != 0);
		assert(m_SelectedSlotListItem != 0);
		m_Ship->Mount(m_SelectedAccessoryListItem->GetAccessory(), m_SelectedSlotListItem->GetSlot()->GetIdentifier());
		m_SelectedSlotListItem->Update();
		RebuildAccessoryList();
		UpdateButtons();
		
		return true;
	}
	else if(EventSource == m_UnmountButton)
	{
		assert(m_SelectedSlotListItem != 0);
		m_Ship->Unmount(m_SelectedSlotListItem->GetSlot()->GetIdentifier());
		m_SelectedSlotListItem->Update();
		RebuildAccessoryList();
		UpdateButtons();
		
		return true;
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
			else if(dynamic_cast< AccessoryListItem * >(EventSource) != 0)
			{
				AccessoryListItem * SelectedAccessoryListItem(dynamic_cast< AccessoryListItem * >(EventSource));
				
				if(m_SelectedAccessoryListItem != 0)
				{
					m_SelectedAccessoryListItem->SetSelected(false);
				}
				m_SelectedAccessoryListItem = SelectedAccessoryListItem;
				m_SelectedAccessoryListItem->SetSelected(true);
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
