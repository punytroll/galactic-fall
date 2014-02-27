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

#include "../class_manager.h"
#include "../color.h"
#include "../globals.h"
#include "../key_event_information.h"
#include "../object_aspect_accessory.h"
#include "../object_aspect_name.h"
#include "../object_aspect_object_container.h"
#include "../object_aspect_outfitting.h"
#include "../object_aspect_physical.h"
#include "../ship.h"
#include "../slot.h"
#include "../slot_class.h"
#include "../storage.h"
#include "../weapon.h"
#include "../weapon_class.h"

#include "label.h"
#include "outfit_ship_dialog.h"
#include "scroll_box.h"
#include "text_button.h"

namespace UI
{
	class AccessoryListItem : public UI::Widget
	{
	public:
		AccessoryListItem(UI::Widget * SupWidget, Reference< Object > Accessory);
		void Update(void);
		// getters
		Reference< Object > GetAccessory(void);
		// setters
		void SetSelected(bool Selected);
	private:
		// callbacks
		void _OnMouseEnter(void);
		void _OnMouseLeave(void);
		// member variables
		Reference< Object > _Accessory;
		bool _Selected;
	};
	
	class SlotListItem : public UI::Widget
	{
	public:
		SlotListItem(UI::Widget * SupWidget, Reference< Slot > Slot);
		void Update(void);
		// getters
		Reference< Slot > GetSlot(void);
		// setters
		void SetSelected(bool Selected);
	private:
		// callbacks
		void _OnMouseEnter(void);
		void _OnMouseLeave(void);
		// member variables
		bool _Selected;
		Reference< Slot> _Slot;
		UI::Label * _TypeOrWeaponLabel;
	};
}

UI::AccessoryListItem::AccessoryListItem(UI::Widget * SupWidget, Reference< Object > Accessory) :
	UI::Widget(SupWidget),
	_Accessory(Accessory),
	_Selected(false)
{
	ConnectMouseEnterCallback(std::bind(&UI::AccessoryListItem::_OnMouseEnter, this));
	ConnectMouseLeaveCallback(std::bind(&UI::AccessoryListItem::_OnMouseLeave, this));
	// set to arbitrary design size
	SetSize(Vector2f(100.0f, 100.0f));
	// safe-guard: only accept objects with a name aspect
	assert(Accessory->GetAspectName() != 0);
	// safe-guard: only accept objects with a accessory aspect
	assert(Accessory->GetAspectAccessory() != 0);
	
	UI::Label * NameLabel(new UI::Label(this, Accessory->GetAspectName()->GetName()));
	
	NameLabel->SetPosition(Vector2f(5.0f, 5.0f));
	NameLabel->SetSize(Vector2f(90.0f, 20.0f));
	NameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	NameLabel->SetAnchorLeft(true);
	NameLabel->SetAnchorRight(true);
	NameLabel->SetAnchorTop(true);
	
	UI::Label * SlotTypeLabel(new UI::Label(this, g_SlotClassManager->Get(Accessory->GetAspectAccessory()->GetSlotClassIdentifier())->GetName()));
	
	SlotTypeLabel->SetPosition(Vector2f(25.0f, 25.0f));
	SlotTypeLabel->SetSize(Vector2f(70.0f, 20.0f));
	SlotTypeLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	SlotTypeLabel->SetAnchorLeft(true);
	SlotTypeLabel->SetAnchorRight(true);
	SlotTypeLabel->SetAnchorTop(true);
}

Reference< Object > UI::AccessoryListItem::GetAccessory(void)
{
	return _Accessory;
}

void UI::AccessoryListItem::SetSelected(bool Selected)
{
	_Selected = Selected;
	if(_Selected == false)
	{
		UnsetBackgroundColor();
	}
	else
	{
		SetBackgroundColor(Color(0.4f, 0.1f, 0.1f, 1.0f));
	}
}

void UI::AccessoryListItem::_OnMouseEnter(void)
{
	if(_Selected == false)
	{
		SetBackgroundColor(Color(0.3f, 0.2f, 0.2f, 1.0f));
	}
}

void UI::AccessoryListItem::_OnMouseLeave(void)
{
	if(_Selected == false)
	{
		UnsetBackgroundColor();
	}
}

