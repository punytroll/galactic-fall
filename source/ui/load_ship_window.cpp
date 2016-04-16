/**
 * Copyright (C) 2014  Hagen Möbius
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

#include <assert.h>

#include <algorithm>

#include "../hangar.h"
#include "../object_aspect_name.h"
#include "../object_aspect_object_container.h"
#include "../object_aspect_physical.h"
#include "../ship.h"
#include "../storage.h"
#include "event.h"
#include "key_event.h"
#include "label.h"
#include "list_box.h"
#include "list_box_item.h"
#include "load_ship_window.h"
#include "mouse_button_event.h"
#include "sub_widget_event.h"
#include "text_button.h"

namespace UI
{
	class ObjectListItem : public UI::ListBoxItem
	{
	public:
		ObjectListItem(Object * Object) :
			UI::ListBoxItem(),
			_Object(Object)
		{
			assert(_Object != nullptr);
			ConnectDestroyingCallback(std::bind(&UI::ObjectListItem::_OnDestroying, this, std::placeholders::_1));
			_ObjectDestroyingConnection = _Object->ConnectDestroyingCallback(std::bind(&UI::ObjectListItem::_OnObjectDestroying, this));
			assert(_Object->GetAspectName() != nullptr);
			
			auto NameLabel(new UI::Label(this, _Object->GetAspectName()->GetName()));
			
			NameLabel->SetLeft(5.0f);
			NameLabel->SetTop(0.0f);
			NameLabel->SetSize(Vector2f(90.0f, 20.0f));
			NameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
			NameLabel->SetAnchorLeft(true);
			NameLabel->SetAnchorRight(true);
			NameLabel->SetAnchorTop(true);
		}

		Object * GetObject(void)
		{
			return _Object;
		}
	private:
		// event handlers
		void _OnDestroying(UI::Event & DestroyingEvent)
		{
			if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
			{
				assert(_Object != nullptr);
				_ObjectDestroyingConnection.Disconnect();
				_Object = nullptr;
			}
		}
		
		void _OnObjectDestroying(void)
		{
			Destroy();
		}
		// member variables
		Object * _Object;
		Connection _ObjectDestroyingConnection;
	};
}

UI::LoadShipWindow::LoadShipWindow(UI::Widget * SupWidget, Hangar * Hangar, Ship * Ship) :
	UI::Window(SupWidget, "Load Ship"),
	_Hangar(Hangar),
	_Ship(Ship)
{
	assert(_Hangar != nullptr);
	_HangarDestroyingCallbackConnection = _Hangar->ConnectDestroyingCallback(std::bind(&UI::LoadShipWindow::_OnHangarDestroying, this));
	assert(_Hangar->GetAspectObjectContainer() != nullptr);
	_HangarContentAddedCallbackConnection = _Hangar->GetAspectObjectContainer()->ConnectContentAddedCallback(std::bind(&UI::LoadShipWindow::_OnHangarContentAdded, this, std::placeholders::_1));
	_HangarContentRemovedCallbackConnection = _Hangar->GetAspectObjectContainer()->ConnectContentRemovedCallback(std::bind(&UI::LoadShipWindow::_OnHangarContentRemoved, this, std::placeholders::_1));
	assert(_Ship != nullptr);
	_ShipDestroyingCallbackConnection = _Ship->ConnectDestroyingCallback(std::bind(&UI::LoadShipWindow::_OnShipDestroying, this));
	assert(_Ship->GetCargoHold() != nullptr);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	_ShipContentAddedCallbackConnection = _Ship->GetCargoHold()->GetAspectObjectContainer()->ConnectContentAddedCallback(std::bind(&UI::LoadShipWindow::_OnShipContentAdded, this, std::placeholders::_1));
	_ShipContentRemovedCallbackConnection = _Ship->GetCargoHold()->GetAspectObjectContainer()->ConnectContentRemovedCallback(std::bind(&UI::LoadShipWindow::_OnShipContentRemoved, this, std::placeholders::_1));
	ConnectDestroyingCallback(std::bind(&UI::LoadShipWindow::_OnDestroying, this, std::placeholders::_1));
	ConnectKeyCallback(std::bind(&UI::LoadShipWindow::_OnKey, this, std::placeholders::_1));
	
	auto LeftPane(new UI::Widget(this));
	
	LeftPane->SetLeft(10.0f);
	LeftPane->SetTop(40.0f);
	LeftPane->SetSize(Vector2f(200.0f, GetSize()[1] - 50.0f));
	LeftPane->SetAnchorBottom(true);
	
	auto HangarLabel(new UI::Label(LeftPane, "Hangar"));
	
	HangarLabel->SetLeft(0.0f);
	HangarLabel->SetTop(0.0f);
	HangarLabel->SetSize(Vector2f(LeftPane->GetSize()[0], 20.0f));
	HangarLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	HangarLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	HangarLabel->SetAnchorRight(true);
	_HangarListBox = new UI::ListBox(LeftPane);
	_HangarListBox->SetLeft(0.0f);
	_HangarListBox->SetTop(30.0f);
	_HangarListBox->SetSize(Vector2f(LeftPane->GetSize()[0], LeftPane->GetSize()[1] - 30.0f));
	_HangarListBox->SetHorizontalScrollBarVisible(false);
	_HangarListBox->SetAnchorBottom(true);
	_HangarListBox->SetAnchorRight(true);
	_HangarListBox->GetContent()->SetSize(Vector2f(LeftPane->GetSize()[0] - 20.0f, 0.0f));
	_HangarListBox->GetContent()->SetAnchorRight(true);
	assert(_Hangar->GetAspectObjectContainer() != nullptr);
	for(auto HangarObject : _Hangar->GetAspectObjectContainer()->GetContent())
	{
		if(HangarObject->GetAspectName() != nullptr)
		{
			_HangarListBox->GetContent()->AddSubWidget(new ObjectListItem(HangarObject));
		}
	}
	
	auto CenterPane(new UI::Widget(this));
	
	CenterPane->SetLeft(10.0f + LeftPane->GetSize()[0] + 10.0f);
	CenterPane->SetTop(70.0f);
	CenterPane->SetSize(Vector2f(160.0f, GetSize()[1] - 80.0f));
	CenterPane->SetAnchorBottom(true);
	
	auto MoveToHangarButton(new UI::TextButton(CenterPane, "<<"));
	
	MoveToHangarButton->SetLeft(0.0f);
	MoveToHangarButton->SetTop(50.0f);
	MoveToHangarButton->SetSize(Vector2f(CenterPane->GetSize()[0], 20.0f));
	MoveToHangarButton->SetAnchorRight(true);
	MoveToHangarButton->ConnectClickedCallback(std::bind(&UI::LoadShipWindow::_OnMoveToHangarButtonClicked, this));
	MoveToHangarButton->ConnectUpdatingCallback(std::bind(&UI::LoadShipWindow::_OnMoveToHangarButtonUpdating, this, std::placeholders::_1, std::placeholders::_2, MoveToHangarButton));
	
	auto MoveToShipButton(new UI::TextButton(CenterPane, ">>"));
	
	MoveToShipButton->SetLeft(0.0f);
	MoveToShipButton->SetTop(80.0f);
	MoveToShipButton->SetSize(Vector2f(CenterPane->GetSize()[0], 20.0f));
	MoveToShipButton->SetAnchorRight(true);
	MoveToShipButton->ConnectClickedCallback(std::bind(&UI::LoadShipWindow::_OnMoveToShipButtonClicked, this));
	MoveToShipButton->ConnectUpdatingCallback(std::bind(&UI::LoadShipWindow::_OnMoveToShipButtonUpdating, this, std::placeholders::_1, std::placeholders::_2, MoveToShipButton));
	
	auto OKButton(new UI::TextButton(CenterPane, "OK"));
	
	OKButton->SetLeft(0.0f);
	OKButton->SetTop(CenterPane->GetSize()[1] - 30.0f);
	OKButton->SetSize(Vector2f(CenterPane->GetSize()[0], 20.0f));
	OKButton->SetAnchorBottom(true);
	OKButton->SetAnchorRight(true);
	OKButton->SetAnchorTop(false);
	OKButton->ConnectClickedCallback(std::bind(&UI::LoadShipWindow::_OnOKButtonClicked, this));
	
	auto RightPane(new UI::Widget(this));
	
	RightPane->SetLeft(10.0f + LeftPane->GetSize()[0] + 10.0f + CenterPane->GetSize()[0] + 10.0f);
	RightPane->SetTop(40.0f);
	RightPane->SetSize(Vector2f(200.0f, GetSize()[1] - 50.0f));
	RightPane->SetAnchorBottom(true);
	_ShipListBox = new UI::ListBox(RightPane);
	_ShipListBox->SetLeft(0.0f);
	_ShipListBox->SetTop(30.0f);
	_ShipListBox->SetSize(Vector2f(RightPane->GetSize()[0], RightPane->GetSize()[1] - 30.0f));
	_ShipListBox->SetHorizontalScrollBarVisible(false);
	_ShipListBox->SetAnchorBottom(true);
	_ShipListBox->SetAnchorRight(true);
	_ShipListBox->GetContent()->SetSize(Vector2f(RightPane->GetSize()[0] - 20.0f, 0.0f));
	_ShipListBox->GetContent()->SetAnchorRight(true);
	assert(_Ship->GetCargoHold() != nullptr);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	for(auto ShipObject : _Ship->GetCargoHold()->GetAspectObjectContainer()->GetContent())
	{
		_ShipListBox->GetContent()->AddSubWidget(new ObjectListItem(ShipObject));
	}
	
	auto ShipLabel(new UI::Label(RightPane, "Ship"));
	
	ShipLabel->SetLeft(0.0f);
	ShipLabel->SetTop(0.0f);
	ShipLabel->SetSize(Vector2f(RightPane->GetSize()[0], 20.0f));
	ShipLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	ShipLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	ShipLabel->SetAnchorRight(true);
	ConnectSizeChangedCallback(std::bind(&UI::LoadShipWindow::_OnSizeChanged, this, std::placeholders::_1, LeftPane, CenterPane, RightPane));
	SetSize(Vector2f(600.0f, 400.0f));
}

void UI::LoadShipWindow::_OnDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		if(_Hangar != nullptr)
		{
			assert(_Hangar->GetAspectObjectContainer() != nullptr);
			assert(_HangarContentAddedCallbackConnection.IsValid() == true);
			_Hangar->GetAspectObjectContainer()->DisconnectContentAddedCallback(_HangarContentAddedCallbackConnection);
			assert(_HangarContentRemovedCallbackConnection.IsValid() == true);
			_Hangar->GetAspectObjectContainer()->DisconnectContentRemovedCallback(_HangarContentRemovedCallbackConnection);
			assert(_HangarDestroyingCallbackConnection.IsValid() == true);
			_Hangar->DisconnectDestroyingCallback(_HangarDestroyingCallbackConnection);
		}
		assert(_HangarContentAddedCallbackConnection.IsValid() == false);
		assert(_HangarContentRemovedCallbackConnection.IsValid() == false);
		assert(_HangarDestroyingCallbackConnection.IsValid() == false);
		assert(_Ship != nullptr);
		assert(_ShipDestroyingCallbackConnection.IsValid() == true);
		_Ship->DisconnectDestroyingCallback(_ShipDestroyingCallbackConnection);
		assert(_ShipDestroyingCallbackConnection.IsValid() == false);
		assert(_Ship->GetCargoHold() != nullptr);
		assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
		assert(_ShipContentAddedCallbackConnection.IsValid() == true);
		_Ship->GetCargoHold()->GetAspectObjectContainer()->DisconnectContentAddedCallback(_ShipContentAddedCallbackConnection);
		assert(_ShipContentAddedCallbackConnection.IsValid() == false);
		assert(_ShipContentRemovedCallbackConnection.IsValid() == true);
		_Ship->GetCargoHold()->GetAspectObjectContainer()->DisconnectContentRemovedCallback(_ShipContentRemovedCallbackConnection);
		assert(_ShipContentRemovedCallbackConnection.IsValid() == false);
	}
}

void UI::LoadShipWindow::_OnHangarContentAdded(Object * Content)
{
	assert(Content != nullptr);
	if(Content->GetAspectName() != nullptr)
	{
		assert(_HangarListBox != nullptr);
		assert(_HangarListBox->GetContent() != nullptr);
		_HangarListBox->GetContent()->AddSubWidget(new UI::ObjectListItem(Content));
	}
}

void UI::LoadShipWindow::_OnHangarContentRemoved(Object * Content)
{
	assert(Content != nullptr);
	assert(_HangarListBox != nullptr);
	assert(_HangarListBox->GetContent() != nullptr);
	
	auto HangarWidgetIterator(std::find_if(_HangarListBox->GetContent()->GetSubWidgets().begin(), _HangarListBox->GetContent()->GetSubWidgets().end(), [Content](UI::Widget * Widget) { return dynamic_cast< UI::ObjectListItem * >(Widget)->GetObject() == Content; }));
	
	if(HangarWidgetIterator != _HangarListBox->GetContent()->GetSubWidgets().end())
	{
		(*HangarWidgetIterator)->Destroy();
	}
}

void UI::LoadShipWindow::_OnHangarDestroying(void)
{
	assert(_Hangar != nullptr);
	assert(_Hangar->GetAspectObjectContainer() != nullptr);
	assert(_HangarContentAddedCallbackConnection.IsValid() == true);
	_Hangar->GetAspectObjectContainer()->DisconnectContentAddedCallback(_HangarContentAddedCallbackConnection);
	assert(_HangarContentAddedCallbackConnection.IsValid() == false);
	assert(_HangarContentRemovedCallbackConnection.IsValid() == true);
	_Hangar->GetAspectObjectContainer()->DisconnectContentRemovedCallback(_HangarContentRemovedCallbackConnection);
	assert(_HangarContentRemovedCallbackConnection.IsValid() == false);
	assert(_HangarDestroyingCallbackConnection.IsValid() == true);
	_Hangar->DisconnectDestroyingCallback(_HangarDestroyingCallbackConnection);
	assert(_HangarDestroyingCallbackConnection.IsValid() == false);
	_Hangar = nullptr;
}

void UI::LoadShipWindow::_OnKey(UI::KeyEvent & KeyEvent)
{
	if(KeyEvent.GetPhase() == UI::Event::Phase::Bubbling)
	{
		if(((KeyEvent.GetKeyCode() == 9 /* ESCAPE */) || (KeyEvent.GetKeyCode() == 36 /* RETURN */)) && (KeyEvent.IsDown() == true))
		{
			Destroy();
			KeyEvent.StopPropagation();
		}
	}
}

