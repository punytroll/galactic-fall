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

#include <algorithm>

#include "../color.h"
#include "../graphics/gl.h"
#include "../math.h"
#include "widget.h"

std::list< UI::Widget * > UI::Widget::_DestroyedWidgets;
std::stack< std::pair< Vector2f, Vector2f > > UI::Widget::_ClippingRectangles;

UI::Widget::Widget(UI::Widget * SupWidget, const std::string & Name) :
	_AnchorBottom(false),
	_AnchorLeft(true),
	_AnchorRight(false),
	_AnchorTop(true),
	_BackgroundColor(nullptr),
	_DisabledBackgroundColor(nullptr),
	_Enabled(true),
	_HoverWidget(nullptr),
	_KeyFocus(nullptr),
	_Name(Name),
	_Position(true),
	_Size(true),
	_SupWidget(nullptr),
	_Visible(true)
{
	if(SupWidget != nullptr)
	{
		SupWidget->AddSubWidget(this);
	}
}

UI::Widget::~Widget(void)
{
	assert(_SupWidget == nullptr);
	assert(_SubWidgets.size() == 0);
	delete _BackgroundColor;
	_BackgroundColor = nullptr;
	delete _DisabledBackgroundColor;
	_DisabledBackgroundColor = nullptr;
}

void UI::Widget::Draw(void)
{
	auto Color((_Enabled == true) ? (_BackgroundColor) : (_DisabledBackgroundColor));
	
	if(Color != nullptr)
	{
		GLColor4fv(Color->GetColor().GetPointer());
		GLBegin(GL_QUADS);
		GLVertex2f(0.0f, 0.0f);
		GLVertex2f(0.0f, _Size[1]);
		GLVertex2f(_Size[0], _Size[1]);
		GLVertex2f(_Size[0], 0.0f);
		GLEnd();
	}
	if(_SubWidgets.empty() == false)
	{
		for(std::list< Widget * >::const_reverse_iterator SubWidgetIterator = _SubWidgets.rbegin(); SubWidgetIterator != _SubWidgets.rend(); ++SubWidgetIterator)
		{
			Widget * SubWidget(*SubWidgetIterator);
			
			if(SubWidget->_Visible == true)
			{
				GLPushMatrix();
				_PushClippingRectangle(SubWidget->_Position, SubWidget->_Size);
				GLTranslatef(SubWidget->_Position[0], SubWidget->_Position[1], 0.0f);
				_DrawClippingRectangle();
				SubWidget->Draw();
				_PopClippingRectangle();
				GLPopMatrix();
			}
		}
		// restore the clipping rectangle for this widget
		_DrawClippingRectangle();
	}
}

Vector2f UI::Widget::GetGlobalPosition(void) const
{
	const UI::Widget * CurrentWidget(this);
	Vector2f Result(true);
	
	while(CurrentWidget != nullptr)
	{
		Result += CurrentWidget->GetPosition();
		CurrentWidget = CurrentWidget->_SupWidget;
	}
	
	return Result;
}

std::string UI::Widget::GetPath(void) const
{
	std::string Result;
	auto Widget(this);
	
	while(Widget != nullptr)
	{
		Result = "/" + _Name + Result;
		Widget = Widget->_SupWidget;
	}
	
	return Result;
}

void UI::Widget::SetBackgroundColor(const Color & BackgroundColor)
{
	delete _BackgroundColor;
	_BackgroundColor = new Color(BackgroundColor);
}

void UI::Widget::SetDisabledBackgroundColor(const Color & DisabledBackgroundColor)
{
	delete _DisabledBackgroundColor;
	_DisabledBackgroundColor = new Color(DisabledBackgroundColor);
}

void UI::Widget::UnsetBackgroundColor(void)
{
	delete _BackgroundColor;
	_BackgroundColor = nullptr;
}

void UI::Widget::UnsetDisabledBackgroundColor(void)
{
	delete _DisabledBackgroundColor;
	_DisabledBackgroundColor = nullptr;
}

void UI::Widget::SetPosition(const Vector2f & Position)
{
	if(Position != _Position)
	{
		_Position = Position;
		_PositionChangedEvent();
	}
}