UI::SlotListItem::SlotListItem(UI::Widget * SupWidget, Reference< Slot > Slot) :
	UI::Widget(SupWidget),
	_Selected(false),
	_Slot(Slot)
{
	ConnectMouseEnterCallback(std::bind(&UI::SlotListItem::_OnMouseEnter, this));
	ConnectMouseLeaveCallback(std::bind(&UI::SlotListItem::_OnMouseLeave, this));
	// set to arbitrary design size
	SetSize(Vector2f(100.0f, 100.0f));
	
	UI::Label * IdentifierLabel(new UI::Label(this, _Slot->GetName()));
	
	IdentifierLabel->SetPosition(Vector2f(5.0f, 5.0f));
	IdentifierLabel->SetSize(Vector2f(90.0f, 20.0f));
	IdentifierLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	IdentifierLabel->SetAnchorLeft(true);
	IdentifierLabel->SetAnchorRight(true);
	IdentifierLabel->SetAnchorTop(true);
	_TypeOrWeaponLabel = new UI::Label(this);
	_TypeOrWeaponLabel->SetPosition(Vector2f(25.0f, 25.0f));
	_TypeOrWeaponLabel->SetSize(Vector2f(70.0f, 20.0f));
	_TypeOrWeaponLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	_TypeOrWeaponLabel->SetAnchorLeft(true);
	_TypeOrWeaponLabel->SetAnchorRight(true);
	_TypeOrWeaponLabel->SetAnchorTop(true);
	Update();
}

void UI::SlotListItem::Update(void)
{
	Object * MountedObject(dynamic_cast< Object * >(_Slot->GetMountedObject()));
	
	if(MountedObject != 0)
	{
		_TypeOrWeaponLabel->SetText(MountedObject->GetAspectName()->GetName());
		_TypeOrWeaponLabel->SetTextColor(Color(0.6f, 0.8f, 0.6f, 1.0f));
	}
	else
	{
		_TypeOrWeaponLabel->SetText(_Slot->GetSlotClass()->GetName());
		_TypeOrWeaponLabel->SetTextColor(Color(0.8f, 0.6f, 0.6f, 1.0f));
	}
}

Reference< Slot > UI::SlotListItem::GetSlot(void)
{
	return _Slot;
}

void UI::SlotListItem::SetSelected(bool Selected)
{
	_Selected = Selected;
	if(_Selected == false)
	{
		UnsetBackgroundColor();
	}
	else
	{
		SetBackgroundColor(Color(0.4f, 0.1f, 0.1f, 1.0f));
	}
}

void UI::SlotListItem::_OnMouseEnter(void)
{
	if(_Selected == false)
	{
		SetBackgroundColor(Color(0.3f, 0.2f, 0.2f, 1.0f));
	}
}

void UI::SlotListItem::_OnMouseLeave(void)
{
	if(_Selected == false)
	{
		UnsetBackgroundColor();
	}
}

