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
#include "../globals.h"
#include "../graphics/color_rgbo.h"
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
#include "key_event.h"
#include "label.h"
#include "list_box_item.h"
#include "mouse_button_event.h"
#include "outfit_ship_dialog.h"
#include "scroll_box.h"
#include "text_button.h"

namespace UI
{
	class AccessoryListItem : public UI::ListBoxItem
	{
	public:
		AccessoryListItem(UI::Widget * SupWidget, Object * Accessory);
		virtual ~AccessoryListItem(void);
		void Update(void);
		// getters
		Object * GetAccessory(void);
		// setters
	private:
		// callbacks
		void _OnAccessoryDestroying(void);
		// member variables
		Object * _Accessory;
		Connection _AccessoryDestroyingConnection;
	};
	
	class SlotListItem : public UI::ListBoxItem
	{
	public:
		SlotListItem(UI::Widget * SupWidget, Slot * Slot);
		virtual ~SlotListItem(void);
		void Update(void);
		// getters
		Slot * GetSlot(void);
		// setters
	private:
		// callbacks
		void _OnSlotDestroying(void);
		// member variables
		Slot * _Slot;
		Connection _SlotDestroyingConnection;
		UI::Label * _TypeOrWeaponLabel;
	};
}

UI::AccessoryListItem::AccessoryListItem(UI::Widget * SupWidget, Object * Accessory) :
	UI::ListBoxItem(SupWidget),
	_Accessory(Accessory)
{
	assert(_Accessory != nullptr);
	_AccessoryDestroyingConnection = _Accessory->ConnectDestroyingCallback(std::bind(&UI::AccessoryListItem::_OnAccessoryDestroying, this));
	// safe-guard: only accept objects with a name aspect
	assert(Accessory->GetAspectName() != nullptr);
	// safe-guard: only accept objects with a accessory aspect
	assert(Accessory->GetAspectAccessory() != nullptr);
	
	auto NameLabel{new UI::Label(this, Accessory->GetAspectName()->GetName())};
	
	NameLabel->SetLeft(5.0f);
	NameLabel->SetTop(5.0f);
	NameLabel->SetSize(Vector2f(GetWidth() - 10.0f, 20.0f));
	NameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	NameLabel->SetAnchorLeft(true);
	NameLabel->SetAnchorRight(true);
	NameLabel->SetAnchorTop(true);
	
	auto SlotTypeLabel{new UI::Label(this, g_SlotClassManager->Get(Accessory->GetAspectAccessory()->GetSlotClassIdentifier())->GetName())};
	
	SlotTypeLabel->SetLeft(25.0f);
	SlotTypeLabel->SetTop(25.0f);
	SlotTypeLabel->SetSize(Vector2f(GetWidth() - 30.0f, 20.0f));
	SlotTypeLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	SlotTypeLabel->SetAnchorLeft(true);
	SlotTypeLabel->SetAnchorRight(true);
	SlotTypeLabel->SetAnchorTop(true);
}

UI::AccessoryListItem::~AccessoryListItem(void)
{
	if(_Accessory != nullptr)
	{
		assert(_AccessoryDestroyingConnection.IsValid() == true);
		_Accessory->DisconnectDestroyingCallback(_AccessoryDestroyingConnection);
		assert(_AccessoryDestroyingConnection.IsValid() == false);
		_Accessory = nullptr;
	}
}

Object * UI::AccessoryListItem::GetAccessory(void)
{
	return _Accessory;
}

void UI::AccessoryListItem::_OnAccessoryDestroying(void)
{
	assert(_Accessory != nullptr);
	assert(_AccessoryDestroyingConnection.IsValid() == true);
	_Accessory->DisconnectDestroyingCallback(_AccessoryDestroyingConnection);
	assert(_AccessoryDestroyingConnection.IsValid() == false);
	_Accessory = nullptr;
}

