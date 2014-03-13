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
#include "user_interface.h"
#include "widget.h"

struct EventPropagationPathItem
{
	Connection _Connection;
	UI::Event::Phase _Phase;
	UI::Widget * _Widget;
};

void DestroyingPropagationPathItem(EventPropagationPathItem * EventPropagationPathItem)
{
	assert(EventPropagationPathItem != nullptr);
	assert(EventPropagationPathItem->_Widget != nullptr);
	assert(EventPropagationPathItem->_Connection.IsValid() == true);
	EventPropagationPathItem->_Widget->DisconnectDestroyingCallback(EventPropagationPathItem->_Connection);
	assert(EventPropagationPathItem->_Connection.IsValid() == false);
	EventPropagationPathItem->_Widget = nullptr;
}

struct KeyEventPropagationPathItem : EventPropagationPathItem
{
};

struct MouseButtonEventPropagationPathItem : EventPropagationPathItem
{
	Vector2f _Position;
};

UI::UserInterface::UserInterface(void) :
	_CaptureWidget(nullptr),
	_HoverWidget(nullptr),
	_RootWidget(new UI::Widget(0))
{
	_RootWidget->SetSize(Vector2f(1280.0f, 1024.0f));
	_RootWidget->ConnectDestroyingCallback(std::bind(&UI::UserInterface::_OnRootWidgetDestroying, this));
}

UI::UserInterface::~UserInterface(void)
{
	if(_CaptureWidgetDestroyingCallbackConnection.IsValid() == true)
	{
		assert(_CaptureWidget != nullptr);
		_CaptureWidget->DisconnectDestroyingCallback(_CaptureWidgetDestroyingCallbackConnection);
	}
	else
	{
		assert(_CaptureWidget == nullptr);
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
	assert(Widget != 0);
	assert(_CaptureWidget == 0);
	_CaptureWidget = Widget;
	_CaptureWidgetDestroyingCallbackConnection = _CaptureWidget->ConnectDestroyingCallback(std::bind(&UI::UserInterface::_OnCaptureWidgetDestroying, this));
}

void UI::UserInterface::ReleaseCaptureWidget(void)
{
	if(_CaptureWidget != 0)
	{
		assert(_CaptureWidgetDestroyingCallbackConnection.IsValid() == true);
		_CaptureWidget->DisconnectDestroyingCallback(_CaptureWidgetDestroyingCallbackConnection);
		_CaptureWidget = 0;
	}
}

UI::Widget * UI::UserInterface::GetWidget(const std::string & Path)
{
	if(Path[0] != '/')
	{
		return 0;
	}
	
	std::string::size_type Position(1);
	UI::Widget * Root(_RootWidget);
	
	while((Root != 0) && (Position < Path.length()))
	{
		std::string::size_type SlashPosition(Path.find('/', Position));
		
		Root = Root->GetSubWidget(Path.substr(Position, SlashPosition - Position));
		Position = ((SlashPosition == std::string::npos) ? (Path.length()) : (SlashPosition + 1));
	}
	
	return Root;
}

void UI::UserInterface::DispatchMouseButtonEvent(UI::MouseButtonEvent & MouseButtonEvent)
{
	assert(MouseButtonEvent.GetTarget() == nullptr);
	
	std::list< MouseButtonEventPropagationPathItem * > PropagationPath;
	
	if(_CaptureWidget == nullptr)
	{
		if((_RootWidget != nullptr) && (_RootWidget->_Visible == true) && (_RootWidget->_Enabled == true))
		{
			auto Position(MouseButtonEvent.GetPosition() - _RootWidget->_Position);
			
			if((Position[0] >= 0.0f) && (Position[0] <= _RootWidget->_Size[0]) && (Position[1] >= 0.0f) && (Position[1] <= _RootWidget->_Size[1]))
			{
				auto PathWidget(_RootWidget);
				
				while(PathWidget != nullptr)
				{
					auto CapturingWidget(new MouseButtonEventPropagationPathItem());
					
					CapturingWidget->_Widget = PathWidget;
					CapturingWidget->_Phase = UI::Event::Phase::Capturing;
					CapturingWidget->_Position = Position;
					CapturingWidget->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, CapturingWidget));
					PropagationPath.push_back(CapturingWidget);
					
					Widget * NextWidget(nullptr);
					
					for(auto SubWidget : PathWidget->_SubWidgets)
					{
						const Vector2f & SubWidgetPosition(SubWidget->GetPosition());
						const Vector2f & SubWidgetSize(SubWidget->GetSize());
						
						if((SubWidget->_Visible == true) && (SubWidget->_Enabled == true) && (Position[0] >= SubWidgetPosition[0]) && (Position[0] < SubWidgetPosition[0] + SubWidgetSize[0]) && (Position[1] >= SubWidgetPosition[1]) && (Position[1] < SubWidgetPosition[1] + SubWidgetSize[1]))
						{
							Position -= SubWidgetPosition;
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
			CapturingWidget->_Connection = PathWidget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, CapturingWidget));
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
		TargetWidget->_Connection = PropagationPath.back()->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, TargetWidget));
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
			BubblingWidget->_Connection = BubblingWidget->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, BubblingWidget));
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
			for(auto & Callback : PropagationPathItem->_Widget->_MouseButtonEvent.CopyCallbacks())
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
			assert(PropagationPathItem->_Connection.IsValid() == true);
			PropagationPathItem->_Widget->DisconnectDestroyingCallback(PropagationPathItem->_Connection);
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::DispatchMouseEnterEvent(UI::Event & MouseEnterEvent)
{
	assert(MouseEnterEvent.GetTarget() != nullptr);
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
		CapturingItem->_Connection = CapturingItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, CapturingItem));
		PropagationPath.push_back(CapturingItem);
		PathWidget = PathWidget->_KeyFocus;
	}
	if(PropagationPath.size() > 0)
	{
		auto TargetItem(new KeyEventPropagationPathItem());
		
		TargetItem->_Widget = PropagationPath.back()->_Widget;
		TargetItem->_Phase = UI::Event::Phase::Target;
		TargetItem->_Connection = TargetItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, TargetItem));
		PropagationPath.push_back(TargetItem);
		PathWidget = PropagationPath.back()->_Widget;
		while(PathWidget != nullptr)
		{
			assert(PathWidget->_Enabled == true);
			
			auto BubblingItem(new KeyEventPropagationPathItem());
			
			BubblingItem->_Widget = PathWidget;
			BubblingItem->_Phase = UI::Event::Phase::Bubbling;
			BubblingItem->_Connection = BubblingItem->_Widget->ConnectDestroyingCallback(std::bind(DestroyingPropagationPathItem, BubblingItem));
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
			for(auto & Callback : PropagationPathItem->_Widget->_KeyEvent.CopyCallbacks())
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
			assert(PropagationPathItem->_Connection.IsValid() == true);
			PropagationPathItem->_Widget->DisconnectDestroyingCallback(PropagationPathItem->_Connection);
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
		else
		{
			assert(PropagationPathItem->_Connection.IsValid() == false);
		}
		delete PropagationPathItem;
	}
}