UI::OutfitShipDialog::OutfitShipDialog(UI::Widget * SupWidget, Reference< Ship > Ship) :
	UI::Window(SupWidget, "Outfit Ship"),
	_SelectedAccessoryListItem(0),
	_SelectedSlotListItem(0),
	_Ship(Ship)
{
	ConnectSizeChangedCallback(std::bind(&UI::OutfitShipDialog::_OnSizeChanged, this));
	ConnectKeyCallback(std::bind(&UI::OutfitShipDialog::_OnKey, this, std::placeholders::_1));
	_LeftPane = new UI::Widget(this);
	_LeftPane->SetPosition(Vector2f(10.0f, 40.0f));
	_LeftPane->SetSize(Vector2f(200.0f, GetSize()[1] - 50.0f));
	_LeftPane->SetAnchorBottom(true);
	
	UI::Label * SlotListLabel(new UI::Label(_LeftPane, "Slots"));
	
	SlotListLabel->SetPosition(Vector2f(0.0f, 0.0f));
	SlotListLabel->SetSize(Vector2f(_LeftPane->GetSize()[0], 20.0f));
	SlotListLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	SlotListLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	SlotListLabel->SetAnchorRight(true);
	_SlotScrollBox = new UI::ScrollBox(_LeftPane);
	_SlotScrollBox->SetPosition(Vector2f(0.0f, 30.0f));
	_SlotScrollBox->SetSize(Vector2f(_LeftPane->GetSize()[0], _LeftPane->GetSize()[1] - 30.0f));
	_SlotScrollBox->SetHorizontalScrollBarVisible(false);
	_SlotScrollBox->SetAnchorBottom(true);
	_SlotScrollBox->SetAnchorRight(true);
	
	float Top(5.0f);
	const std::map< std::string, Slot * > & Slots(_Ship->GetAspectOutfitting()->GetSlots());
	
	for(std::map< std::string, Slot * >::const_iterator SlotIterator = Slots.begin(); SlotIterator != Slots.end(); ++SlotIterator)
	{
		SlotListItem * NewSlotListItem(new SlotListItem(_SlotScrollBox->GetContent(), SlotIterator->second->GetReference()));
		
		NewSlotListItem->SetPosition(Vector2f(5.0f, Top));
		NewSlotListItem->SetSize(Vector2f(_SlotScrollBox->GetContent()->GetSize()[0] - 10.0f, 50.0f));
		NewSlotListItem->SetAnchorRight(true);
		NewSlotListItem->ConnectMouseButtonCallback(std::bind(&UI::OutfitShipDialog::_OnSlotListItemMouseButton, this, NewSlotListItem, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
		Top += 55.0f;
	}
	_SlotScrollBox->GetContent()->SetSize(Vector2f(180.0f, std::max(Top, _SlotScrollBox->GetView()->GetSize()[1])));
	_SlotScrollBox->GetContent()->SetAnchorRight(true);
	// center pane
	_CenterPane = new UI::Widget(this);
	_CenterPane->SetPosition(Vector2f(10.0f + _LeftPane->GetSize()[0] + 10.0f, 70.0f));
	_CenterPane->SetSize(Vector2f(160.0f, GetSize()[1] - 80.0f));
	_CenterPane->SetAnchorBottom(true);
	
	UI::Button * MountButton(new UI::TextButton(_CenterPane, "Mount"));
	
	MountButton->SetPosition(Vector2f(0.0f, 40.0f));
	MountButton->SetSize(Vector2f(_CenterPane->GetSize()[0], 20.0f));
	MountButton->SetAnchorRight(true);
	MountButton->ConnectClickedCallback(std::bind(&UI::OutfitShipDialog::_OnMountButtonClicked, this));
	MountButton->ConnectUpdatingCallback(std::bind(&UI::OutfitShipDialog::_OnMountButtonUpdating, this, MountButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * UnmountButton(new UI::TextButton(_CenterPane, "Unmount"));
	
	UnmountButton->SetPosition(Vector2f(0.0f, 70.0f));
	UnmountButton->SetSize(Vector2f(_CenterPane->GetSize()[0], 20.0f));
	UnmountButton->SetAnchorRight(true);
	UnmountButton->ConnectClickedCallback(std::bind(&UI::OutfitShipDialog::_OnUnmountButtonClicked, this));
	UnmountButton->ConnectUpdatingCallback(std::bind(&UI::OutfitShipDialog::_OnUnmountButtonUpdating, this, UnmountButton, std::placeholders::_1, std::placeholders::_2));
	
	UI::Button * OKButton(new UI::TextButton(_CenterPane, "OK"));
	
	OKButton->SetPosition(Vector2f(0.0f, _CenterPane->GetSize()[1] - 30.0f));
	OKButton->SetSize(Vector2f(_CenterPane->GetSize()[0], 20.0f));
	OKButton->SetAnchorBottom(true);
	OKButton->SetAnchorRight(true);
	OKButton->SetAnchorTop(false);
	OKButton->ConnectClickedCallback(std::bind(&UI::OutfitShipDialog::_OnOKButtonClicked, this));
	// right pane
	_RightPane = new UI::Widget(this);
	_RightPane->SetPosition(Vector2f(10.0f + _LeftPane->GetSize()[0] + 10.0f + _CenterPane->GetSize()[0] + 10.0f, 40.0f));
	_RightPane->SetSize(Vector2f(200.0f, GetSize()[1] - 50.0f));
	_RightPane->SetAnchorBottom(true);
	
	UI::Label * AccessoryListLabel(new UI::Label(_RightPane, "Accessories"));
	
	AccessoryListLabel->SetPosition(Vector2f(0.0f, 0.0f));
	AccessoryListLabel->SetSize(Vector2f(_RightPane->GetSize()[0], 20.0f));
	AccessoryListLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	AccessoryListLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	AccessoryListLabel->SetAnchorRight(true);
	_AccessoryScrollBox = new UI::ScrollBox(_RightPane);
	_AccessoryScrollBox->SetPosition(Vector2f(0.0f, 30.0f));
	_AccessoryScrollBox->SetSize(Vector2f(_RightPane->GetSize()[0], _RightPane->GetSize()[1] - 30.0f));
	_AccessoryScrollBox->SetHorizontalScrollBarVisible(false);
	_AccessoryScrollBox->SetAnchorBottom(true);
	_AccessoryScrollBox->SetAnchorRight(true);
	_RebuildAccessoryList();
	SetPosition(Vector2f(70.0f, 400.0f));
	SetSize(Vector2f(600.0f, 400.0f));
}

void UI::OutfitShipDialog::_RebuildAccessoryList(void)
{
	// empty the weapon list first
	// save the selected weapon so we can reselect it if it is available after the rebuild
	Reference< Object > SelectedAccessory;
	
	if(_SelectedAccessoryListItem != 0)
	{
		SelectedAccessory = _SelectedAccessoryListItem->GetAccessory();
		_SelectedAccessoryListItem = 0;
	}
	while(_AccessoryScrollBox->GetContent()->GetSubWidgets().empty() == false)
	{
		_AccessoryScrollBox->GetContent()->GetSubWidgets().front()->Destroy();
	}
	// now fill the weapon list
	assert(_Ship.IsValid() == true);
	assert(_Ship->GetCargoHold() != 0);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != 0);
	
	const std::set< Object * > & Content(_Ship->GetCargoHold()->GetAspectObjectContainer()->GetContent());
	float Top(5.0f);
	
	for(std::set< Object * >::iterator ContentIterator = Content.begin(); ContentIterator != Content.end(); ++ContentIterator)
	{
		Reference< Object > ContentObject((*ContentIterator)->GetReference());
		
		if((ContentObject->GetAspectAccessory() != 0) && (ContentObject->GetAspectAccessory()->GetSlot() == 0))
		{
			AccessoryListItem * NewAccessoryListItem(new UI::AccessoryListItem(_AccessoryScrollBox->GetContent(), ContentObject));
			
			NewAccessoryListItem->SetPosition(Vector2f(5.0f, Top));
			NewAccessoryListItem->SetSize(Vector2f(_AccessoryScrollBox->GetContent()->GetSize()[0] - 10.0f, 50.0f));
			NewAccessoryListItem->SetAnchorRight(true);
			NewAccessoryListItem->ConnectMouseButtonCallback(std::bind(&UI::OutfitShipDialog::_OnAccessoryListItemMouseButton, this, NewAccessoryListItem, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
			if(ContentObject == SelectedAccessory)
			{
				_SelectedAccessoryListItem = NewAccessoryListItem;
				_SelectedAccessoryListItem->SetSelected(true);
			}
			Top += 55.0f;
		}
	}
	_AccessoryScrollBox->GetContent()->SetSize(Vector2f(_AccessoryScrollBox->GetView()->GetSize()[0], std::max(Top, _AccessoryScrollBox->GetView()->GetSize()[1])));
	_AccessoryScrollBox->GetContent()->SetAnchorRight(true);
}

void UI::OutfitShipDialog::_OnMountButtonUpdating(UI::Button * MountButton, float RealTimeSeconds, float GameTimeSeconds)
{
	if(_SelectedSlotListItem != 0)
	{
		assert(_SelectedSlotListItem->GetSlot().IsValid() == true);
		MountButton->SetEnabled((_SelectedSlotListItem->GetSlot()->GetMountedObject() == nullptr) && (_SelectedAccessoryListItem != 0) && (_SelectedSlotListItem->GetSlot()->GetSlotClass()->AcceptsSlotClassIdentifier(_SelectedAccessoryListItem->GetAccessory()->GetAspectAccessory()->GetSlotClassIdentifier()) == true));
	}
	else
	{
		MountButton->SetEnabled(false);
	}
}

void UI::OutfitShipDialog::_OnUnmountButtonUpdating(UI::Button * UnmountButton, float RealTimeSeconds, float GameTimeSeconds)
{
	if(_SelectedSlotListItem != 0)
	{
		assert(_SelectedSlotListItem->GetSlot().IsValid() == true);
		if(_SelectedSlotListItem->GetSlot()->GetMountedObject() != nullptr)
		{
			assert(_SelectedSlotListItem->GetSlot()->GetMountedObject()->GetAspectPhysical() != 0);
			UnmountButton->SetEnabled((_Ship->GetCargoHold()->GetSpace() >= _SelectedSlotListItem->GetSlot()->GetMountedObject()->GetAspectPhysical()->GetSpaceRequirement()));
		}
		else
		{
			UnmountButton->SetEnabled(false);
		}
	}
	else
	{
		UnmountButton->SetEnabled(false);
	}
}

void UI::OutfitShipDialog::_OnMountButtonClicked()
{
	assert(_SelectedAccessoryListItem != 0);
	assert(_SelectedSlotListItem != 0);
	
	Reference< Object > Accessory(_SelectedAccessoryListItem->GetAccessory());
	
	assert(Accessory.IsValid() == true);
	assert(_Ship.IsValid() == true);
	_Ship->GetCargoHold()->GetAspectObjectContainer()->RemoveContent(Accessory.Get());
	_Ship->GetAspectObjectContainer()->AddContent(Accessory.Get());
	_SelectedSlotListItem->GetSlot()->Mount(Accessory.Get());
	_SelectedSlotListItem->Update();
	_RebuildAccessoryList();
}

void UI::OutfitShipDialog::_OnOKButtonClicked()
{
	Destroy();
}

void UI::OutfitShipDialog::_OnUnmountButtonClicked()
{
	assert(_SelectedSlotListItem != 0);
	
	Object * Accessory(_SelectedSlotListItem->GetSlot()->GetMountedObject());
	
	assert(_Ship.IsValid() == true);
	assert(Accessory != nullptr);
	if(Accessory->GetAspectPhysical() != 0)
	{
		assert(_Ship->GetCargoHold()->GetSpace() >= Accessory->GetAspectPhysical()->GetSpaceRequirement());
	}
	_SelectedSlotListItem->GetSlot()->Unmount();
	_Ship->GetAspectObjectContainer()->RemoveContent(Accessory);
	_Ship->GetCargoHold()->GetAspectObjectContainer()->AddContent(Accessory);
	_SelectedSlotListItem->Update();
	_RebuildAccessoryList();
}

bool UI::OutfitShipDialog::_OnKey(const KeyEventInformation & KeyEventInformation)
{
	if(((KeyEventInformation.GetKeyCode() == 9 /* ESCAPE */) || (KeyEventInformation.GetKeyCode() == 36 /* RETURN */) || (KeyEventInformation.GetKeyCode() == 25 /* W */)) && (KeyEventInformation.IsDown() == true))
	{
		Destroy();
	}
	
	// eat all input
	return true;
}

bool UI::OutfitShipDialog::_OnSlotListItemMouseButton(UI::SlotListItem * SlotListItem, int Button, int State, float X, float Y)
{
	if((Button == 1 /* LEFT */) && (State == EV_DOWN))
	{
		if(_SelectedSlotListItem != 0)
		{
			_SelectedSlotListItem->SetSelected(false);
		}
		_SelectedSlotListItem = SlotListItem;
		_SelectedSlotListItem->SetSelected(true);
		
		return true;
	}
	
	return false;
}

bool UI::OutfitShipDialog::_OnAccessoryListItemMouseButton(UI::AccessoryListItem * AccessoryListItem, int Button, int State, float X, float Y)
{
	if((Button == 1 /* LEFT */) && (State == EV_DOWN))
	{
		if(_SelectedAccessoryListItem != 0)
		{
			_SelectedAccessoryListItem->SetSelected(false);
		}
		_SelectedAccessoryListItem = AccessoryListItem;
		_SelectedAccessoryListItem->SetSelected(true);
		
		return true;
	}
	
	return false;
}

void UI::OutfitShipDialog::_OnSizeChanged(void)
{
	float AvailableWidth(GetSize()[0]);
	
	// substract 10.0f for each border: left of left, between left and center, between center and right, right of right
	AvailableWidth -= 10.0f + 10.0f + 10.0f + 10.0f;
	AvailableWidth -= _CenterPane->GetSize()[0];
	AvailableWidth /= 2.0f;
	_LeftPane->SetSize(Vector2f(AvailableWidth, _LeftPane->GetSize()[1]));
	_CenterPane->SetPosition(Vector2f(10.0f + _LeftPane->GetSize()[0] + 10.0f, _CenterPane->GetPosition()[1]));
	_RightPane->SetSize(Vector2f(AvailableWidth, _RightPane->GetSize()[1]));
	_RightPane->SetPosition(Vector2f(10.0f + _LeftPane->GetSize()[0] + 10.0f + _CenterPane->GetSize()[0] + 10.0f, _RightPane->GetPosition()[1]));
}
