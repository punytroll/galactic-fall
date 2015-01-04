/**
 * galactic-fall
 * Copyright (C) 2006  Hagen Möbius
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

#include <vector>

#include "../graphics/gl.h"
#include "key_event.h"
#include "mouse_button_event.h"
#include "mouse_move_event.h"
#include "sub_widget_event.h"
#include "user_interface.h"
#include "widget.h"

struct EventPropagationPathItem
{
	Connection _Connection;
	UI::Event::Phase _Phase;
	UI::Widget * _Widget;
};

void DestroyingPropagationPathItem(UI::Event & DestroyingEvent, EventPropagationPathItem * EventPropagationPathItem)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		assert(EventPropagationPathItem != nullptr);
		EventPropagationPathItem->_Widget = nullptr;
		EventPropagationPathItem->_Connection.Disconnect();
	}
}

struct KeyEventPropagationPathItem : EventPropagationPathItem
{
};

struct MouseButtonEventPropagationPathItem : EventPropagationPathItem
{
	Vector2f _Position;
};

struct MouseMoveEventPropagationPathItem : EventPropagationPathItem
{
	Vector2f _Position;
};

UI::UserInterface::UserInterface(void) :
	_CaptureWidget(nullptr),
	_HoverWidget(nullptr),
	_RootWidget(new UI::Widget(nullptr))
{
	_RootWidget->ConnectDestroyingCallback(std::bind(&UI::UserInterface::_OnRootWidgetDestroying, this, std::placeholders::_1));
}

UI::UserInterface::~UserInterface(void)
{
	if(_CaptureWidget != nullptr)
	{
		_CaptureWidget = nullptr;
		_CaptureWidgetDestroyingCallbackConnection.Disconnect();
	}
	_RootWidget->Destroy();
	_RootWidget = nullptr;
}

void UI::UserInterface::Draw(void) const
{
	if((_RootWidget != nullptr) && (_RootWidget->IsVisible() == true))
	{
		GLPushMatrix();
		UI::Widget::_PushClippingRectangle(_RootWidget->_Position, _RootWidget->_Size);
		GLTranslatef(_RootWidget->_Position[0], _RootWidget->_Position[1], 0.0f);
		UI::Widget::_DrawClippingRectangle();
		_RootWidget->Draw();
		UI::Widget::_PopClippingRectangle();
		GLPopMatrix();
	}
}

void UI::UserInterface::SetCaptureWidget(UI::Widget * Widget)
{
	assert(Widget != nullptr);
	assert(_CaptureWidget == nullptr);
	_CaptureWidget = Widget;
	_CaptureWidgetDestroyingCallbackConnection = _CaptureWidget->ConnectDestroyingCallback(std::bind(&UI::UserInterface::_OnCaptureWidgetDestroying, this, std::placeholders::_1));
}

void UI::UserInterface::ReleaseCaptureWidget(void)
{
	if(_CaptureWidget != nullptr)
	{
		_CaptureWidget = nullptr;
		_CaptureWidgetDestroyingCallbackConnection.Disconnect();
	}
}

UI::Widget * UI::UserInterface::GetWidget(const std::string & Path)
{
	if(Path[0] != '/')
	{
		return nullptr;
	}
	
	std::string::size_type Position(1);
	auto * Root(_RootWidget);
	
	while((Root != nullptr) && (Position < Path.length()))
	{
		std::string::size_type SlashPosition(Path.find('/', Position));
		
		Root = Root->GetSubWidget(Path.substr(Position, SlashPosition - Position));
		Position = ((SlashPosition == std::string::npos) ? (Path.length()) : (SlashPosition + 1));
	}
	
	return Root;
}

void UI::UserInterface::DispatchDestroyingEvent(UI::Event & DestroyingEvent)
{
	assert(DestroyingEvent.GetTarget() != nullptr);
	
	std::list< EventPropagationPathItem * > PropagationPath;
	auto TargetPathItem(new EventPropagationPathItem());
		
	TargetPathItem->_Widget = DestroyingEvent.GetTarget();
	TargetPathItem->_Phase = UI::Event::Phase::Target;
	TargetPathItem->_Connection = TargetPathItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetPathItem));
	PropagationPath.push_back(TargetPathItem);
	
	auto PathWidget(TargetPathItem->_Widget);
	
	while(PathWidget != nullptr)
	{
		auto CapturingPathItem(new EventPropagationPathItem());
		
		CapturingPathItem->_Widget = PathWidget;
		CapturingPathItem->_Phase = UI::Event::Phase::Capturing;
		CapturingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingPathItem));
		PropagationPath.push_front(CapturingPathItem);
	
		auto BubblingPathItem(new EventPropagationPathItem());
		
		BubblingPathItem->_Widget = PathWidget;
		BubblingPathItem->_Phase = UI::Event::Phase::Bubbling;
		BubblingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingPathItem));
		PropagationPath.push_back(BubblingPathItem);
		PathWidget = PathWidget->_SupWidget;
	}
	for(auto PropagationPathItem : PropagationPath)
	{
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			assert(PropagationPathItem->_Connection.IsValid() == true);
			DestroyingEvent.SetCurrentTarget(PropagationPathItem->_Widget);
			DestroyingEvent.SetPhase(PropagationPathItem->_Phase);
			DestroyingEvent.ResumeCallbacks();
			for(auto & Callback : PropagationPathItem->_Widget->_DestroyingEvent.GetCallbacks())
			{
				Callback(DestroyingEvent);
				if(DestroyingEvent.GetStopCallbacks() == true)
				{
					break;
				}
			}
			if(DestroyingEvent.GetStopPropagation() == true)
			{
				break;
			}
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
	}
	while(PropagationPath.empty() == false)
	{
		auto PropagationPathItem(PropagationPath.front());
		
		PropagationPath.pop_front();
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			PropagationPathItem->_Connection.Disconnect();
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::DispatchKeyEvent(UI::KeyEvent & KeyEvent)
{
	assert(KeyEvent.GetTarget() == nullptr);
	
	std::list< KeyEventPropagationPathItem * > PropagationPath;
	auto PathWidget(_RootWidget);
	
	while(PathWidget != nullptr)
	{
		assert(PathWidget->_Enabled == true);
		
		auto CapturingItem(new KeyEventPropagationPathItem());
		
		CapturingItem->_Widget = PathWidget;
		CapturingItem->_Phase = UI::Event::Phase::Capturing;
		CapturingItem->_Connection = CapturingItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingItem));
		PropagationPath.push_back(CapturingItem);
		PathWidget = PathWidget->_KeyFocus;
	}
	if(PropagationPath.size() > 0)
	{
		auto TargetItem(new KeyEventPropagationPathItem());
		
		TargetItem->_Widget = PropagationPath.back()->_Widget;
		TargetItem->_Phase = UI::Event::Phase::Target;
		TargetItem->_Connection = TargetItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetItem));
		PropagationPath.push_back(TargetItem);
		PathWidget = PropagationPath.back()->_Widget;
		while(PathWidget != nullptr)
		{
			assert(PathWidget->_Enabled == true);
			
			auto BubblingItem(new KeyEventPropagationPathItem());
			
			BubblingItem->_Widget = PathWidget;
			BubblingItem->_Phase = UI::Event::Phase::Bubbling;
			BubblingItem->_Connection = BubblingItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingItem));
			PropagationPath.push_back(BubblingItem);
			PathWidget = PathWidget->_SupWidget;
		}
	}
	for(auto PropagationPathItem : PropagationPath)
	{
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			assert(PropagationPathItem->_Connection.IsValid() == true);
			KeyEvent.SetCurrentTarget(PropagationPathItem->_Widget);
			KeyEvent.SetPhase(PropagationPathItem->_Phase);
			KeyEvent.ResumeCallbacks();
			for(auto & Callback : PropagationPathItem->_Widget->_KeyEvent.GetCallbacks())
			{
				Callback(KeyEvent);
				if(KeyEvent.GetStopCallbacks() == true)
				{
					break;
				}
			}
			if(KeyEvent.GetStopPropagation() == true)
			{
				break;
			}
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
	}
	while(PropagationPath.empty() == false)
	{
		auto PropagationPathItem(PropagationPath.front());
		
		PropagationPath.pop_front();
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			PropagationPathItem->_Connection.Disconnect();
		}
		delete PropagationPathItem;
	}
}

bool IsInside(const Vector2f & Position, const Vector2f & WidgetPosition, const Vector2f & WidgetSize)
{
	return (Position[0] >= WidgetPosition[0]) && (Position[0] < WidgetPosition[0] + WidgetSize[0]) && (Position[1] >= WidgetPosition[1]) && (Position[1] < WidgetPosition[1] + WidgetSize[1]);
}

bool IsInside(const Vector2f & Position, const Vector2f & WidgetSize)
{
	return (Position[0] >= 0.0f) && (Position[0] < WidgetSize[0]) && (Position[1] >= 0.0f) && (Position[1] < WidgetSize[1]);
}

void UI::UserInterface::DispatchMouseButtonEvent(UI::MouseButtonEvent & MouseButtonEvent)
{
	assert(MouseButtonEvent.GetTarget() == nullptr);
	
	std::list< MouseButtonEventPropagationPathItem * > PropagationPath;
	
	if(_CaptureWidget == nullptr)
	{
		if((_RootWidget != nullptr) && (_RootWidget->IsVisible() == true) && (_RootWidget->IsEnabled() == true))
		{
			auto Position(MouseButtonEvent.GetPosition() - _RootWidget->_Position);
			
			if(IsInside(Position, _RootWidget->GetSize()) == true)
			{
				auto PathWidget(_RootWidget);
				
				while(PathWidget != nullptr)
				{
					auto CapturingWidget(new MouseButtonEventPropagationPathItem());
					
					CapturingWidget->_Widget = PathWidget;
					CapturingWidget->_Phase = UI::Event::Phase::Capturing;
					CapturingWidget->_Position = Position;
					CapturingWidget->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingWidget));
					PropagationPath.push_back(CapturingWidget);
					
					Widget * NextWidget(nullptr);
					
					for(auto SubWidget : PathWidget->_SubWidgets)
					{
						if((SubWidget->IsVisible() == true) && (SubWidget->IsEnabled() == true) && (IsInside(Position, SubWidget->GetPosition(), SubWidget->GetSize()) == true))
						{
							Position -= SubWidget->GetPosition();
							NextWidget = SubWidget;
							
							break;
						}
					}
					PathWidget = NextWidget;
				}
			}
		}
	}
	else
	{
		Vector2f GlobalPosition(_CaptureWidget->GetGlobalPosition());
		auto PathWidget(_CaptureWidget);
		
		while(PathWidget != nullptr)
		{
			auto CapturingWidget(new MouseButtonEventPropagationPathItem());
			
			CapturingWidget->_Widget = PathWidget;
			CapturingWidget->_Phase = UI::Event::Phase::Capturing;
			CapturingWidget->_Position = MouseButtonEvent.GetPosition() - GlobalPosition;
			CapturingWidget->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingWidget));
			PropagationPath.push_front(CapturingWidget);
			GlobalPosition -= PathWidget->GetPosition();
			PathWidget = PathWidget->_SupWidget;
		}
	}
	if(PropagationPath.size() > 0)
	{
		auto TargetWidget(new MouseButtonEventPropagationPathItem());
		
		TargetWidget->_Widget = PropagationPath.back()->_Widget;
		TargetWidget->_Phase = UI::Event::Phase::Target;
		TargetWidget->_Position = PropagationPath.back()->_Position;
		TargetWidget->_Connection = PropagationPath.back()->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetWidget));
		PropagationPath.push_back(TargetWidget);
		
		auto Iterator(PropagationPath.end());
		
		--Iterator;
		do
		{
			--Iterator;
			
			auto BubblingWidget((new MouseButtonEventPropagationPathItem()));
			
			BubblingWidget->_Widget = (*Iterator)->_Widget;
			BubblingWidget->_Phase = UI::Event::Phase::Bubbling;
			BubblingWidget->_Position = (*Iterator)->_Position;
			BubblingWidget->_Connection = BubblingWidget->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingWidget));
			PropagationPath.push_back(BubblingWidget);
		} while(Iterator != PropagationPath.begin());
	}
	for(auto PropagationPathItem : PropagationPath)
	{
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			assert(PropagationPathItem->_Connection.IsValid() == true);
			MouseButtonEvent.SetCurrentTarget(PropagationPathItem->_Widget);
			MouseButtonEvent.SetPhase(PropagationPathItem->_Phase);
			MouseButtonEvent.SetPosition(PropagationPathItem->_Position);
			MouseButtonEvent.ResumeCallbacks();
			for(auto & Callback : PropagationPathItem->_Widget->_MouseButtonEvent.GetCallbacks())
			{
				Callback(MouseButtonEvent);
				if(MouseButtonEvent.GetStopCallbacks() == true)
				{
					break;
				}
			}
			if(MouseButtonEvent.GetStopPropagation() == true)
			{
				break;
			}
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
	}
	while(PropagationPath.empty() == false)
	{
		auto PropagationPathItem(PropagationPath.front());
		
		PropagationPath.pop_front();
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			PropagationPathItem->_Connection.Disconnect();
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::DispatchMouseEnterEvent(UI::Event & MouseEnterEvent)
{
	assert(MouseEnterEvent.GetTarget() != nullptr);
	
	std::list< EventPropagationPathItem * > PropagationPath;
	auto TargetPathItem(new EventPropagationPathItem());
		
	TargetPathItem->_Widget = MouseEnterEvent.GetTarget();
	TargetPathItem->_Phase = UI::Event::Phase::Target;
	TargetPathItem->_Connection = TargetPathItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetPathItem));
	PropagationPath.push_back(TargetPathItem);
	
	auto PathWidget(TargetPathItem->_Widget);
	
	while(PathWidget != nullptr)
	{
		auto CapturingPathItem(new EventPropagationPathItem());
		
		CapturingPathItem->_Widget = PathWidget;
		CapturingPathItem->_Phase = UI::Event::Phase::Capturing;
		CapturingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingPathItem));
		PropagationPath.push_front(CapturingPathItem);
	
		auto BubblingPathItem(new EventPropagationPathItem());
		
		BubblingPathItem->_Widget = PathWidget;
		BubblingPathItem->_Phase = UI::Event::Phase::Bubbling;
		BubblingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingPathItem));
		PropagationPath.push_back(BubblingPathItem);
		PathWidget = PathWidget->_SupWidget;
	}
	for(auto PropagationPathItem : PropagationPath)
	{
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			assert(PropagationPathItem->_Connection.IsValid() == true);
			MouseEnterEvent.SetCurrentTarget(PropagationPathItem->_Widget);
			MouseEnterEvent.SetPhase(PropagationPathItem->_Phase);
			MouseEnterEvent.ResumeCallbacks();
			for(auto & Callback : PropagationPathItem->_Widget->_MouseEnterEvent.GetCallbacks())
			{
				Callback(MouseEnterEvent);
				if(MouseEnterEvent.GetStopCallbacks() == true)
				{
					break;
				}
			}
			if(MouseEnterEvent.GetStopPropagation() == true)
			{
				break;
			}
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
	}
	while(PropagationPath.empty() == false)
	{
		auto PropagationPathItem(PropagationPath.front());
		
		PropagationPath.pop_front();
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			PropagationPathItem->_Connection.Disconnect();
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::DispatchMouseLeaveEvent(UI::Event & MouseLeaveEvent)
{
	assert(MouseLeaveEvent.GetTarget() != nullptr);
	
	std::list< EventPropagationPathItem * > PropagationPath;
	auto TargetPathItem(new EventPropagationPathItem());
		
	TargetPathItem->_Widget = MouseLeaveEvent.GetTarget();
	TargetPathItem->_Phase = UI::Event::Phase::Target;
	TargetPathItem->_Connection = TargetPathItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetPathItem));
	PropagationPath.push_back(TargetPathItem);
	
	auto PathWidget(TargetPathItem->_Widget);
	
	while(PathWidget != nullptr)
	{
		auto CapturingPathItem(new EventPropagationPathItem());
		
		CapturingPathItem->_Widget = PathWidget;
		CapturingPathItem->_Phase = UI::Event::Phase::Capturing;
		CapturingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingPathItem));
		PropagationPath.push_front(CapturingPathItem);
	
		auto BubblingPathItem(new EventPropagationPathItem());
		
		BubblingPathItem->_Widget = PathWidget;
		BubblingPathItem->_Phase = UI::Event::Phase::Bubbling;
		BubblingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingPathItem));
		PropagationPath.push_back(BubblingPathItem);
		PathWidget = PathWidget->_SupWidget;
	}
	for(auto PropagationPathItem : PropagationPath)
	{
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			assert(PropagationPathItem->_Connection.IsValid() == true);
			MouseLeaveEvent.SetCurrentTarget(PropagationPathItem->_Widget);
			MouseLeaveEvent.SetPhase(PropagationPathItem->_Phase);
			MouseLeaveEvent.ResumeCallbacks();
			for(auto & Callback : PropagationPathItem->_Widget->_MouseLeaveEvent.GetCallbacks())
			{
				Callback(MouseLeaveEvent);
				if(MouseLeaveEvent.GetStopCallbacks() == true)
				{
					break;
				}
			}
			if(MouseLeaveEvent.GetStopPropagation() == true)
			{
				break;
			}
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
	}
	while(PropagationPath.empty() == false)
	{
		auto PropagationPathItem(PropagationPath.front());
		
		PropagationPath.pop_front();
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			PropagationPathItem->_Connection.Disconnect();
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::DispatchMouseMoveEvent(UI::MouseMoveEvent & MouseMoveEvent)
{
	assert(MouseMoveEvent.GetTarget() == nullptr);
	
	std::list< MouseMoveEventPropagationPathItem * > PropagationPath;
	auto InsertIterator(PropagationPath.end());
	
	if(_CaptureWidget == nullptr)
	{
		if((_RootWidget != nullptr) && (_RootWidget->IsEnabled() == true) && (_RootWidget->IsVisible() == true))
		{
			auto Position(MouseMoveEvent.GetPosition());
			
			assert((_HoverWidget == nullptr) || (_HoverWidget == _RootWidget));
			if(IsInside(Position, _RootWidget->GetPosition(), _RootWidget->GetSize()) == true)
			{
				if(_HoverWidget == nullptr)
				{
					_HoverWidget = _RootWidget;
					
					UI::Event MouseEnterEvent;
					
					MouseEnterEvent.SetTarget(_RootWidget);
					DispatchMouseEnterEvent(MouseEnterEvent);
				}
				
				auto CurrentWidget(_RootWidget);
				
				while(CurrentWidget != nullptr)
				{
					UI::Widget * NextWidget(nullptr);
					
					Position -= CurrentWidget->GetPosition();
					
					// insert bubbling phase path item
					auto BubblingItem(new MouseMoveEventPropagationPathItem());
					
					BubblingItem->_Widget = CurrentWidget;
					BubblingItem->_Phase = UI::Event::Phase::Bubbling;
					BubblingItem->_Position = Position;
					BubblingItem->_Connection = CurrentWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingItem));
					InsertIterator = PropagationPath.insert(InsertIterator, BubblingItem);
					
					// calculate next widget
					for(auto SubWidget : CurrentWidget->_SubWidgets)
					{
						assert(SubWidget != nullptr);
						if((SubWidget->IsEnabled() == true) && (SubWidget->IsVisible() == true) && (IsInside(Position, SubWidget->GetPosition(), SubWidget->GetSize()) == true))
						{
							if(CurrentWidget->_HoverWidget != SubWidget)
							{
								CurrentWidget->_UnsetHoverWidget();
								CurrentWidget->_SetHoverWidget(SubWidget);
							}
							NextWidget = SubWidget;
							
							break;
						}
					}
					if(NextWidget == nullptr)
					{
						if(CurrentWidget->_HoverWidget != nullptr)
						{
							CurrentWidget->_UnsetHoverWidget();
						}
						
						// insert target phase path item
						auto TargetItem(new MouseMoveEventPropagationPathItem());
						
						TargetItem->_Widget = CurrentWidget;
						TargetItem->_Phase = UI::Event::Phase::Target;
						TargetItem->_Position = Position;
						TargetItem->_Connection = CurrentWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetItem));
						InsertIterator = PropagationPath.insert(InsertIterator, TargetItem);
					}
					
					// insert capturing phase item
					auto CapturingItem(new MouseMoveEventPropagationPathItem());
					
					CapturingItem->_Widget = CurrentWidget;
					CapturingItem->_Phase = UI::Event::Phase::Capturing;
					CapturingItem->_Position = Position;
					CapturingItem->_Connection = CurrentWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingItem));
					InsertIterator = PropagationPath.insert(InsertIterator, CapturingItem);
					CurrentWidget = NextWidget;
				}
			}
			else if(_HoverWidget == _RootWidget)
			{
				_RootWidget->_UnsetHoverWidget();
				_HoverWidget = nullptr;
			}
		}
	}
	else
	{
		if((_CaptureWidget->IsEnabled() == true) && (_CaptureWidget->IsVisible() == true))
		{
			auto Position(MouseMoveEvent.GetPosition() - _CaptureWidget->GetGlobalPosition());
			auto CurrentWidget(_CaptureWidget);
			
			while(CurrentWidget != nullptr)
			{
				if(CurrentWidget == _CaptureWidget)
				{
					// insert target phase path item
					auto TargetItem(new MouseMoveEventPropagationPathItem());
					
					TargetItem->_Widget = CurrentWidget;
					TargetItem->_Phase = UI::Event::Phase::Target;
					TargetItem->_Position = Position;
					TargetItem->_Connection = CurrentWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetItem));
					PropagationPath.push_back(TargetItem);
				}
				if(CurrentWidget->_SupWidget != nullptr)
				{
					if((IsInside(Position, CurrentWidget->GetSize()) == true) && (CurrentWidget->_SupWidget->_HoverWidget != CurrentWidget))
					{
						CurrentWidget->_SupWidget->_SetHoverWidget(CurrentWidget);
					}
					if((IsInside(Position, CurrentWidget->GetSize()) == false) && (CurrentWidget->_SupWidget->_HoverWidget == CurrentWidget))
					{
						CurrentWidget->_SupWidget->_UnsetHoverWidget();
					}
				}
				
				// insert capturing phase item
				auto CapturingItem(new MouseMoveEventPropagationPathItem());
				
				CapturingItem->_Widget = CurrentWidget;
				CapturingItem->_Phase = UI::Event::Phase::Capturing;
				CapturingItem->_Position = Position;
				CapturingItem->_Connection = CurrentWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingItem));
				PropagationPath.push_front(CapturingItem);
				
				// insert bubbling phase item
				auto BubblingItem(new MouseMoveEventPropagationPathItem());
				
				BubblingItem->_Widget = CurrentWidget;
				BubblingItem->_Phase = UI::Event::Phase::Bubbling;
				BubblingItem->_Position = Position;
				BubblingItem->_Connection = CurrentWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingItem));
				PropagationPath.push_back(BubblingItem);
				Position = Position + CurrentWidget->GetPosition();
				CurrentWidget = CurrentWidget->_SupWidget;
			}
		}
	}
	for(auto PropagationPathItem : PropagationPath)
	{
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			assert(PropagationPathItem->_Connection.IsValid() == true);
			MouseMoveEvent.SetCurrentTarget(PropagationPathItem->_Widget);
			MouseMoveEvent.SetPhase(PropagationPathItem->_Phase);
			MouseMoveEvent.SetPosition(PropagationPathItem->_Position);
			MouseMoveEvent.ResumeCallbacks();
			for(auto & Callback : PropagationPathItem->_Widget->_MouseMoveEvent.GetCallbacks())
			{
				Callback(MouseMoveEvent);
				if(MouseMoveEvent.GetStopCallbacks() == true)
				{
					break;
				}
			}
			if(MouseMoveEvent.GetStopPropagation() == true)
			{
				break;
			}
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
	}
	while(PropagationPath.empty() == false)
	{
		auto PropagationPathItem(PropagationPath.front());
		
		PropagationPath.pop_front();
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			PropagationPathItem->_Connection.Disconnect();
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::DispatchPositionChangedEvent(UI::Event & PositionChangedEvent)
{
	assert(PositionChangedEvent.GetTarget() != nullptr);
	
	std::list< EventPropagationPathItem * > PropagationPath;
	auto TargetPathItem(new EventPropagationPathItem());
		
	TargetPathItem->_Widget = PositionChangedEvent.GetTarget();
	TargetPathItem->_Phase = UI::Event::Phase::Target;
	TargetPathItem->_Connection = TargetPathItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetPathItem));
	PropagationPath.push_back(TargetPathItem);
	
	auto PathWidget(TargetPathItem->_Widget);
	
	while(PathWidget != nullptr)
	{
		auto CapturingPathItem(new EventPropagationPathItem());
		
		CapturingPathItem->_Widget = PathWidget;
		CapturingPathItem->_Phase = UI::Event::Phase::Capturing;
		CapturingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingPathItem));
		PropagationPath.push_front(CapturingPathItem);
	
		auto BubblingPathItem(new EventPropagationPathItem());
		
		BubblingPathItem->_Widget = PathWidget;
		BubblingPathItem->_Phase = UI::Event::Phase::Bubbling;
		BubblingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingPathItem));
		PropagationPath.push_back(BubblingPathItem);
		PathWidget = PathWidget->_SupWidget;
	}
	for(auto PropagationPathItem : PropagationPath)
	{
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			assert(PropagationPathItem->_Connection.IsValid() == true);
			PositionChangedEvent.SetCurrentTarget(PropagationPathItem->_Widget);
			PositionChangedEvent.SetPhase(PropagationPathItem->_Phase);
			PositionChangedEvent.ResumeCallbacks();
			for(auto & Callback : PropagationPathItem->_Widget->_PositionChangedEvent.GetCallbacks())
			{
				Callback(PositionChangedEvent);
				if(PositionChangedEvent.GetStopCallbacks() == true)
				{
					break;
				}
			}
			if(PositionChangedEvent.GetStopPropagation() == true)
			{
				break;
			}
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
	}
	while(PropagationPath.empty() == false)
	{
		auto PropagationPathItem(PropagationPath.front());
		
		PropagationPath.pop_front();
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			PropagationPathItem->_Connection.Disconnect();
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::DispatchSizeChangedEvent(UI::Event & SizeChangedEvent)
{
	assert(SizeChangedEvent.GetTarget() != nullptr);
	
	std::list< EventPropagationPathItem * > PropagationPath;
	auto TargetPathItem(new EventPropagationPathItem());
		
	TargetPathItem->_Widget = SizeChangedEvent.GetTarget();
	TargetPathItem->_Phase = UI::Event::Phase::Target;
	TargetPathItem->_Connection = TargetPathItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetPathItem));
	PropagationPath.push_back(TargetPathItem);
	
	auto PathWidget(TargetPathItem->_Widget);
	
	while(PathWidget != nullptr)
	{
		auto CapturingPathItem(new EventPropagationPathItem());
		
		CapturingPathItem->_Widget = PathWidget;
		CapturingPathItem->_Phase = UI::Event::Phase::Capturing;
		CapturingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingPathItem));
		PropagationPath.push_front(CapturingPathItem);
	
		auto BubblingPathItem(new EventPropagationPathItem());
		
		BubblingPathItem->_Widget = PathWidget;
		BubblingPathItem->_Phase = UI::Event::Phase::Bubbling;
		BubblingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingPathItem));
		PropagationPath.push_back(BubblingPathItem);
		PathWidget = PathWidget->_SupWidget;
	}
	for(auto PropagationPathItem : PropagationPath)
	{
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			assert(PropagationPathItem->_Connection.IsValid() == true);
			SizeChangedEvent.SetCurrentTarget(PropagationPathItem->_Widget);
			SizeChangedEvent.SetPhase(PropagationPathItem->_Phase);
			SizeChangedEvent.ResumeCallbacks();
			for(auto & Callback : PropagationPathItem->_Widget->_SizeChangedEvent.GetCallbacks())
			{
				Callback(SizeChangedEvent);
				if(SizeChangedEvent.GetStopCallbacks() == true)
				{
					break;
				}
			}
			if(SizeChangedEvent.GetStopPropagation() == true)
			{
				break;
			}
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
	}
	while(PropagationPath.empty() == false)
	{
		auto PropagationPathItem(PropagationPath.front());
		
		PropagationPath.pop_front();
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			PropagationPathItem->_Connection.Disconnect();
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::DispatchSubWidgetAddedEvent(UI::SubWidgetEvent & SubWidgetAddedEvent)
{
	assert(SubWidgetAddedEvent.GetTarget() != nullptr);
	
	std::list< EventPropagationPathItem * > PropagationPath;
	auto TargetPathItem(new EventPropagationPathItem());
		
	TargetPathItem->_Widget = SubWidgetAddedEvent.GetTarget();
	TargetPathItem->_Phase = UI::Event::Phase::Target;
	TargetPathItem->_Connection = TargetPathItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetPathItem));
	PropagationPath.push_back(TargetPathItem);
	
	auto PathWidget(TargetPathItem->_Widget);
	
	while(PathWidget != nullptr)
	{
		auto CapturingPathItem(new EventPropagationPathItem());
		
		CapturingPathItem->_Widget = PathWidget;
		CapturingPathItem->_Phase = UI::Event::Phase::Capturing;
		CapturingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingPathItem));
		PropagationPath.push_front(CapturingPathItem);
	
		auto BubblingPathItem(new EventPropagationPathItem());
		
		BubblingPathItem->_Widget = PathWidget;
		BubblingPathItem->_Phase = UI::Event::Phase::Bubbling;
		BubblingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingPathItem));
		PropagationPath.push_back(BubblingPathItem);
		PathWidget = PathWidget->_SupWidget;
	}
	for(auto PropagationPathItem : PropagationPath)
	{
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			assert(PropagationPathItem->_Connection.IsValid() == true);
			SubWidgetAddedEvent.SetCurrentTarget(PropagationPathItem->_Widget);
			SubWidgetAddedEvent.SetPhase(PropagationPathItem->_Phase);
			SubWidgetAddedEvent.ResumeCallbacks();
			for(auto & Callback : PropagationPathItem->_Widget->_SubWidgetAddedEvent.GetCallbacks())
			{
				Callback(SubWidgetAddedEvent);
				if(SubWidgetAddedEvent.GetStopCallbacks() == true)
				{
					break;
				}
			}
			if(SubWidgetAddedEvent.GetStopPropagation() == true)
			{
				break;
			}
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
	}
	while(PropagationPath.empty() == false)
	{
		auto PropagationPathItem(PropagationPath.front());
		
		PropagationPath.pop_front();
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			PropagationPathItem->_Connection.Disconnect();
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::DispatchSubWidgetRemovedEvent(UI::SubWidgetEvent & SubWidgetRemovedEvent)
{
	assert(SubWidgetRemovedEvent.GetTarget() != nullptr);
	
	std::list< EventPropagationPathItem * > PropagationPath;
	auto TargetPathItem(new EventPropagationPathItem());
		
	TargetPathItem->_Widget = SubWidgetRemovedEvent.GetTarget();
	TargetPathItem->_Phase = UI::Event::Phase::Target;
	TargetPathItem->_Connection = TargetPathItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, TargetPathItem));
	PropagationPath.push_back(TargetPathItem);
	
	auto PathWidget(TargetPathItem->_Widget);
	
	while(PathWidget != nullptr)
	{
		auto CapturingPathItem(new EventPropagationPathItem());
		
		CapturingPathItem->_Widget = PathWidget;
		CapturingPathItem->_Phase = UI::Event::Phase::Capturing;
		CapturingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, CapturingPathItem));
		PropagationPath.push_front(CapturingPathItem);
	
		auto BubblingPathItem(new EventPropagationPathItem());
		
		BubblingPathItem->_Widget = PathWidget;
		BubblingPathItem->_Phase = UI::Event::Phase::Bubbling;
		BubblingPathItem->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, std::placeholders::_1, BubblingPathItem));
		PropagationPath.push_back(BubblingPathItem);
		PathWidget = PathWidget->_SupWidget;
	}
	for(auto PropagationPathItem : PropagationPath)
	{
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			assert(PropagationPathItem->_Connection.IsValid() == true);
			SubWidgetRemovedEvent.SetCurrentTarget(PropagationPathItem->_Widget);
			SubWidgetRemovedEvent.SetPhase(PropagationPathItem->_Phase);
			SubWidgetRemovedEvent.ResumeCallbacks();
			for(auto & Callback : PropagationPathItem->_Widget->_SubWidgetRemovedEvent.GetCallbacks())
			{
				Callback(SubWidgetRemovedEvent);
				if(SubWidgetRemovedEvent.GetStopCallbacks() == true)
				{
					break;
				}
			}
			if(SubWidgetRemovedEvent.GetStopPropagation() == true)
			{
				break;
			}
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
	}
	while(PropagationPath.empty() == false)
	{
		auto PropagationPathItem(PropagationPath.front());
		
		PropagationPath.pop_front();
		assert(PropagationPathItem != nullptr);
		if(PropagationPathItem->_Widget != nullptr)
		{
			PropagationPathItem->_Connection.Disconnect();
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::Update(float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_RootWidget != nullptr);
	
	std::stack< std::pair< UI::Widget *, bool > > ToDo;
	
	ToDo.push(std::make_pair(_RootWidget, false));
	while(ToDo.size() > 0)
	{
		std::pair< UI::Widget *, bool > & Widget(ToDo.top());
		
		if((Widget.first->_SubWidgets.size() == 0) || (Widget.second == true))
		{
			Widget.first->_UpdatingEvent(RealTimeSeconds, GameTimeSeconds);
			ToDo.pop();
		}
		else
		{
			Widget.second = true;
			for(std::list< UI::Widget * >::reverse_iterator SubWidgetIterator = Widget.first->_SubWidgets.rbegin(); SubWidgetIterator != Widget.first->_SubWidgets.rend(); ++SubWidgetIterator)
			{
				ToDo.push(std::make_pair(*SubWidgetIterator, false));
			}
		}
	}
}

void UI::UserInterface::_OnCaptureWidgetDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		assert(_CaptureWidget != nullptr);
		_CaptureWidget = nullptr;
		_CaptureWidgetDestroyingCallbackConnection.Disconnect();
	}
}

void UI::UserInterface::_OnHoverWidgetDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		assert(_HoverWidget != nullptr);
		_HoverWidget = nullptr;
	}
}

void UI::UserInterface::_OnRootWidgetDestroying(UI::Event & DestroyingEvent)
{
	if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
	{
		assert(_RootWidget != nullptr);
		_RootWidget = nullptr;
	}
}
