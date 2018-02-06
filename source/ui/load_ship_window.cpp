/**
 * Copyright (C) 2014-2018  Hagen Möbius
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

#include <expressions/operators.h>

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
#include "list_box_object_item.h"
#include "load_ship_window.h"
#include "mouse_button_event.h"
#include "sub_widget_event.h"
#include "text_button.h"

using namespace Expressions::Operators;

UI::LoadShipWindow::LoadShipWindow(Hangar * Hangar, Ship * Ship) :
	_Hangar(Hangar),
	_Ship(Ship)
{
	SetTitle("Load Ship");
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
	
	auto LeftPane{new UI::Widget{this}};
	
	LeftPane->SetLeft(10.0_c);
	LeftPane->SetTop(40.0_c);
	LeftPane->SetHeight(constant(GetHeight() - 50.0f));
	LeftPane->SetAnchorBottom(true);
	
	auto HangarLabel{new UI::Label{LeftPane, "Hangar"}};
	
	HangarLabel->SetLeft(0.0_c);
	HangarLabel->SetTop(0.0_c);
	HangarLabel->SetWidth(width(LeftPane));
	HangarLabel->SetHeight(20.0_c);
	HangarLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	HangarLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	_HangarListBox = new UI::ListBox{};
	_HangarListBox->SetLeft(0.0_c);
	_HangarListBox->SetTop(30.0_c);
	_HangarListBox->SetWidth(constant(LeftPane->GetWidth()));
	_HangarListBox->SetHeight(constant(LeftPane->GetHeight() - 30.0f));
	_HangarListBox->SetHorizontalScrollBarVisible(false);
	_HangarListBox->SetAnchorBottom(true);
	_HangarListBox->SetAnchorRight(true);
	_HangarListBox->GetContent()->SetWidth(constant(_HangarListBox->GetView()->GetWidth()));
	_HangarListBox->GetContent()->SetAnchorRight(true);
	assert(_Hangar->GetAspectObjectContainer() != nullptr);
	for(auto HangarObject : _Hangar->GetAspectObjectContainer()->GetContent())
	{
		if(HangarObject->GetAspectName() != nullptr)
		{
			_HangarListBox->GetContent()->AddSubWidget(new UI::ListBoxObjectItem(HangarObject));
		}
	}
	LeftPane->AddSubWidget(_HangarListBox);
	
	auto CenterPane{new UI::Widget{this}};
	
	CenterPane->SetTop(70.0_c);
	CenterPane->SetWidth(100.0_c);
	CenterPane->SetHeight(height(this) - 10.0_c - top(CenterPane));
	
	auto MoveToHangarButton{new UI::TextButton{CenterPane, "<<"}};
	
	MoveToHangarButton->SetLeft(0.0_c);
	MoveToHangarButton->SetTop(50.0_c);
	MoveToHangarButton->SetWidth(width(CenterPane));
	MoveToHangarButton->SetHeight(20.0_c);
	MoveToHangarButton->ConnectClickedCallback(std::bind(&UI::LoadShipWindow::_OnMoveToHangarButtonClicked, this));
	MoveToHangarButton->ConnectUpdatingCallback(std::bind(&UI::LoadShipWindow::_OnMoveToHangarButtonUpdating, this, std::placeholders::_1, std::placeholders::_2, MoveToHangarButton));
	
	auto MoveToShipButton(new UI::TextButton(CenterPane, ">>"));
	
	MoveToShipButton->SetLeft(0.0_c);
	MoveToShipButton->SetTop(bottom(MoveToHangarButton) + 10.0_c);
	MoveToShipButton->SetWidth(width(CenterPane));
	MoveToShipButton->SetHeight(20.0_c);
	MoveToShipButton->ConnectClickedCallback(std::bind(&UI::LoadShipWindow::_OnMoveToShipButtonClicked, this));
	MoveToShipButton->ConnectUpdatingCallback(std::bind(&UI::LoadShipWindow::_OnMoveToShipButtonUpdating, this, std::placeholders::_1, std::placeholders::_2, MoveToShipButton));
	
	auto OKButton{new UI::TextButton{CenterPane, "OK"}};
	
	OKButton->SetLeft(0.0_c);
	OKButton->SetTop(height(CenterPane) - 10.0_c - height(OKButton));
	OKButton->SetWidth(width(CenterPane));
	OKButton->SetHeight(20.0_c);
	OKButton->ConnectClickedCallback(std::bind(&UI::LoadShipWindow::_OnOKButtonClicked, this));
	
	auto RightPane{new UI::Widget{this}};
	
	RightPane->SetTop(40.0_c);
	RightPane->SetHeight(constant(GetHeight() - 50.0f));
	RightPane->SetAnchorBottom(true);
	_ShipListBox = new UI::ListBox{};
	_ShipListBox->SetLeft(0.0_c);
	_ShipListBox->SetTop(30.0_c);
	_ShipListBox->SetWidth(constant(RightPane->GetWidth()));
	_ShipListBox->SetHeight(constant(RightPane->GetHeight() - 30.0f));
	_ShipListBox->SetHorizontalScrollBarVisible(false);
	_ShipListBox->SetAnchorBottom(true);
	_ShipListBox->SetAnchorRight(true);
	_ShipListBox->GetContent()->SetWidth(constant(_ShipListBox->GetView()->GetWidth()));
	_ShipListBox->GetContent()->SetAnchorRight(true);
	assert(_Ship->GetCargoHold() != nullptr);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	for(auto ShipObject : _Ship->GetCargoHold()->GetAspectObjectContainer()->GetContent())
	{
		_ShipListBox->GetContent()->AddSubWidget(new UI::ListBoxObjectItem(ShipObject));
	}
	RightPane->AddSubWidget(_ShipListBox);
	
	auto ShipLabel{new UI::Label{RightPane, "Ship"}};
	
	ShipLabel->SetLeft(0.0_c);
	ShipLabel->SetTop(0.0_c);
	ShipLabel->SetWidth(width(RightPane));
	ShipLabel->SetHeight(20.0_c);
	ShipLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
	ShipLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
	ConnectWidthChangedCallback(std::bind(&UI::LoadShipWindow::_OnWidthChanged, this, std::placeholders::_1, LeftPane, CenterPane, RightPane));
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
		_HangarListBox->GetContent()->AddSubWidget(new UI::ListBoxObjectItem(Content));
	}
}

void UI::LoadShipWindow::_OnHangarContentRemoved(Object * Content)
{
	assert(Content != nullptr);
	assert(_HangarListBox != nullptr);
	assert(_HangarListBox->GetContent() != nullptr);
	
	auto HangarWidgetIterator(std::find_if(_HangarListBox->GetContent()->GetSubWidgets().begin(), _HangarListBox->GetContent()->GetSubWidgets().end(), [Content](UI::Widget * Widget) { return dynamic_cast< UI::ListBoxObjectItem * >(Widget)->GetObject() == Content; }));
	
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
	auto SelectedShipItem(dynamic_cast< UI::ListBoxObjectItem * >(_ShipListBox->GetSelectedItem()));
	
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
	MoveToHangarButton->SetEnabled((_Hangar != nullptr) && (dynamic_cast< UI::ListBoxObjectItem * >(_ShipListBox->GetSelectedItem()) != nullptr));
}

void UI::LoadShipWindow::_OnMoveToShipButtonClicked(void)
{
	auto SelectedHangarItem(dynamic_cast< UI::ListBoxObjectItem * >(_HangarListBox->GetSelectedItem()));
	
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
	auto SelectedHangarItem(dynamic_cast< UI::ListBoxObjectItem * >(_HangarListBox->GetSelectedItem()));
	
	MoveToShipButton->SetEnabled((_Hangar != nullptr) && (SelectedHangarItem != nullptr) && (SelectedHangarItem->GetObject() != nullptr) && (_Ship != nullptr) && (_Ship->GetCargoHold() != nullptr) && (_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr) && ((SelectedHangarItem->GetObject()->GetAspectPhysical() == nullptr) || (_Ship->GetCargoHold()->GetSpace() >= SelectedHangarItem->GetObject()->GetAspectPhysical()->GetSpaceRequirement())));
}

void UI::LoadShipWindow::_OnOKButtonClicked(void)
{
	Destroy();
}

void UI::LoadShipWindow::_OnShipContentAdded(Object * Content)
{
	_ShipListBox->GetContent()->AddSubWidget(new UI::ListBoxObjectItem(Content));
}

void UI::LoadShipWindow::_OnShipContentRemoved(Object * Content)
{
	auto ShipWidgetIterator(std::find_if(_ShipListBox->GetContent()->GetSubWidgets().begin(), _ShipListBox->GetContent()->GetSubWidgets().end(), [Content](UI::Widget * Widget) { return dynamic_cast< UI::ListBoxObjectItem * >(Widget)->GetObject() == Content; }));
	
	if(ShipWidgetIterator != _ShipListBox->GetContent()->GetSubWidgets().end())
	{
		(*ShipWidgetIterator)->Destroy();
	}
}

void UI::LoadShipWindow::_OnShipDestroying(void)
{
	assert(false);
}

void UI::LoadShipWindow::_OnWidthChanged(UI::Event & WidthChangedEvent, UI::Widget * LeftPane, UI::Widget * CenterPane, UI::Widget * RightPane)
{
	if(WidthChangedEvent.GetPhase() == UI::Event::Phase::Target)
	{
		float AvailableWidth{GetWidth()};
		
		// substract 10.0f for each border: left of left, between left and center, between center and right, right of right
		AvailableWidth -= 10.0f + 10.0f + 10.0f + 10.0f;
		AvailableWidth -= CenterPane->GetWidth();
		AvailableWidth /= 2.0f;
		LeftPane->SetWidth(constant(AvailableWidth));
		CenterPane->SetLeft(constant(LeftPane->GetRight() + 10.0f));
		RightPane->SetLeft(constant(CenterPane->GetRight() + 10.0f));
		RightPane->SetWidth(constant(AvailableWidth));
	}
}
