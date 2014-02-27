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

#include "../graphics/gl.h"
#include "user_interface.h"
#include "widget.h"

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
		UI::Widget::PushClippingRectangle(_RootWidget->GetPosition(), _RootWidget->GetSize());
		GLTranslatef(_RootWidget->GetPosition()[0], _RootWidget->GetPosition()[1], 0.0f);
		UI::Widget::DrawClippingRectangle();
		_RootWidget->Draw();
		UI::Widget::PopClippingRectangle();
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

bool UI::UserInterface::MouseButton(int Button, int State, float X, float Y)
{
	if(_CaptureWidget == 0)
	{
		assert(_RootWidget != 0);
		if(_RootWidget->GetEnabled() == true)
		{
			const Vector2f & LeftTopCorner(_RootWidget->GetPosition());
			Vector2f RightBottomCorner(LeftTopCorner + _RootWidget->GetSize());
			
			if((X >= LeftTopCorner[0]) && (X < RightBottomCorner[0]) && (Y >= LeftTopCorner[1]) && (Y < RightBottomCorner[1]))
			{
				return _RootWidget->MouseButton(Button, State, X - LeftTopCorner[0], Y - LeftTopCorner[1]);
			}
		}
	}
	else
	{
		if(_CaptureWidget->GetEnabled() == true)
		{
			Vector2f TopLeftCorner(_CaptureWidget->GetGlobalPosition());
			
			return _CaptureWidget->MouseButton(Button, State, X - TopLeftCorner[0], Y - TopLeftCorner[1]);
		}
	}
	
	return false;
}

bool UI::UserInterface::Key(const KeyEventInformation & KeyEventInformation)
{
	assert(_RootWidget != 0);
	if(_RootWidget->GetEnabled() == true)
	{
		return _RootWidget->Key(KeyEventInformation);
	}
	
	return false;
}

void UI::UserInterface::MouseMoved(float X, float Y)
{
	if(_CaptureWidget == 0)
	{
		assert(_RootWidget != 0);
		if(_RootWidget->GetEnabled() == true)
		{
			const Vector2f & LeftTopCorner(_RootWidget->GetPosition());
			Vector2f RightBottomCorner(LeftTopCorner + _RootWidget->GetSize());
			
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
					_HoverWidget = 0;
					_RootWidget->MouseLeave();
				}
			}
		}
	}
	else
	{
		if(_CaptureWidget->GetEnabled() == true)
		{
			Vector2f LeftTopCorner(_CaptureWidget->GetGlobalPosition());
			Vector2f RightBottomCorner(LeftTopCorner + _CaptureWidget->GetSize());
			
			if((X >= LeftTopCorner[0]) && (X < RightBottomCorner[0]) && (Y >= LeftTopCorner[1]) && (Y < RightBottomCorner[1]) && (_CaptureWidget->m_SupWidget->m_HoverWidget != _CaptureWidget))
			{
				_CaptureWidget->m_SupWidget->m_HoverWidget = _CaptureWidget;
				_CaptureWidget->MouseEnter();
			}
			_CaptureWidget->MouseMoved(X - LeftTopCorner[0], Y - LeftTopCorner[1]);
			if(((X < LeftTopCorner[0]) || (X >= RightBottomCorner[0]) || (Y < LeftTopCorner[1]) || (Y >= RightBottomCorner[1])) && (_CaptureWidget->m_SupWidget->m_HoverWidget == _CaptureWidget))
			{
				_CaptureWidget->m_SupWidget->m_HoverWidget = 0;
				_CaptureWidget->MouseLeave();
			}
		}
	}
}

void UI::UserInterface::Update(float RealTimeSeconds, float GameTimeSeconds)
{
	assert(_RootWidget != 0);
	
	std::stack< std::pair< UI::Widget *, bool > > ToDo;
	
	ToDo.push(std::make_pair(_RootWidget, false));
	while(ToDo.size() > 0)
	{
		std::pair< UI::Widget *, bool > & Widget(ToDo.top());
		
		if((Widget.first->m_SubWidgets.size() == 0) || (Widget.second == true))
		{
			Widget.first->_UpdatingEvent(RealTimeSeconds, GameTimeSeconds);
			ToDo.pop();
		}
		else
		{
			Widget.second = true;
			for(std::list< UI::Widget * >::reverse_iterator SubWidgetIterator = Widget.first->m_SubWidgets.rbegin(); SubWidgetIterator != Widget.first->m_SubWidgets.rend(); ++SubWidgetIterator)
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
	_CaptureWidget = 0;
}

void UI::UserInterface::_OnHoverWidgetDestroying(void)
{
	_HoverWidget = 0;
}

void UI::UserInterface::_OnRootWidgetDestroying(void)
{
	_RootWidget = 0;
}