void UI::Widget::SetSize(const Vector2f & Size)
{
	// early bailing out if this is a no-op
	if(Size != _Size)
	{
		Vector2f Offset(GetSize() - Size);
		
		_Size = Size;
		// iterate through the list of sub widgets and correct widget positions and sizes
		for(auto SubWidget : _SubWidgets)
		{
			Vector2f SubWidgetNewPosition(SubWidget->GetPosition());
			Vector2f SubWidgetNewSize(SubWidget->GetSize());
			
			if(SubWidget->_AnchorRight == true)
			{
				if(SubWidget->_AnchorLeft == true)
				{
					SubWidgetNewSize[0] -= Offset[0];
				}
				else
				{
					SubWidgetNewPosition[0] -= Offset[0];
				}
			}
			if(SubWidget->_AnchorBottom == true)
			{
				if(SubWidget->_AnchorTop == true)
				{
					SubWidgetNewSize[1] -= Offset[1];
				}
				else
				{
					SubWidgetNewPosition[1] -= Offset[1];
				}
			}
			SubWidget->SetPosition(SubWidgetNewPosition);
			SubWidget->SetSize(SubWidgetNewSize);
		}
		_SizeChangedEvent();
	}
}

void UI::Widget::GrabKeyFocus(void)
{
	if(_SupWidget != nullptr)
	{
		_SupWidget->_KeyFocus = this;
		_SupWidget->GrabKeyFocus();
	}
}

void UI::Widget::AddSubWidget(UI::Widget * SubWidget)
{
	assert(SubWidget->_SupWidget == nullptr);
	SubWidget->_SupWidget = this;
	_SubWidgets.push_front(SubWidget);
}

void UI::Widget::RemoveSubWidget(UI::Widget * SubWidget)
{
	assert(SubWidget->_SupWidget == this);
	SubWidget->_SupWidget = nullptr;
	_SubWidgets.erase(std::find(_SubWidgets.begin(), _SubWidgets.end(), SubWidget));
	if(SubWidget == _KeyFocus)
	{
		_KeyFocus = nullptr;
	}
	if(SubWidget == _HoverWidget)
	{
		_HoverWidget = nullptr;
	}
}

void UI::Widget::RaiseSubWidget(UI::Widget * SubWidget)
{
	assert(SubWidget->_SupWidget == this);
	_SubWidgets.remove(SubWidget);
	_SubWidgets.push_front(SubWidget);
}

void UI::Widget::Destroy(void)
{
	// fire destroying event before aything is actually destroyed
	_DestroyingEvent();
	// now destroy
	// first the sub widgets, they will remove themselves from this widget
	while(_SubWidgets.size() > 0)
	{
		_SubWidgets.front()->Destroy();
	}
	// now remove ourself from the sup widget
	if(_SupWidget != nullptr)
	{
		_SupWidget->RemoveSubWidget(this);
	}
	// now append ourself to the list of destroyed widgets to be delete'd by the garbage collection
	_DestroyedWidgets.push_back(this);
}

void UI::Widget::MouseMoved(float X, float Y)
{
	bool FoundSubWidget(false);
	
	// iterate all sub widgets, look for an intersection and propagate the mouse event with corrected coordinates
	for(auto SubWidget : _SubWidgets)
	{
		const Vector2f & LeftTopCorner(SubWidget->GetPosition());
		Vector2f RightBottomCorner(LeftTopCorner + SubWidget->GetSize());
		
		if((SubWidget->IsVisible() == true) && (SubWidget->_Enabled == true) && (X >= LeftTopCorner[0]) && (X < RightBottomCorner[0]) && (Y >= LeftTopCorner[1]) && (Y < RightBottomCorner[1]))
		{
			// test whether the new hover widget equals the old
			if(_HoverWidget != SubWidget)
			{
				// befor unsetting the hover widget inform the old hover widget that the mouse cursor is leaving
				if(_HoverWidget != nullptr)
				{
					_HoverWidget->MouseLeave();
				}
				// now set the new hover widget
				_HoverWidget = SubWidget;
				// inform the new hover widget about the entering of the mouse cursor
				_HoverWidget->MouseEnter();
			}
			SubWidget->MouseMoved(X - LeftTopCorner[0], Y - LeftTopCorner[1]);
			FoundSubWidget = true;
			
			break;
		}
	}
	// if all sub widget have been iterated through, the old hover widget has been left by the mouse cursor
	if((FoundSubWidget == false) && (_HoverWidget != nullptr))
	{
		_HoverWidget->MouseLeave();
		_HoverWidget = nullptr;
	}
	// after this we may call all our listeners
	_MouseMovedEvent(X, Y);
}

void UI::Widget::MouseEnter(void)
{
	_MouseEnterEvent();
}

void UI::Widget::MouseLeave(void)
{
	if(_HoverWidget != nullptr)
	{
		auto HoverWidget(_HoverWidget);
		
		_HoverWidget = nullptr;
		HoverWidget->MouseLeave();
	}
	_MouseLeaveEvent();
}

