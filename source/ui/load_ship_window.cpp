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

#include "../color.h"
#include "../hangar.h"
#include "../object_aspect_name.h"
#include "../object_aspect_object_container.h"
#include "../object_aspect_physical.h"
#include "../ship.h"
#include "../storage.h"
#include "event.h"
#include "key_event.h"
#include "label.h"
#include "list_box_item.h"
#include "load_ship_window.h"
#include "mouse_button_event.h"
#include "scroll_box.h"
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
			// set design size
			SetSize(Vector2f(100.0f, 20.0f));
			
			auto NameLabel(new UI::Label(this, _Object->GetAspectName()->GetName()));
			
			NameLabel->SetPosition(Vector2f(5.0f, 0.0f));
			NameLabel->SetSize(Vector2f(90.0f, 20.0f));
			NameLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
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
				assert(_ObjectDestroyingConnection.IsValid() == true);
				_Object->DisconnectDestroyingCallback(_ObjectDestroyingConnection);
				assert(_ObjectDestroyingConnection.IsValid() == false);
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
	_SelectedHangarItem(nullptr),
	_SelectedShipItem(nullptr),
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
	
	LeftPane->SetPosition(Vector2f(10.0f, 40.0f));
	LeftPane->SetSize(Vector2f(200.0f, GetSize()[1] - 50.0f));
	LeftPane->SetAnchorBottom(true);
	
	auto HangarLabel(new UI::Label(LeftPane, "Hangar"));
	
	HangarLabel->SetPosition(Vector2f(0.0f, 0.0f));
	HangarLabel->SetSize(Vector2f(LeftPane->GetSize()[0], 20.0f));
	HangarLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	HangarLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
	HangarLabel->SetAnchorRight(true);
	_HangarScrollBox = new UI::ScrollBox(LeftPane);
	_HangarScrollBox->SetPosition(Vector2f(0.0f, 30.0f));
	_HangarScrollBox->SetSize(Vector2f(LeftPane->GetSize()[0], LeftPane->GetSize()[1] - 30.0f));
	_HangarScrollBox->SetHorizontalScrollBarVisible(false);
	_HangarScrollBox->SetAnchorBottom(true);
	_HangarScrollBox->SetAnchorRight(true);
	_HangarScrollBox->GetContent()->SetSize(Vector2f(LeftPane->GetSize()[0] - 20.0f, 0.0f));
	_HangarScrollBox->GetContent()->SetAnchorRight(true);
	_HangarScrollBox->GetContent()->ConnectSubWidgetAddedCallback(std::bind(&UI::LoadShipWindow::_OnHangarScrollBoxSubWidgetAdded, this, std::placeholders::_1));
	_HangarScrollBox->GetContent()->ConnectSubWidgetRemovedCallback(std::bind(&UI::LoadShipWindow::_OnHangarScrollBoxSubWidgetRemoved, this, std::placeholders::_1));
	assert(_Hangar->GetAspectObjectContainer() != nullptr);
	for(auto HangarObject : _Hangar->GetAspectObjectContainer()->GetContent())
	{
		_HangarScrollBox->GetContent()->AddSubWidget(new ObjectListItem(HangarObject));
	}
	
	auto CenterPane(new UI::Widget(this));
	
	CenterPane->SetPosition(Vector2f(10.0f + LeftPane->GetSize()[0] + 10.0f, 70.0f));
	CenterPane->SetSize(Vector2f(160.0f, GetSize()[1] - 80.0f));
	CenterPane->SetAnchorBottom(true);
	
	auto MoveToHangarButton(new UI::TextButton(CenterPane, "<<"));
	
	MoveToHangarButton->SetPosition(Vector2f(0.0f, 50.0f));
	MoveToHangarButton->SetSize(Vector2f(CenterPane->GetSize()[0], 20.0f));
	MoveToHangarButton->SetAnchorRight(true);
	MoveToHangarButton->ConnectClickedCallback(std::bind(&UI::LoadShipWindow::_OnMoveToHangarButtonClicked, this));
	MoveToHangarButton->ConnectUpdatingCallback(std::bind(&UI::LoadShipWindow::_OnMoveToHangarButtonUpdating, this, std::placeholders::_1, std::placeholders::_2, MoveToHangarButton));
	
	auto MoveToShipButton(new UI::TextButton(CenterPane, ">>"));
	
	MoveToShipButton->SetPosition(Vector2f(0.0f, 80.0f));
	MoveToShipButton->SetSize(Vector2f(CenterPane->GetSize()[0], 20.0f));
	MoveToShipButton->SetAnchorRight(true);
	MoveToShipButton->ConnectClickedCallback(std::bind(&UI::LoadShipWindow::_OnMoveToShipButtonClicked, this));
	MoveToShipButton->ConnectUpdatingCallback(std::bind(&UI::LoadShipWindow::_OnMoveToShipButtonUpdating, this, std::placeholders::_1, std::placeholders::_2, MoveToShipButton));
	
	auto OKButton(new UI::TextButton(CenterPane, "OK"));
	
	OKButton->SetPosition(Vector2f(0.0f, CenterPane->GetSize()[1] - 30.0f));
	OKButton->SetSize(Vector2f(CenterPane->GetSize()[0], 20.0f));
	OKButton->SetAnchorBottom(true);
	OKButton->SetAnchorRight(true);
	OKButton->SetAnchorTop(false);
	OKButton->ConnectClickedCallback(std::bind(&UI::LoadShipWindow::_OnOKButtonClicked, this));
	
	auto RightPane(new UI::Widget(this));
	
	RightPane->SetPosition(Vector2f(10.0f + LeftPane->GetSize()[0] + 10.0f + CenterPane->GetSize()[0] + 10.0f, 40.0f));
	RightPane->SetSize(Vector2f(200.0f, GetSize()[1] - 50.0f));
	RightPane->SetAnchorBottom(true);
	_ShipScrollBox = new UI::ScrollBox(RightPane);
	_ShipScrollBox->SetPosition(Vector2f(0.0f, 30.0f));
	_ShipScrollBox->SetSize(Vector2f(RightPane->GetSize()[0], RightPane->GetSize()[1] - 30.0f));
	_ShipScrollBox->SetHorizontalScrollBarVisible(false);
	_ShipScrollBox->SetAnchorBottom(true);
	_ShipScrollBox->SetAnchorRight(true);
	_ShipScrollBox->GetContent()->SetSize(Vector2f(RightPane->GetSize()[0] - 20.0f, 0.0f));
	_ShipScrollBox->GetContent()->SetAnchorRight(true);
	_ShipScrollBox->GetContent()->ConnectSubWidgetAddedCallback(std::bind(&UI::LoadShipWindow::_OnShipScrollBoxSubWidgetAdded, this, std::placeholders::_1));
	_ShipScrollBox->GetContent()->ConnectSubWidgetRemovedCallback(std::bind(&UI::LoadShipWindow::_OnShipScrollBoxSubWidgetRemoved, this, std::placeholders::_1));
	assert(_Ship->GetCargoHold() != nullptr);
	assert(_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr);
	for(auto ShipObject : _Ship->GetCargoHold()->GetAspectObjectContainer()->GetContent())
	{
		_ShipScrollBox->GetContent()->AddSubWidget(new ObjectListItem(ShipObject));
	}
	
	auto ShipLabel(new UI::Label(RightPane, "Ship"));
	
	ShipLabel->SetPosition(Vector2f(0.0f, 0.0f));
	ShipLabel->SetSize(Vector2f(RightPane->GetSize()[0], 20.0f));
	ShipLabel->SetHorizontalAlignment(UI::Label::ALIGN_HORIZONTAL_CENTER);
	ShipLabel->SetVerticalAlignment(UI::Label::ALIGN_VERTICAL_CENTER);
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
	_HangarScrollBox->GetContent()->AddSubWidget(new ObjectListItem(Content));
}

