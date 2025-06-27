/**
 * galactic-fall
 * Copyright (C) 2007-2019  Hagen Möbius
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

#include <graphics/color_rgbo.h>

#include "../class_manager.h"
#include "../globals.h"
#include "../object_aspect_accessory.h"
#include "../object_aspect_name.h"
#include "../object_aspect_object_container.h"
#include "../object_aspect_outfitting.h"
#include "../object_aspect_physical.h"
#include "../ship.h"
#include "../slot.h"
#include "../slot_class.h"
#include "../storage.h"
#include "key_event.h"
#include "label.h"
#include "list_box.h"
#include "list_box_item.h"
#include "outfit_ship_dialog.h"
#include "text_button.h"

using namespace Expressions::Operators;

namespace UI
{
	class AccessoryListItem : public UI::ListBoxItem
	{
	public:
		AccessoryListItem(Object * Accessory);
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
		SlotListItem(Slot * Slot);
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

UI::AccessoryListItem::AccessoryListItem(Object * Accessory) :
	_Accessory(Accessory)
{
	assert(_Accessory != nullptr);
	_AccessoryDestroyingConnection = _Accessory->ConnectDestroyingCallback(std::bind(&UI::AccessoryListItem::_OnAccessoryDestroying, this));
	// safe-guard: only accept objects with a name aspect
	assert(Accessory->GetAspectName() != nullptr);
	// safe-guard: only accept objects with a accessory aspect
	assert(Accessory->GetAspectAccessory() != nullptr);
	
	// create components
	auto NameLabel{new UI::Label{}};
	auto SlotTypeLabel{new UI::Label{}};
	
	// initialize components
	NameLabel->SetLeft(5.0_c);
	NameLabel->SetTop(5.0_c);
	NameLabel->SetWidth(width(this) - 5.0_c - left(NameLabel));
	NameLabel->SetHeight(20.0_c);
	NameLabel->SetText(Accessory->GetAspectName()->GetName());
	NameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	SlotTypeLabel->SetLeft(25.0_c);
	SlotTypeLabel->SetTop(bottom(NameLabel));
	SlotTypeLabel->SetWidth(width(this) - 5.0_c - left(SlotTypeLabel));
	SlotTypeLabel->SetHeight(20.0_c);
	SlotTypeLabel->SetText(g_SlotClassManager->Get(Accessory->GetAspectAccessory()->GetSlotClassIdentifier())->GetName());
	SlotTypeLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	// add components
	AddSubWidget(NameLabel);
	AddSubWidget(SlotTypeLabel);
	SetHeight(bottom(SlotTypeLabel) + 5.0_c);
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

UI::SlotListItem::SlotListItem(Slot * Slot) :
	_Slot(Slot)
{
	assert(_Slot != nullptr);
	_SlotDestroyingConnection = _Slot->ConnectDestroyingCallback(std::bind(&UI::SlotListItem::_OnSlotDestroying, this));
	
	// create components
	auto IdentifierLabel{new UI::Label{}};
	
	_TypeOrWeaponLabel = new UI::Label{};
	// initialize components
	IdentifierLabel->SetLeft(5.0_c);
	IdentifierLabel->SetTop(5.0_c);
	IdentifierLabel->SetWidth(width(this) - 5.0_c - left(IdentifierLabel));
	IdentifierLabel->SetHeight(20.0_c);
	IdentifierLabel->SetText(_Slot->GetName());
	IdentifierLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_TypeOrWeaponLabel->SetLeft(25.0_c);
	_TypeOrWeaponLabel->SetTop(bottom(IdentifierLabel));
	_TypeOrWeaponLabel->SetWidth(width(this) - 5.0_c - left(_TypeOrWeaponLabel));
	_TypeOrWeaponLabel->SetHeight(20.0_c);
	_TypeOrWeaponLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	// add components
	AddSubWidget(IdentifierLabel);
	AddSubWidget(_TypeOrWeaponLabel);
	SetHeight(bottom(_TypeOrWeaponLabel) + 5.0_c);
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
		auto MountedObject(_Slot->GetMountedObject());
		
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

UI::OutfitShipDialog::OutfitShipDialog(Ship * Ship) :
	_Ship(Ship)
{
	SetTitle("Outfit Ship");
	assert(_Ship != nullptr);
	_ShipDestroyingConnection = _Ship->ConnectDestroyingCallback(std::bind(&UI::OutfitShipDialog::_OnShipDestroying, this));
	ConnectDestroyingCallback(std::bind(&UI::OutfitShipDialog::_OnDestroying, this, std::placeholders::_1));
	ConnectKeyCallback(std::bind(&UI::OutfitShipDialog::_OnKey, this, std::placeholders::_1));
	
	// create components
	auto AccessoryListLabel{new UI::Label{}};
	auto LeftPane{new UI::Widget{}};
	auto CenterPane{new UI::Widget{}};
	auto MountButton{new UI::TextButton{}};
	auto OKButton{new UI::TextButton{}};
	auto RightPane{new UI::Widget{}};
	auto SlotListLabel{new UI::Label{}};
	auto UnmountButton{new UI::TextButton{}};
	
	_AccessoryListBox = new UI::ListBox{};
	_SlotListBox = new UI::ListBox{};
	// initialize components
	LeftPane->SetLeft(10.0_c);
	LeftPane->SetTop(40.0_c);
	LeftPane->SetHeight(height(this) - 10.0_c - top(LeftPane));
	LeftPane->SetWidth((width(this) - 10.0_c - 10.0_c - width(CenterPane) - 10.0_c - 10.0_c) / 2.0_c);
	SlotListLabel->SetLeft(0.0_c);
	SlotListLabel->SetTop(0.0_c);
	SlotListLabel->SetWidth(width(LeftPane));
	SlotListLabel->SetHeight(20.0_c);
	SlotListLabel->SetText("Slots");
	SlotListLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	SlotListLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_SlotListBox->SetLeft(0.0_c);
	_SlotListBox->SetTop(bottom(SlotListLabel));
	_SlotListBox->SetWidth(width(LeftPane));
	_SlotListBox->SetHeight(height(LeftPane) - top(_SlotListBox));
	_SlotListBox->SetHorizontalScrollBarVisible(false);
	for(auto SlotPair : _Ship->GetAspectOutfitting()->GetSlots())
	{
		_SlotListBox->GetContent()->AddSubWidget(new UI::SlotListItem{SlotPair.second});
	}
	CenterPane->SetLeft(right(LeftPane) + 10.0_c);
	CenterPane->SetTop(70.0_c);
	CenterPane->SetWidth(160.0_c);
	CenterPane->SetHeight(height(this) - 10.0_c - top(CenterPane));
	MountButton->SetLeft(0.0_c);
	MountButton->SetTop(40.0_c);
	MountButton->SetWidth(width(CenterPane));
	MountButton->SetHeight(20.0_c);
	MountButton->SetText("Mount");
	MountButton->ConnectClickedCallback(std::bind(&UI::OutfitShipDialog::_OnMountButtonClicked, this));
	MountButton->ConnectUpdatingCallback(std::bind(&UI::OutfitShipDialog::_OnMountButtonUpdating, this, MountButton, std::placeholders::_1, std::placeholders::_2));
	UnmountButton->SetLeft(0.0_c);
	UnmountButton->SetTop(bottom(MountButton) + 10.0_c);
	UnmountButton->SetWidth(width(CenterPane));
	UnmountButton->SetHeight(20.0_c);
	UnmountButton->SetText("Unmount");
	UnmountButton->ConnectClickedCallback(std::bind(&UI::OutfitShipDialog::_OnUnmountButtonClicked, this));
	UnmountButton->ConnectUpdatingCallback(std::bind(&UI::OutfitShipDialog::_OnUnmountButtonUpdating, this, UnmountButton, std::placeholders::_1, std::placeholders::_2));
	OKButton->SetLeft(0.0_c);
	OKButton->SetTop(height(CenterPane) - 10.0_c - height(OKButton));
	OKButton->SetWidth(width(CenterPane));
	OKButton->SetHeight(20.0_c);
	OKButton->SetText("OK");
	OKButton->ConnectClickedCallback(std::bind(&UI::OutfitShipDialog::_OnOKButtonClicked, this));
	RightPane->SetLeft(right(CenterPane) + 10.0_c);
	RightPane->SetTop(40.0_c);
	RightPane->SetHeight(height(this) - 10.0_c - top(RightPane));
	RightPane->SetWidth((width(this) - 10.0_c - 10.0_c - width(CenterPane) - 10.0_c - 10.0_c) / 2.0_c);
	AccessoryListLabel->SetLeft(0.0_c);
	AccessoryListLabel->SetTop(0.0_c);
	AccessoryListLabel->SetWidth(width(RightPane));
	AccessoryListLabel->SetHeight(20.0_c);
	AccessoryListLabel->SetText("Accessories");
	AccessoryListLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	AccessoryListLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_AccessoryListBox->SetLeft(0.0_c);
	_AccessoryListBox->SetTop(bottom(AccessoryListLabel));
	_AccessoryListBox->SetWidth(width(RightPane));
	_AccessoryListBox->SetHeight(height(RightPane) - top(_AccessoryListBox));
	_AccessoryListBox->SetHorizontalScrollBarVisible(false);
	//add components
	AddSubWidget(CenterPane);
	CenterPane->AddSubWidget(MountButton);
	CenterPane->AddSubWidget(UnmountButton);
	CenterPane->AddSubWidget(OKButton);
	AddSubWidget(LeftPane);
	LeftPane->AddSubWidget(_SlotListBox);
	LeftPane->AddSubWidget(SlotListLabel);
	AddSubWidget(RightPane);
	RightPane->AddSubWidget(_AccessoryListBox);
	RightPane->AddSubWidget(AccessoryListLabel);
	// initialize content
	_RebuildAccessoryList();
}

void UI::OutfitShipDialog::_RebuildAccessoryList(void)
{
	// empty the weapon list first
	// save the selected weapon so we can reselect it if it is available after the rebuild
	Object * SelectedAccessory(nullptr);
	
	if(_AccessoryListBox->GetSelectedItem() != nullptr)
	{
		SelectedAccessory = dynamic_cast< UI::AccessoryListItem * >(_AccessoryListBox->GetSelectedItem())->GetAccessory();
	}
	_AccessoryListBox->Clear();
	// now fill the weapon list
	assert(_Ship != nullptr);
	assert(_Ship->GetCargoHold() != nullptr);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	for(auto Content : _Ship->GetCargoHold()->GetAspectObjectContainer()->GetContent())
	{
		if((Content->GetAspectAccessory() != nullptr) && (Content->GetAspectAccessory()->GetSlot() == nullptr))
		{
			auto AccessoryListItem{new UI::AccessoryListItem{Content}};
			
			_AccessoryListBox->GetContent()->AddSubWidget(AccessoryListItem);
			if(Content == SelectedAccessory)
			{
				_AccessoryListBox->SetSelectedItem(AccessoryListItem);
			}
		}
	}
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

void UI::OutfitShipDialog::_OnMountButtonUpdating(UI::Button * MountButton, [[maybe_unused]] float RealTimeSeconds, [[maybe_unused]] float GameTimeSeconds)
{
	auto SelectedSlotListItem{dynamic_cast< UI::SlotListItem * >(_SlotListBox->GetSelectedItem())};
	
	if(SelectedSlotListItem != nullptr)
	{
		auto SelectedAccessoryListItem{dynamic_cast< UI::AccessoryListItem * >(_AccessoryListBox->GetSelectedItem())};
		
		assert(SelectedSlotListItem->GetSlot() != nullptr);
		MountButton->SetEnabled((SelectedSlotListItem->GetSlot()->GetMountedObject() == nullptr) && (SelectedAccessoryListItem != nullptr) && (SelectedSlotListItem->GetSlot()->GetSlotClass()->AcceptsSlotClassIdentifier(SelectedAccessoryListItem->GetAccessory()->GetAspectAccessory()->GetSlotClassIdentifier()) == true));
	}
	else
	{
		MountButton->SetEnabled(false);
	}
}

void UI::OutfitShipDialog::_OnUnmountButtonUpdating(UI::Button * UnmountButton, [[maybe_unused]] float RealTimeSeconds, [[maybe_unused]] float GameTimeSeconds)
{
	auto SelectedSlotListItem{dynamic_cast< UI::SlotListItem * >(_SlotListBox->GetSelectedItem())};
	
	if(SelectedSlotListItem != nullptr)
	{
		assert(SelectedSlotListItem->GetSlot() != nullptr);
		if(SelectedSlotListItem->GetSlot()->GetMountedObject() != nullptr)
		{
			assert(SelectedSlotListItem->GetSlot()->GetMountedObject()->GetAspectPhysical() != nullptr);
			UnmountButton->SetEnabled((_Ship->GetCargoHold()->GetSpace() >= SelectedSlotListItem->GetSlot()->GetMountedObject()->GetAspectPhysical()->GetSpaceRequirement()));
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
	auto SelectedSlotListItem{dynamic_cast< UI::SlotListItem * >(_SlotListBox->GetSelectedItem())};
	auto SelectedAccessoryListItem{dynamic_cast< UI::AccessoryListItem * >(_AccessoryListBox->GetSelectedItem())};
	
	assert(SelectedAccessoryListItem != nullptr);
	assert(SelectedSlotListItem != nullptr);
	
	auto Accessory{SelectedAccessoryListItem->GetAccessory()};
	
	assert(Accessory != nullptr);
	assert(_Ship != nullptr);
	_Ship->GetCargoHold()->GetAspectObjectContainer()->RemoveContent(Accessory);
	_Ship->GetAspectObjectContainer()->AddContent(Accessory);
	SelectedSlotListItem->GetSlot()->Mount(Accessory);
	SelectedSlotListItem->Update();
	_RebuildAccessoryList();
}

void UI::OutfitShipDialog::_OnOKButtonClicked(void)
{
	Destroy();
}

void UI::OutfitShipDialog::_OnUnmountButtonClicked(void)
{
	auto SelectedSlotListItem{dynamic_cast< UI::SlotListItem * >(_SlotListBox->GetSelectedItem())};
	
	assert(SelectedSlotListItem != nullptr);
	assert(SelectedSlotListItem->GetSlot() != nullptr);
	
	auto Accessory{SelectedSlotListItem->GetSlot()->GetMountedObject()};
	
	assert(Accessory != nullptr);
	assert(_Ship != nullptr);
	assert(_Ship->GetCargoHold() != nullptr);
	if(Accessory->GetAspectPhysical() != nullptr)
	{
		assert(Accessory->GetAspectPhysical() != nullptr);
		assert(_Ship->GetCargoHold()->GetSpace() >= Accessory->GetAspectPhysical()->GetSpaceRequirement());
	}
	SelectedSlotListItem->GetSlot()->Unmount();
	assert(_Ship->GetAspectObjectContainer() != nullptr);
	_Ship->GetAspectObjectContainer()->RemoveContent(Accessory);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	_Ship->GetCargoHold()->GetAspectObjectContainer()->AddContent(Accessory);
	SelectedSlotListItem->Update();
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
