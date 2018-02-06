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

#include <expressions/operators.h>

#include "list_box.h"
#include "list_box_item.h"
#include "mouse_button_event.h"
#include "sub_widget_event.h"

using namespace Expressions::Operators;

float g_ListBoxItemPadding{2.0f};

UI::ListBox::ListBox(void) :
	_SelectedItem(nullptr)
{
	GetContent()->SetWidth(width(GetView()));
	GetContent()->ConnectSubWidgetAddedCallback(std::bind(&UI::ListBox::_OnSubWidgetAdded, this, std::placeholders::_1));
	GetContent()->ConnectSubWidgetRemovedCallback(std::bind(&UI::ListBox::_OnSubWidgetRemoved, this, std::placeholders::_1));
}

Connection UI::ListBox::ConnectSelectedItemChangedCallback(std::function< void (void) > SelectedItemChangedHandler)
{
	return _SelectedItemChangedEvent.Connect(SelectedItemChangedHandler);
}

void UI::ListBox::SetSelectedItem(UI::ListBoxItem * ListBoxItem)
{
	auto SelectedItemChanged{false};
	
	if(_SelectedItem != nullptr)
	{
		_SelectedItem->SetSelected(false);
		_SelectedItem = nullptr;
		SelectedItemChanged = true;
	}
	_SelectedItem = ListBoxItem;
	if(_SelectedItem != nullptr)
	{
		assert(ListBoxItem->GetSupWidget() == GetContent());
		_SelectedItem->SetSelected(true);
		SelectedItemChanged = true;
	}
	if(SelectedItemChanged == true)
	{
		_SelectedItemChangedEvent();
	}
}

void UI::ListBox::_OnSubWidgetAdded(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		auto NewListBoxItem(dynamic_cast< UI::ListBoxItem * >(SubWidgetEvent.GetSubWidget()));
		
		if(NewListBoxItem != nullptr)
		{
			UI::ListBoxItem * LeadingListBoxItem{nullptr};
			
			for(auto SubWidget : GetContent()->GetSubWidgets())
			{
				auto ListBoxItem{dynamic_cast< UI::ListBoxItem * >(SubWidget)};
				
				if((ListBoxItem != nullptr) && (ListBoxItem != NewListBoxItem) && ((LeadingListBoxItem == nullptr) || (ListBoxItem->GetBottom() > LeadingListBoxItem->GetBottom())))
				{
					LeadingListBoxItem = ListBoxItem;
				}
			}
			NewListBoxItem->SetLeft(constant(g_ListBoxItemPadding));
			if(LeadingListBoxItem != nullptr)
			{
				NewListBoxItem->SetTop(bottom(LeadingListBoxItem) + constant(g_ListBoxItemPadding));
			}
			else
			{
				NewListBoxItem->SetTop(constant(g_ListBoxItemPadding));
			}
			NewListBoxItem->SetWidth(width(GetContent()) - 2.0_c * constant(g_ListBoxItemPadding));
			NewListBoxItem->ConnectMouseButtonCallback(std::bind(&UI::ListBox::_OnItemMouseButton, this, std::placeholders::_1, NewListBoxItem));
			GetContent()->SetHeight(bottom(NewListBoxItem) + constant(g_ListBoxItemPadding));
		}
	}
}

void UI::ListBox::_OnSubWidgetRemoved(UI::SubWidgetEvent & SubWidgetEvent)
{
	if(SubWidgetEvent.GetPhase() == UI::Event::Phase::Target)
	{
		auto RemovedListItem{dynamic_cast< UI::ListBoxItem * >(SubWidgetEvent.GetSubWidget())};
		
		if(RemovedListItem != nullptr)
		{
			UI::ListBoxItem * LastListItem{nullptr};
			UI::ListBoxItem * LeadingListItem{nullptr};
			UI::ListBoxItem * FollowingListItem{nullptr};
			
			for(auto SubWidget : GetContent()->GetSubWidgets())
			{
				if(SubWidget != RemovedListItem)
				{
					auto ListItem{dynamic_cast< UI::ListBoxItem * >(SubWidget)};
					
					if(ListItem != nullptr)
					{
						// calculate the last widget in the list, so that we can set the content's height accordingly
						if((LastListItem == nullptr) || (ListItem->GetTop() > LastListItem->GetTop()))
						{
							LastListItem = ListItem;
						}
						// calculate the widgets that immediately surround the removed widget, so that we can set the following widget's top accordingly
						if(ListItem->GetTop() < RemovedListItem->GetTop())
						{
							if((LeadingListItem == nullptr) || (ListItem->GetTop() > LeadingListItem->GetTop()))
							{
								LeadingListItem = ListItem;
							}
						}
						else
						{
							if((FollowingListItem == nullptr) || (ListItem->GetTop() < FollowingListItem->GetTop()))
							{
								FollowingListItem = ListItem;
							}
						}
					}
				}
			}
			if(FollowingListItem != nullptr)
			{
				if(LeadingListItem != nullptr)
				{
					FollowingListItem->SetTop(bottom(LeadingListItem) + constant(g_ListBoxItemPadding));
				}
				else
				{
					FollowingListItem->SetTop(constant(g_ListBoxItemPadding));
				}
			}
			if(RemovedListItem == _SelectedItem)
			{
				_SelectedItem->SetSelected(false);
				if(FollowingListItem != nullptr)
				{
					FollowingListItem->SetSelected(true);
					_SelectedItem = FollowingListItem;
				}
				else if(LeadingListItem != nullptr)
				{
					LeadingListItem->SetSelected(true);
					_SelectedItem = LeadingListItem;
				}
				else
				{
					_SelectedItem = nullptr;
				}
			}
			if(LastListItem != nullptr)
			{
				GetContent()->SetHeight(constant(LastListItem->GetBottom() + g_ListBoxItemPadding));
			}
			else
			{
				GetContent()->SetHeight(0.0_c);
			}
			_SelectedItemChangedEvent();
		}
	}
}

void UI::ListBox::_OnItemMouseButton(UI::MouseButtonEvent & MouseButtonEvent, UI::ListBoxItem * Item)
{
	if((MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left) && (MouseButtonEvent.IsDown() == true))
	{
		if(_SelectedItem != Item)
		{
			if(_SelectedItem != nullptr)
			{
				_SelectedItem->SetSelected(false);
			}
			_SelectedItem = Item;
			_SelectedItem->SetSelected(true);
			_SelectedItemChangedEvent();
		}
	}
}