void UI::UserInterface::MouseMoved(float X, float Y)
{
	if(_CaptureWidget == nullptr)
	{
		assert(_RootWidget != nullptr);
		if(_RootWidget->_Enabled == true)
		{
			const Vector2f & LeftTopCorner(_RootWidget->_Position);
			Vector2f RightBottomCorner(LeftTopCorner + _RootWidget->_Size);
			
			if((X >= LeftTopCorner[0]) && (X < RightBottomCorner[0]) && (Y >= LeftTopCorner[1]) && (Y < RightBottomCorner[1]))
			{
				if(_HoverWidget != _RootWidget)
				{
					_HoverWidget = _RootWidget;
					_RootWidget->MouseEnter();
				}
				_RootWidget->MouseMoved(X, Y);
			}
			else
			{
				if(_HoverWidget == _RootWidget)
				{
					_HoverWidget = nullptr;
					_RootWidget->MouseLeave();
				}
			}
		}
	}
	else
	{
		if(_CaptureWidget->_Enabled == true)
		{
			Vector2f LeftTopCorner(_CaptureWidget->GetGlobalPosition());
			Vector2f RightBottomCorner(LeftTopCorner + _CaptureWidget->_Size);
			
			if((X >= LeftTopCorner[0]) && (X < RightBottomCorner[0]) && (Y >= LeftTopCorner[1]) && (Y < RightBottomCorner[1]) && (_CaptureWidget->_SupWidget->_HoverWidget != _CaptureWidget))
			{
				_CaptureWidget->_SupWidget->_HoverWidget = _CaptureWidget;
				_CaptureWidget->MouseEnter();
			}
			_CaptureWidget->MouseMoved(X - LeftTopCorner[0], Y - LeftTopCorner[1]);
			if(((X < LeftTopCorner[0]) || (X >= RightBottomCorner[0]) || (Y < LeftTopCorner[1]) || (Y >= RightBottomCorner[1])) && (_CaptureWidget->_SupWidget->_HoverWidget == _CaptureWidget))
			{
				_CaptureWidget->_SupWidget->_HoverWidget = nullptr;
				_CaptureWidget->MouseLeave();
			}
		}
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

void UI::UserInterface::_OnCaptureWidgetDestroying(void)
{
	assert(_CaptureWidget != nullptr);
	assert(_CaptureWidgetDestroyingCallbackConnection.IsValid() == true);
	_CaptureWidget->DisconnectDestroyingCallback(_CaptureWidgetDestroyingCallbackConnection);
	_CaptureWidget = nullptr;
}

void UI::UserInterface::_OnHoverWidgetDestroying(void)
{
	_HoverWidget = nullptr;
}

void UI::UserInterface::_OnRootWidgetDestroying(void)
{
	_RootWidget = nullptr;
}
