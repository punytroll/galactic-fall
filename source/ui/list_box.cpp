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

#include "list_box.h"
#include "list_box_item.h"
#include "mouse_button_event.h"
#include "sub_widget_event.h"

UI::ListBox::ListBox(UI::Widget * SupWidget) :
	UI::ScrollBox(SupWidget),
	_SelectedItem(nullptr)
{
	GetContent()->ConnectSubWidgetAddedCallback(std::bind(&UI::ListBox::_OnSubWidgetAdded, this, std::placeholders::_1));
	GetContent()->ConnectSubWidgetRemovedCallback(std::bind(&UI::ListBox::_OnSubWidgetRemoved, this, std::placeholders::_1));
}

void UI::ListBox::_OnSubWidgetAdded(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		auto NewWidget(dynamic_cast< UI::ListBoxItem * >(SubWidgetEvent.GetSubWidget()));
		
		if(NewWidget != nullptr)
		{
			float Top(0.0f);
			
			for(auto OldWidget : GetContent()->GetSubWidgets())
			{
				if(OldWidget != NewWidget)
				{
					Top = std::max(Top, OldWidget->GetTop() + OldWidget->GetSize()[1]);
				}
			}
			Top += 5.0f;
			NewWidget->SetLeft(5.0f);
			NewWidget->SetTop(Top);
			NewWidget->SetSize(Vector2f(GetContent()->GetSize()[0] - 10.0f, 20.0f));
			NewWidget->SetAnchorRight(true);
			NewWidget->ConnectMouseButtonCallback(std::bind(&UI::ListBox::_OnItemMouseButton, this, std::placeholders::_1, NewWidget));
			GetContent()->SetSize(Vector2f(GetContent()->GetSize()[0], std::max(NewWidget->GetTop() + NewWidget->GetSize()[1], GetView()->GetSize()[1])));
		}
	}
}

void UI::ListBox::_OnSubWidgetRemoved(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		float NextTop;
		UI::ListBoxItem * NextSelectedListItem(nullptr);
		float Top(0.0f);
		
		for(auto OldSubWidget : GetContent()->GetSubWidgets())
		{
			if(OldSubWidget->GetTop() > SubWidgetEvent.GetSubWidget()->GetTop())
			{
				if((NextSelectedListItem == nullptr) || (OldSubWidget->GetTop() < NextTop))
				{
					NextSelectedListItem = dynamic_cast< UI::ListBoxItem * >(OldSubWidget);
					NextTop = OldSubWidget->GetTop();
				}
				OldSubWidget->SetTop(OldSubWidget->GetTop() - (SubWidgetEvent.GetSubWidget()->GetSize()[1] + 5.0f));
			}
			else
			{
				if((NextSelectedListItem == nullptr) || (OldSubWidget->GetTop() > NextTop))
				{
					NextSelectedListItem = dynamic_cast< UI::ListBoxItem * >(OldSubWidget);
					NextTop = OldSubWidget->GetTop();
				}
			}
			Top = std::max(Top, OldSubWidget->GetTop() + OldSubWidget->GetSize()[1]);
		}
		if(SubWidgetEvent.GetSubWidget() == _SelectedItem)
		{
			_SelectedItem->SetSelected(false);
			if(NextSelectedListItem != nullptr)
			{
				NextSelectedListItem->SetSelected(true);
				_SelectedItem = NextSelectedListItem;
			}
			else
			{
				_SelectedItem = nullptr;
			}
		}
		GetContent()->SetSize(Vector2f(GetContent()->GetSize()[0], std::max(Top + 5.0f, GetView()->GetSize()[1])));
	}
}

void UI::ListBox::_OnItemMouseButton(UI::MouseButtonEvent & MouseButtonEvent, UI::ListBoxItem * Item)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedItem != nullptr)
		{
			_SelectedItem->SetSelected(false);
		}
		_SelectedItem = Item;
		_SelectedItem->SetSelected(true);
	}
}