void UI::LoadShipWindow::_OnHangarContentRemoved(Object * Content)
{
	auto HangarWidgetIterator(std::find_if(_HangarScrollBox->GetContent()->GetSubWidgets().begin(), _HangarScrollBox->GetContent()->GetSubWidgets().end(), [Content](UI::Widget * Widget) { return dynamic_cast< UI::ObjectListItem * >(Widget)->GetObject() == Content; }));
	
	if(HangarWidgetIterator != _HangarScrollBox->GetContent()->GetSubWidgets().end())
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

void UI::LoadShipWindow::_OnHangarItemMouseButton(UI::MouseButtonEvent & MouseButtonEvent, UI::ObjectListItem * HangarItem)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedHangarItem != nullptr)
		{
			_SelectedHangarItem->SetSelected(false);
		}
		_SelectedHangarItem = HangarItem;
		_SelectedHangarItem->SetSelected(true);
	}
}

void UI::LoadShipWindow::_OnHangarScrollBoxSubWidgetAdded(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		auto NewWidget(dynamic_cast< UI::ObjectListItem * >(SubWidgetEvent.GetSubWidget()));
		
		if(NewWidget != nullptr)
		{
			float Top(0.0f);
			
			for(auto HangarScrollBoxWidget : _HangarScrollBox->GetContent()->GetSubWidgets())
			{
				if(HangarScrollBoxWidget != NewWidget)
				{
					Top = std::max(Top, HangarScrollBoxWidget->GetPosition()[1] + HangarScrollBoxWidget->GetSize()[1]);
				}
			}
			Top += 5.0f;
			NewWidget->SetPosition(Vector2f(5.0f, Top));
			NewWidget->SetSize(Vector2f(_HangarScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
			NewWidget->SetAnchorRight(true);
			NewWidget->ConnectMouseButtonCallback(std::bind(&UI::LoadShipWindow::_OnHangarItemMouseButton, this, std::placeholders::_1, NewWidget));
			_HangarScrollBox->GetContent()->SetSize(Vector2f(_HangarScrollBox->GetContent()->GetSize()[0], std::max(NewWidget->GetPosition()[1] + NewWidget->GetSize()[1], _HangarScrollBox->GetView()->GetSize()[1])));
		}
	}
}

void UI::LoadShipWindow::_OnHangarScrollBoxSubWidgetRemoved(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		float NextTop;
		UI::ObjectListItem * NextSelectedListItem(nullptr);
		float Top(0.0f);
		
		for(auto HangarScrollBoxWidget : _HangarScrollBox->GetContent()->GetSubWidgets())
		{
			if(HangarScrollBoxWidget->GetPosition()[1] > SubWidgetEvent.GetSubWidget()->GetPosition()[1])
			{
				if((NextSelectedListItem == nullptr) || (HangarScrollBoxWidget->GetPosition()[1] < NextTop))
				{
					NextSelectedListItem = dynamic_cast< ObjectListItem * >(HangarScrollBoxWidget);
					NextTop = HangarScrollBoxWidget->GetPosition()[1];
				}
				HangarScrollBoxWidget->SetPosition(Vector2f(HangarScrollBoxWidget->GetPosition()[0], HangarScrollBoxWidget->GetPosition()[1] - (SubWidgetEvent.GetSubWidget()->GetSize()[1] + 5.0f)));
			}
			else
			{
				if((NextSelectedListItem == nullptr) || (HangarScrollBoxWidget->GetPosition()[1] > NextTop))
				{
					NextSelectedListItem = dynamic_cast< ObjectListItem * >(HangarScrollBoxWidget);
					NextTop = HangarScrollBoxWidget->GetPosition()[1];
				}
			}
			Top = std::max(Top, HangarScrollBoxWidget->GetPosition()[1] + HangarScrollBoxWidget->GetSize()[1]);
		}
		if(SubWidgetEvent.GetSubWidget() == _SelectedHangarItem)
		{
			_SelectedHangarItem->SetSelected(false);
			if(NextSelectedListItem != nullptr)
			{
				NextSelectedListItem->SetSelected(true);
				_SelectedHangarItem = NextSelectedListItem;
			}
			else
			{
				_SelectedHangarItem = nullptr;
			}
		}
		_HangarScrollBox->GetContent()->SetSize(Vector2f(_HangarScrollBox->GetContent()->GetSize()[0], std::max(Top + 5.0f, _HangarScrollBox->GetView()->GetSize()[1])));
	}
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
	assert(_SelectedShipItem != nullptr);
	
	auto MovingObject(_SelectedShipItem->GetObject());
	
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
	MoveToHangarButton->SetEnabled((_Hangar != nullptr) && (_SelectedShipItem != nullptr));
}