void UI::LoadShipWindow::_OnMoveToHangarButtonClicked(void)
{
	auto SelectedShipItem(dynamic_cast< UI::ObjectListItem * >(_ShipListBox->GetSelectedItem()));
	
	assert(SelectedShipItem != nullptr);
	
	auto MovingObject(SelectedShipItem->GetObject());
	
	assert(MovingObject != nullptr);
	assert(_Ship != nullptr);
	assert(_Ship->GetCargoHold() != nullptr);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	_Ship->GetCargoHold()->GetAspectObjectContainer()->RemoveContent(MovingObject);
	assert(_Hangar != nullptr);
	assert(_Hangar->GetAspectObjectContainer() != nullptr);
	_Hangar->GetAspectObjectContainer()->AddContent(MovingObject);
}

void UI::LoadShipWindow::_OnMoveToHangarButtonUpdating(float RealTimeSeconds, float GameTimeSeconds, UI::Button * MoveToHangarButton)
{
	MoveToHangarButton->SetEnabled((_Hangar != nullptr) && (dynamic_cast< UI::ObjectListItem * >(_ShipListBox->GetSelectedItem()) != nullptr));
}

void UI::LoadShipWindow::_OnMoveToShipButtonClicked(void)
{
	auto SelectedHangarItem(dynamic_cast< UI::ObjectListItem * >(_HangarListBox->GetSelectedItem()));
	
	assert(SelectedHangarItem != nullptr);
	
	auto MovingObject(SelectedHangarItem->GetObject());
	
	assert(MovingObject != nullptr);
	assert(_Hangar != nullptr);
	assert(_Hangar->GetAspectObjectContainer() != nullptr);
	_Hangar->GetAspectObjectContainer()->RemoveContent(MovingObject);
	assert(_Ship != nullptr);
	assert(_Ship->GetCargoHold() != nullptr);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	_Ship->GetCargoHold()->GetAspectObjectContainer()->AddContent(MovingObject);
}