UI::SlotListItem::SlotListItem(UI::Widget * SupWidget, Slot * Slot) :
	UI::ListBoxItem(SupWidget),
	_Slot(Slot)
{
	assert(_Slot != nullptr);
	_SlotDestroyingConnection = _Slot->ConnectDestroyingCallback(std::bind(&UI::SlotListItem::_OnSlotDestroying, this));
	
	auto IdentifierLabel{new UI::Label(this, _Slot->GetName())};
	
	IdentifierLabel->SetLeft(5.0f);
	IdentifierLabel->SetTop(5.0f);
	IdentifierLabel->SetSize(Vector2f(GetWidth() - 10.0f, 20.0f));
	IdentifierLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	IdentifierLabel->SetAnchorLeft(true);
	IdentifierLabel->SetAnchorRight(true);
	IdentifierLabel->SetAnchorTop(true);
	_TypeOrWeaponLabel = new UI::Label(this);
	_TypeOrWeaponLabel->SetLeft(25.0f);
	_TypeOrWeaponLabel->SetTop(25.0f);
	_TypeOrWeaponLabel->SetSize(Vector2f(GetWidth() - 30.0f, 20.0f));
	_TypeOrWeaponLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	_TypeOrWeaponLabel->SetAnchorLeft(true);
	_TypeOrWeaponLabel->SetAnchorRight(true);
	_TypeOrWeaponLabel->SetAnchorTop(true);
	Update();
}

UI::SlotListItem::~SlotListItem(void)
{
	if(_Slot != nullptr)
	{
		assert(_SlotDestroyingConnection.IsValid() == true);
		_Slot->DisconnectDestroyingCallback(_SlotDestroyingConnection);
		assert(_SlotDestroyingConnection.IsValid() == false);
		_Slot = nullptr;
	}
}