Connection UI::Widget::ConnectDestroyingCallback(std::function< void (void) > Callback)
{
	return _DestroyingEvent.Connect(Callback);
}

Connection UI::Widget::ConnectKeyCallback(std::function< void (UI::KeyEvent &) > Callback)
{
	return _KeyEvent.Connect(Callback);
}

Connection UI::Widget::ConnectMouseButtonCallback(std::function< void (UI::MouseButtonEvent &) > Callback)
{
	return _MouseButtonEvent.Connect(Callback);
}

Connection UI::Widget::ConnectMouseEnterCallback(std::function< void (void) > Callback)
{
	return _MouseEnterEvent.Connect(Callback);
}

Connection UI::Widget::ConnectMouseLeaveCallback(std::function< void (void) > Callback)
{
	return _MouseLeaveEvent.Connect(Callback);
}

Connection UI::Widget::ConnectMouseMovedCallback(std::function< void (float, float) > Callback)
{
	return _MouseMovedEvent.Connect(Callback);
}

Connection UI::Widget::ConnectPositionChangedCallback(std::function< void (void) > Callback)
{
	return _PositionChangedEvent.Connect(Callback);
}

Connection UI::Widget::ConnectSizeChangedCallback(std::function< void (void) > Callback)
{
	return _SizeChangedEvent.Connect(Callback);
}

Connection UI::Widget::ConnectUpdatingCallback(std::function< void (float, float) > Callback)
{
	return _UpdatingEvent.Connect(Callback);
}

void UI::Widget::DisconnectDestroyingCallback(Connection & Connection)
{
	_DestroyingEvent.Disconnect(Connection);
}

void UI::Widget::DisconnectKeyCallback(Connection & Connection)
{
	_KeyEvent.Disconnect(Connection);
}

void UI::Widget::DisconnectMouseButtonCallback(Connection & Connection)
{
	_MouseButtonEvent.Disconnect(Connection);
}

void UI::Widget::DisconnectMouseEnterCallback(Connection & Connection)
{
	_MouseEnterEvent.Disconnect(Connection);
}

void UI::Widget::DisconnectMouseLeaveCallback(Connection & Connection)
{
	_MouseLeaveEvent.Disconnect(Connection);
}

void UI::Widget::DisconnectMouseMovedCallback(Connection & Connection)
{
	_MouseMovedEvent.Disconnect(Connection);
}

void UI::Widget::DisconnectPositionChangedCallback(Connection & Connection)
{
	_PositionChangedEvent.Disconnect(Connection);
}

void UI::Widget::DisconnectSizeChangedCallback(Connection & Connection)
{
	_SizeChangedEvent.Disconnect(Connection);
}

void UI::Widget::DisconnectUpdatingCallback(Connection & Connection)
{
	_UpdatingEvent.Disconnect(Connection);
}

void UI::Widget::_PushClippingRectangle(const Vector2f & Position, const Vector2f & Size)
{
	Vector2f LeftTop(true);
	Vector2f RightBottom(Size);
	
	if(_ClippingRectangles.empty() == false)
	{
		LeftTop[0] = std::max(0.0f, _ClippingRectangles.top().first[0] - Position[0]);
		LeftTop[1] = std::max(0.0f, _ClippingRectangles.top().first[1] - Position[1]);
		RightBottom[0] = std::min(RightBottom[0], _ClippingRectangles.top().second[0] - Position[0]);
		RightBottom[1] = std::min(RightBottom[1], _ClippingRectangles.top().second[1] - Position[1]);
	}
	_ClippingRectangles.push(std::make_pair(LeftTop, RightBottom));
}

void UI::Widget::_PopClippingRectangle(void)
{
	_ClippingRectangles.pop();
}

void UI::Widget::_DrawClippingRectangle(void)
{
	double LeftPlane[4] = { 1.0, 0.0, 0.0, -_ClippingRectangles.top().first[0] };
	double TopPlane[4] = { 0.0, 1.0, 0.0, -_ClippingRectangles.top().first[1] };
	double RightPlane[4] = { -1.0, 0.0, 0.0, _ClippingRectangles.top().second[0] };
	double BottomPlane[4] = { 0.0, -1.0, 0.0, _ClippingRectangles.top().second[1] };
	
	GLClipPlane(GL_CLIP_PLANE0, LeftPlane);
	GLClipPlane(GL_CLIP_PLANE1, TopPlane);
	GLClipPlane(GL_CLIP_PLANE2, RightPlane);
	GLClipPlane(GL_CLIP_PLANE3, BottomPlane);
}