void UI::LoadShipWindow::_OnMoveToShipButtonClicked(void)
{
	assert(_SelectedHangarItem != nullptr);
	
	auto MovingObject(_SelectedHangarItem->GetObject());
	
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
	MoveToShipButton->SetEnabled((_Hangar != nullptr) && (_SelectedHangarItem != nullptr) && (_SelectedHangarItem->GetObject() != nullptr) && (_Ship != nullptr) && (_Ship->GetCargoHold() != nullptr) && (_Ship->GetCargoHold()->GetAspectObjectContainer() != nullptr) && ((_SelectedHangarItem->GetObject()->GetAspectPhysical() == nullptr) || (_Ship->GetCargoHold()->GetSpace() >= _SelectedHangarItem->GetObject()->GetAspectPhysical()->GetSpaceRequirement())));
}

void UI::LoadShipWindow::_OnOKButtonClicked(void)
{
	Destroy();
}

void UI::LoadShipWindow::_OnShipContentAdded(Object * Content)
{
	_ShipScrollBox->GetContent()->AddSubWidget(new ObjectListItem(Content));
}

void UI::LoadShipWindow::_OnShipContentRemoved(Object * Content)
{
	auto ShipWidgetIterator(std::find_if(_ShipScrollBox->GetContent()->GetSubWidgets().begin(), _ShipScrollBox->GetContent()->GetSubWidgets().end(), [Content](UI::Widget * Widget) { return dynamic_cast< UI::ObjectListItem * >(Widget)->GetObject() == Content; }));
	
	if(ShipWidgetIterator != _ShipScrollBox->GetContent()->GetSubWidgets().end())
	{
		(*ShipWidgetIterator)->Destroy();
	}
}