void UI::SlotListItem::Update(void)
{
	if(_Slot != nullptr)
	{
		Object * MountedObject(_Slot->GetMountedObject());
		
		if(MountedObject != nullptr)
		{
			assert(MountedObject->GetAspectName() != nullptr);
			_TypeOrWeaponLabel->SetText(MountedObject->GetAspectName()->GetName());
			_TypeOrWeaponLabel->SetTextColor(Graphics::ColorRGBO(0.6f, 0.8f, 0.6f, 1.0f));
		}
		else
		{
			assert(_Slot->GetSlotClass() != nullptr);
			_TypeOrWeaponLabel->SetText(_Slot->GetSlotClass()->GetName());
			_TypeOrWeaponLabel->SetTextColor(Graphics::ColorRGBO(0.8f, 0.6f, 0.6f, 1.0f));
		}
	}
	else
	{
		_TypeOrWeaponLabel->SetText("<slot destroyed>");
		_TypeOrWeaponLabel->SetTextColor(Graphics::ColorRGBO(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

Slot * UI::SlotListItem::GetSlot(void)
{
	return _Slot;
}

void UI::SlotListItem::_OnSlotDestroying(void)
{
	assert(_Slot != nullptr);
	assert(_SlotDestroyingConnection.IsValid() == true);
	_Slot->DisconnectDestroyingCallback(_SlotDestroyingConnection);
	assert(_SlotDestroyingConnection.IsValid() == false);
	_Slot = nullptr;
}

UI::OutfitShipDialog::OutfitShipDialog(UI::Widget * SupWidget, Ship * Ship) :
	UI::Window(SupWidget, "Outfit Ship"),
	_SelectedAccessoryListItem(nullptr),
	_SelectedSlotListItem(nullptr),
	_Ship(Ship)
{
	assert(_Ship != nullptr);
	_ShipDestroyingConnection = _Ship->ConnectDestroyingCallback(std::bind(&UI::OutfitShipDialog::_OnShipDestroying, this));
	ConnectDestroyingCallback(std::bind(&UI::OutfitShipDialog::_OnDestroying, this, std::placeholders::_1));
	ConnectSizeChangedCallback(std::bind(&UI::OutfitShipDialog::_OnSizeChanged, this, std::placeholders::_1));
	ConnectKeyCallback(std::bind(&UI::OutfitShipDialog::_OnKey, this, std::placeholders::_1));
	_LeftPane = new UI::Widget(this);
	_LeftPane->SetLeft(10.0f);
	_LeftPane->SetTop(40.0f);
	_LeftPane->SetSize(Vector2f(200.0f, GetHeight() - 50.0f));
	_LeftPane->SetAnchorBottom(true);
	
	auto SlotListLabel{new UI::Label(_LeftPane, "Slots")};
	
	SlotListLabel->SetLeft(0.0f);
	SlotListLabel->SetTop(0.0f);
	SlotListLabel->SetSize(Vector2f(_LeftPane->GetWidth(), 20.0f));
	SlotListLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	SlotListLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	SlotListLabel->SetAnchorRight(true);
	_SlotScrollBox = new UI::ScrollBox(_LeftPane);
	_SlotScrollBox->SetLeft(0.0f);
	_SlotScrollBox->SetTop(30.0f);
	_SlotScrollBox->SetSize(Vector2f(_LeftPane->GetWidth(), _LeftPane->GetHeight() - 30.0f));
	_SlotScrollBox->SetHorizontalScrollBarVisible(false);
	_SlotScrollBox->SetAnchorBottom(true);
	_SlotScrollBox->SetAnchorRight(true);
	
	float Top(5.0f);
	const std::map< std::string, Slot * > & Slots();
	
	for(auto SlotPair : _Ship->GetAspectOutfitting()->GetSlots())
	{
		auto NewSlotListItem(new SlotListItem(_SlotScrollBox->GetContent(), SlotPair.second));
		
		NewSlotListItem->SetLeft(5.0f);
		NewSlotListItem->SetTop(Top);
		NewSlotListItem->SetSize(Vector2f(_SlotScrollBox->GetContent()->GetSize()[0] - 10.0f, 50.0f));
		NewSlotListItem->SetAnchorRight(true);
		NewSlotListItem->ConnectMouseButtonCallback(std::bind(&UI::OutfitShipDialog::_OnSlotListItemMouseButton, this, NewSlotListItem, std::placeholders::_1));
		Top += 55.0f;
	}
	_SlotScrollBox->GetContent()->SetSize(Vector2f(180.0f, std::max(Top, _SlotScrollBox->GetView()->GetSize()[1])));
	_SlotScrollBox->GetContent()->SetAnchorRight(true);
	// center pane
	_CenterPane = new UI::Widget(this);
	_CenterPane->SetLeft(10.0f + _LeftPane->GetSize()[0] + 10.0f);
	_CenterPane->SetTop(70.0f);
	_CenterPane->SetSize(Vector2f(160.0f, GetSize()[1] - 80.0f));
	_CenterPane->SetAnchorBottom(true);
	
	auto MountButton{new UI::TextButton(_CenterPane, "Mount")};
	
	MountButton->SetLeft(0.0f);
	MountButton->SetTop(40.0f);
	MountButton->SetSize(Vector2f(_CenterPane->GetSize()[0], 20.0f));
	MountButton->SetAnchorRight(true);
	MountButton->ConnectClickedCallback(std::bind(&UI::OutfitShipDialog::_OnMountButtonClicked, this));
	MountButton->ConnectUpdatingCallback(std::bind(&UI::OutfitShipDialog::_OnMountButtonUpdating, this, MountButton, std::placeholders::_1, std::placeholders::_2));
	
	auto UnmountButton{new UI::TextButton(_CenterPane, "Unmount")};
	
	UnmountButton->SetLeft(0.0f);
	UnmountButton->SetTop(70.0f);
	UnmountButton->SetSize(Vector2f(_CenterPane->GetSize()[0], 20.0f));
	UnmountButton->SetAnchorRight(true);
	UnmountButton->ConnectClickedCallback(std::bind(&UI::OutfitShipDialog::_OnUnmountButtonClicked, this));
	UnmountButton->ConnectUpdatingCallback(std::bind(&UI::OutfitShipDialog::_OnUnmountButtonUpdating, this, UnmountButton, std::placeholders::_1, std::placeholders::_2));
	
	auto OKButton{new UI::TextButton(_CenterPane, "OK")};
	
	OKButton->SetLeft(0.0f);
	OKButton->SetTop(_CenterPane->GetSize()[1] - 30.0f);
	OKButton->SetSize(Vector2f(_CenterPane->GetSize()[0], 20.0f));
	OKButton->SetAnchorBottom(true);
	OKButton->SetAnchorRight(true);
	OKButton->SetAnchorTop(false);
	OKButton->ConnectClickedCallback(std::bind(&UI::OutfitShipDialog::_OnOKButtonClicked, this));
	// right pane
	_RightPane = new UI::Widget(this);
	_RightPane->SetLeft(10.0f + _LeftPane->GetSize()[0] + 10.0f + _CenterPane->GetSize()[0] + 10.0f);
	_RightPane->SetTop(40.0f);
	_RightPane->SetSize(Vector2f(200.0f, GetSize()[1] - 50.0f));
	_RightPane->SetAnchorBottom(true);
	
	auto AccessoryListLabel{new UI::Label(_RightPane, "Accessories")};
	
	AccessoryListLabel->SetLeft(0.0f);
	AccessoryListLabel->SetTop(0.0f);
	AccessoryListLabel->SetSize(Vector2f(_RightPane->GetSize()[0], 20.0f));
	AccessoryListLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	AccessoryListLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	AccessoryListLabel->SetAnchorRight(true);
	_AccessoryScrollBox = new UI::ScrollBox(_RightPane);
	_AccessoryScrollBox->SetLeft(0.0f);
	_AccessoryScrollBox->SetTop(30.0f);
	_AccessoryScrollBox->SetSize(Vector2f(_RightPane->GetSize()[0], _RightPane->GetSize()[1] - 30.0f));
	_AccessoryScrollBox->SetHorizontalScrollBarVisible(false);
	_AccessoryScrollBox->SetAnchorBottom(true);
	_AccessoryScrollBox->SetAnchorRight(true);
	_RebuildAccessoryList();
}

void UI::OutfitShipDialog::_RebuildAccessoryList(void)
{
	// empty the weapon list first
	// save the selected weapon so we can reselect it if it is available after the rebuild
	Object * SelectedAccessory(nullptr);
	
	if(_SelectedAccessoryListItem != nullptr)
	{
		SelectedAccessory = _SelectedAccessoryListItem->GetAccessory();
		_SelectedAccessoryListItem = nullptr;
	}
	while(_AccessoryScrollBox->GetContent()->GetSubWidgets().empty() == false)
	{
		_AccessoryScrollBox->GetContent()->GetSubWidgets().front()->Destroy();
	}
	// now fill the weapon list
	assert(_Ship != nullptr);
	assert(_Ship->GetCargoHold() != nullptr);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	
	float Top(5.0f);
	
	for(auto Content : _Ship->GetCargoHold()->GetAspectObjectContainer()->GetContent())
	{
		if((Content->GetAspectAccessory() != nullptr) && (Content->GetAspectAccessory()->GetSlot() == nullptr))
		{
			auto NewAccessoryListItem{new UI::AccessoryListItem(_AccessoryScrollBox->GetContent(), Content)};
			
			NewAccessoryListItem->SetLeft(5.0f);
			NewAccessoryListItem->SetTop(Top);
			NewAccessoryListItem->SetSize(Vector2f(_AccessoryScrollBox->GetContent()->GetSize()[0] - 10.0f, 50.0f));
			NewAccessoryListItem->SetAnchorRight(true);
			NewAccessoryListItem->ConnectMouseButtonCallback(std::bind(&UI::OutfitShipDialog::_OnAccessoryListItemMouseButton, this, NewAccessoryListItem, std::placeholders::_1));
			if(Content== SelectedAccessory)
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

void UI::OutfitShipDialog::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if(_Ship != nullptr)
		{
			_Ship = nullptr;
			assert(_ShipDestroyingConnection.IsValid() == true);
			_ShipDestroyingConnection.Disconnect();
		}
		assert(_ShipDestroyingConnection.IsValid() == false);
	}
}

void UI::OutfitShipDialog::_OnMountButtonUpdating(UI::Button * MountButton, float RealTimeSeconds, float GameTimeSeconds)
{
	if(_SelectedSlotListItem != nullptr)
	{
		assert(_SelectedSlotListItem->GetSlot() != nullptr);
		MountButton->SetEnabled((_SelectedSlotListItem->GetSlot()->GetMountedObject() == nullptr) && (_SelectedAccessoryListItem != 0) && (_SelectedSlotListItem->GetSlot()->GetSlotClass()->AcceptsSlotClassIdentifier(_SelectedAccessoryListItem->GetAccessory()->GetAspectAccessory()->GetSlotClassIdentifier()) == true));
	}
	else
	{
		MountButton->SetEnabled(false);
	}
}

void UI::OutfitShipDialog::_OnUnmountButtonUpdating(UI::Button * UnmountButton, float RealTimeSeconds, float GameTimeSeconds)
{
	if(_SelectedSlotListItem != nullptr)
	{
		assert(_SelectedSlotListItem->GetSlot() != nullptr);
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

void UI::OutfitShipDialog::_OnMountButtonClicked(void)
{
	assert(_SelectedAccessoryListItem != nullptr);
	assert(_SelectedSlotListItem != nullptr);
	
	auto Accessory{_SelectedAccessoryListItem->GetAccessory()};
	
	assert(Accessory != nullptr);
	assert(_Ship != nullptr);
	_Ship->GetCargoHold()->GetAspectObjectContainer()->RemoveContent(Accessory);
	_Ship->GetAspectObjectContainer()->AddContent(Accessory);
	_SelectedSlotListItem->GetSlot()->Mount(Accessory);
	_SelectedSlotListItem->Update();
	_RebuildAccessoryList();
}

void UI::OutfitShipDialog::_OnOKButtonClicked(void)
{
	Destroy();
}

void UI::OutfitShipDialog::_OnUnmountButtonClicked(void)
{
	assert(_SelectedSlotListItem != nullptr);
	assert(_SelectedSlotListItem->GetSlot() != nullptr);
	
	auto Accessory{_SelectedSlotListItem->GetSlot()->GetMountedObject()};
	
	assert(Accessory != nullptr);
	assert(_Ship != nullptr);
	assert(_Ship->GetCargoHold() != nullptr);
	if(Accessory->GetAspectPhysical() != nullptr)
	{
		assert(Accessory->GetAspectPhysical() != nullptr);
		assert(_Ship->GetCargoHold()->GetSpace() >= Accessory->GetAspectPhysical()->GetSpaceRequirement());
	}
	_SelectedSlotListItem->GetSlot()->Unmount();
	assert(_Ship->GetAspectObjectContainer() != nullptr);
	_Ship->GetAspectObjectContainer()->RemoveContent(Accessory);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	_Ship->GetCargoHold()->GetAspectObjectContainer()->AddContent(Accessory);
	_SelectedSlotListItem->Update();
	_RebuildAccessoryList();
}

void UI::OutfitShipDialog::_OnKey(UI::KeyEvent & KeyEvent)
{
	if(((KeyEvent.GetKeyCode() == 9 /* ESCAPE */) || (KeyEvent.GetKeyCode() == 36 /* RETURN */) || (KeyEvent.GetKeyCode() == 25 /* W */)) && (KeyEvent.IsDown() == true))
	{
		Destroy();
	}
}

void UI::OutfitShipDialog::_OnShipDestroying(void)
{
	assert(_Ship != nullptr);
	_Ship = nullptr;
	assert(_ShipDestroyingConnection.IsValid() == true);
	_ShipDestroyingConnection.Disconnect();
	assert(_ShipDestroyingConnection.IsValid() == false);
}

void UI::OutfitShipDialog::_OnSlotListItemMouseButton(UI::SlotListItem * SlotListItem, UI::MouseButtonEvent & MouseButtonEvent)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedSlotListItem != nullptr)
		{
			_SelectedSlotListItem->SetSelected(false);
		}
		_SelectedSlotListItem = SlotListItem;
		_SelectedSlotListItem->SetSelected(true);
	}
}

void UI::OutfitShipDialog::_OnAccessoryListItemMouseButton(UI::AccessoryListItem * AccessoryListItem, UI::MouseButtonEvent & MouseButtonEvent)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedAccessoryListItem != nullptr)
		{
			_SelectedAccessoryListItem->SetSelected(false);
		}
		_SelectedAccessoryListItem = AccessoryListItem;
		_SelectedAccessoryListItem->SetSelected(true);
	}
}

void UI::OutfitShipDialog::_OnSizeChanged(UI::Event & SizeChangedEvent)
{
	if(SizeChangedEvent.GetPhase() == UI::Event::Phase::Target)
	{
		float AvailableWidth(GetSize()[0]);
		
		// substract 10.0f for each border: left of left, between left and center, between center and right, right of right
		AvailableWidth -= 10.0f + 10.0f + 10.0f + 10.0f;
		AvailableWidth -= _CenterPane->GetSize()[0];
		AvailableWidth /= 2.0f;
		_LeftPane->SetSize(Vector2f(AvailableWidth, _LeftPane->GetSize()[1]));
		_CenterPane->SetLeft(10.0f + _LeftPane->GetSize()[0] + 10.0f);
		_RightPane->SetSize(Vector2f(AvailableWidth, _RightPane->GetSize()[1]));
		_RightPane->SetLeft(10.0f + _LeftPane->GetSize()[0] + 10.0f + _CenterPane->GetSize()[0] + 10.0f);
	}
}