void UI::LoadShipWindow::_OnMoveToShipButtonUpdating(float RealTimeSeconds, float GameTimeSeconds, UI::Button * MoveToShipButton)
{
	auto SelectedHangarItem(dynamic_cast< UI::ObjectListItem * >(_HangarListBox->GetSelectedItem()));
	
	MoveToShipButton->SetEnabled((_Hangar != nullptr) && (SelectedHangarItem != nullptr) && (SelectedHangarItem->GetObject() != nullptr) && (_Ship != nullptr) && (_Ship->GetCargoHold() != nullptr) && (_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr) && ((SelectedHangarItem->GetObject()->GetAspectPhysical() == nullptr) || (_Ship->GetCargoHold()->GetSpace() >= SelectedHangarItem->GetObject()->GetAspectPhysical()->GetSpaceRequirement())));
}

void UI::LoadShipWindow::_OnOKButtonClicked(void)
{
	Destroy();
}

void UI::LoadShipWindow::_OnShipContentAdded(Object * Content)
{
	_ShipListBox->GetContent()->AddSubWidget(new ObjectListItem(Content));
}

void UI::LoadShipWindow::_OnShipContentRemoved(Object * Content)
{
	auto ShipWidgetIterator(std::find_if(_ShipListBox->GetContent()->GetSubWidgets().begin(), _ShipListBox->GetContent()->GetSubWidgets().end(), [Content](UI::Widget * Widget) { return dynamic_cast< UI::ObjectListItem * >(Widget)->GetObject() == Content; }));
	
	if(ShipWidgetIterator != _ShipListBox->GetContent()->GetSubWidgets().end())
	{
		(*ShipWidgetIterator)->Destroy();
	}
}

void UI::LoadShipWindow::_OnShipDestroying(void)
{
	assert(false);
}

void UI::LoadShipWindow::_OnSizeChanged(UI::Event & SizeChangedEvent, UI::Widget * LeftPane, UI::Widget * CenterPane, UI::Widget * RightPane)
{
	if(SizeChangedEvent.GetPhase() == UI::Event::Phase::Target)
	{
		float AvailableWidth(GetSize()[0]);
		
		// substract 10.0f for each border: left of left, between left and center, between center and right, right of right
		AvailableWidth -= 10.0f + 10.0f + 10.0f + 10.0f;
		AvailableWidth -= CenterPane->GetSize()[0];
		AvailableWidth /= 2.0f;
		LeftPane->SetSize(Vector2f(AvailableWidth, LeftPane->GetSize()[1]));
		CenterPane->SetLeft(10.0f + LeftPane->GetSize()[0] + 10.0f);
		RightPane->SetSize(Vector2f(AvailableWidth, RightPane->GetSize()[1]));
		RightPane->SetLeft(10.0f + LeftPane->GetSize()[0] + 10.0f + CenterPane->GetSize()[0] + 10.0f);
	}
}