void UI::LoadShipWindow::_OnShipDestroying(void)
{
	assert(false);
}

void UI::LoadShipWindow::_OnShipItemMouseButton(UI::MouseButtonEvent & MouseButtonEvent, UI::ObjectListItem * ShipItem)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedShipItem != nullptr)
		{
			_SelectedShipItem->SetSelected(false);
		}
		_SelectedShipItem = ShipItem;
		_SelectedShipItem->SetSelected(true);
	}
}

void UI::LoadShipWindow::_OnShipScrollBoxSubWidgetAdded(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		auto NewWidget(dynamic_cast< UI::ObjectListItem * >(SubWidgetEvent.GetSubWidget()));
		
		if(NewWidget != nullptr)
		{
			float Top(0.0f);
			
			for(auto ShipScrollBoxWidget : _ShipScrollBox->GetContent()->GetSubWidgets())
			{
				if(ShipScrollBoxWidget != NewWidget)
				{
					Top = std::max(Top, ShipScrollBoxWidget->GetPosition()[1] + ShipScrollBoxWidget->GetSize()[1]);
				}
			}
			Top += 5.0f;
			NewWidget->SetPosition(Vector2f(5.0f, Top));
			NewWidget->SetSize(Vector2f(_ShipScrollBox->GetContent()->GetSize()[0] - 10.0f, 20.0f));
			NewWidget->SetAnchorRight(true);
			NewWidget->ConnectMouseButtonCallback(std::bind(&UI::LoadShipWindow::_OnShipItemMouseButton, this, std::placeholders::_1, NewWidget));
			_ShipScrollBox->GetContent()->SetSize(Vector2f(_ShipScrollBox->GetContent()->GetSize()[0], std::max(NewWidget->GetPosition()[1] + NewWidget->GetSize()[1], _ShipScrollBox->GetView()->GetSize()[1])));
		}
	}
}

void UI::LoadShipWindow::_OnShipScrollBoxSubWidgetRemoved(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		float NextTop;
		UI::ObjectListItem * NextSelectedListItem(nullptr);
		float Top(0.0f);
		
		for(auto ShipScrollBoxWidget : _ShipScrollBox->GetContent()->GetSubWidgets())
		{
			if(ShipScrollBoxWidget->GetPosition()[1] > SubWidgetEvent.GetSubWidget()->GetPosition()[1])
			{
				if((NextSelectedListItem == nullptr) || (ShipScrollBoxWidget->GetPosition()[1] < NextTop))
				{
					NextSelectedListItem = dynamic_cast< ObjectListItem * >(ShipScrollBoxWidget);
					NextTop = ShipScrollBoxWidget->GetPosition()[1];
				}
				ShipScrollBoxWidget->SetPosition(Vector2f(ShipScrollBoxWidget->GetPosition()[0], ShipScrollBoxWidget->GetPosition()[1] - (SubWidgetEvent.GetSubWidget()->GetSize()[1] + 5.0f)));
			}
			else
			{
				if((NextSelectedListItem == nullptr) || (ShipScrollBoxWidget->GetPosition()[1] > NextTop))
				{
					NextSelectedListItem = dynamic_cast< ObjectListItem * >(ShipScrollBoxWidget);
					NextTop = ShipScrollBoxWidget->GetPosition()[1];
				}
			}
			Top = std::max(Top, ShipScrollBoxWidget->GetPosition()[1] + ShipScrollBoxWidget->GetSize()[1]);
		}
		if(SubWidgetEvent.GetSubWidget() == _SelectedShipItem)
		{
			_SelectedShipItem->SetSelected(false);
			if(NextSelectedListItem != nullptr)
			{
				NextSelectedListItem->SetSelected(true);
				_SelectedShipItem = NextSelectedListItem;
			}
			else
			{
				_SelectedShipItem = nullptr;
			}
		}
		_ShipScrollBox->GetContent()->SetSize(Vector2f(_ShipScrollBox->GetContent()->GetSize()[0], std::max(Top + 5.0f, _ShipScrollBox->GetView()->GetSize()[1])));
	}
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
		CenterPane->SetPosition(Vector2f(10.0f + LeftPane->GetSize()[0] + 10.0f, CenterPane->GetPosition()[1]));
		RightPane->SetSize(Vector2f(AvailableWidth, RightPane->GetSize()[1]));
		RightPane->SetPosition(Vector2f(10.0f + LeftPane->GetSize()[0] + 10.0f + CenterPane->GetSize()[0] + 10.0f, RightPane->GetPosition()[1]));
	}
}
