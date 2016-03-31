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

#include "../globals.h"
#include "../graphics/color_rgbo.h"
#include "../graphics/drawing.h"
#include "../graphics/render_context.h"
#include "../math.h"
#include "event.h"
#include "sub_widget_event.h"
#include "user_interface.h"
#include "widget.h"

std::list< UI::Widget * > UI::Widget::_DestroyedWidgets;
std::stack< std::tuple< float, float, float, float > > UI::Widget::_ClippingRectangles;

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
	_Left(0.0f),
	_Name(Name),
	_Size(true),
	_SupWidget(nullptr),
	_Top(0.0f),
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

void UI::Widget::Draw(Graphics::RenderContext * RenderContext)
{
	auto Color((_Enabled == true) ? (_BackgroundColor) : (_DisabledBackgroundColor));
	auto GlobalPosition{GetGlobalPosition()};
	
	if(Color != nullptr)
	{
		assert(RenderContext != nullptr);
		RenderContext->SetColorRGBO(*Color);
		RenderContext->SetProgramIdentifier("widget");
		RenderContext->ActivateProgram();
		Graphics::Drawing::DrawBoxFromPositionAndSize(RenderContext, GlobalPosition, _Size);
		RenderContext->DeactivateProgram();
		RenderContext->UnsetProgramIdentifier();
		RenderContext->UnsetColorRGBO();
	}
	if(_SubWidgets.empty() == false)
	{
		for(std::list< Widget * >::const_reverse_iterator SubWidgetIterator = _SubWidgets.rbegin(); SubWidgetIterator != _SubWidgets.rend(); ++SubWidgetIterator)
		{
			Widget * SubWidget(*SubWidgetIterator);
			
			if(SubWidget->_Visible == true)
			{
				_PushClippingRectangle(RenderContext, GlobalPosition[0] + SubWidget->_Left, GlobalPosition[1] + SubWidget->_Top, GlobalPosition[1] + SubWidget->_Top + SubWidget->_Size[1], GlobalPosition[0] + SubWidget->_Left + SubWidget->_Size[0]);
				SubWidget->Draw(RenderContext);
				_PopClippingRectangle(RenderContext);
			}
		}
	}
}

Vector2f UI::Widget::GetGlobalPosition(void) const
{
	const UI::Widget * CurrentWidget(this);
	Vector2f Result(true);
	
	while(CurrentWidget != nullptr)
	{
		Result[0] += CurrentWidget->GetLeft();
		Result[1] += CurrentWidget->GetTop();
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

void UI::Widget::SetBackgroundColor(const Graphics::ColorRGBO & BackgroundColor)
{
	delete _BackgroundColor;
	_BackgroundColor = new Graphics::ColorRGBO(BackgroundColor);
}

void UI::Widget::SetDisabledBackgroundColor(const Graphics::ColorRGBO & DisabledBackgroundColor)
{
	delete _DisabledBackgroundColor;
	_DisabledBackgroundColor = new Graphics::ColorRGBO(DisabledBackgroundColor);
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
			Vector2f SubWidgetNewSize(SubWidget->GetSize());
			
			if(SubWidget->_AnchorRight == true)
			{
				if(SubWidget->_AnchorLeft == true)
				{
					SubWidgetNewSize[0] -= Offset[0];
				}
				else
				{
					SubWidget->SetLeft(SubWidget->GetLeft() - Offset[0]);
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
					SubWidget->SetTop(SubWidget->GetTop() - Offset[1]);
				}
			}
			SubWidget->SetSize(SubWidgetNewSize);
		}
		
		UI::Event SizeChangedEvent;
		
		SizeChangedEvent.SetTarget(this);
		g_UserInterface->DispatchSizeChangedEvent(SizeChangedEvent);
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
	
	UI::SubWidgetEvent SubWidgetAddedEvent;
	
	SubWidgetAddedEvent.SetTarget(this);
	SubWidgetAddedEvent.SetSubWidget(SubWidget);
	g_UserInterface->DispatchSubWidgetAddedEvent(SubWidgetAddedEvent);
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
	
	UI::SubWidgetEvent SubWidgetRemovedEvent;
	
	SubWidgetRemovedEvent.SetTarget(this);
	SubWidgetRemovedEvent.SetSubWidget(SubWidget);
	g_UserInterface->DispatchSubWidgetRemovedEvent(SubWidgetRemovedEvent);
}

void UI::Widget::RaiseSubWidget(UI::Widget * SubWidget)
{
	assert(SubWidget->_SupWidget == this);
	_SubWidgets.remove(SubWidget);
	_SubWidgets.push_front(SubWidget);
}

void UI::Widget::LowerSubWidget(UI::Widget * SubWidget)
{
	assert(SubWidget->_SupWidget == this);
	_SubWidgets.remove(SubWidget);
	_SubWidgets.push_back(SubWidget);
}

void UI::Widget::Destroy(void)
{
	// fire destroying event before aything is actually destroyed
	UI::Event DestroyingEvent;
	
	DestroyingEvent.SetTarget(this);
	g_UserInterface->DispatchDestroyingEvent(DestroyingEvent);
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

void UI::Widget::_SetHoverWidget(UI::Widget * HoverWidget)
{
	assert(_HoverWidget == nullptr);
	_HoverWidget = HoverWidget;
	
	// inform the new hover widget about the entering of the mouse cursor
	UI::Event MouseEnterEvent;
	
	MouseEnterEvent.SetTarget(_HoverWidget);
	g_UserInterface->DispatchMouseEnterEvent(MouseEnterEvent);
}

void UI::Widget::_UnsetHoverWidget(void)
{
	if(_HoverWidget != nullptr)
	{
		_HoverWidget->_UnsetHoverWidget();
		
		UI::Event MouseLeaveEvent;
		
		MouseLeaveEvent.SetTarget(_HoverWidget);
		g_UserInterface->DispatchMouseLeaveEvent(MouseLeaveEvent);
		_HoverWidget = nullptr;
	}
}

Connection UI::Widget::ConnectDestroyingCallback(std::function< void (UI::Event &) > Callback)
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

Connection UI::Widget::ConnectMouseEnterCallback(std::function< void (UI::Event &) > Callback)
{
	return _MouseEnterEvent.Connect(Callback);
}

Connection UI::Widget::ConnectMouseLeaveCallback(std::function< void (UI::Event &) > Callback)
{
	return _MouseLeaveEvent.Connect(Callback);
}

Connection UI::Widget::ConnectMouseMoveCallback(std::function< void (UI::MouseMoveEvent &) > Callback)
{
	return _MouseMoveEvent.Connect(Callback);
}

Connection UI::Widget::ConnectPositionChangedCallback(std::function< void (UI::Event &) > Callback)
{
	return _PositionChangedEvent.Connect(Callback);
}

Connection UI::Widget::ConnectSizeChangedCallback(std::function< void (UI::Event &) > Callback)
{
	return _SizeChangedEvent.Connect(Callback);
}

Connection UI::Widget::ConnectSubWidgetAddedCallback(std::function< void (UI::SubWidgetEvent &) > Callback)
{
	return _SubWidgetAddedEvent.Connect(Callback);
}

Connection UI::Widget::ConnectSubWidgetRemovedCallback(std::function< void (UI::SubWidgetEvent &) > Callback)
{
	return _SubWidgetRemovedEvent.Connect(Callback);
}

Connection UI::Widget::ConnectUpdatingCallback(std::function< void (float, float) > Callback)
{
	return _UpdatingEvent.Connect(Callback);
}

void UI::Widget::_PopClippingRectangle(Graphics::RenderContext * RenderContext)
{
	RenderContext->UnsetClipping();
	_ClippingRectangles.pop();
	if(_ClippingRectangles.empty() == false)
	{
		auto & TopClippingRectangle{UI::Widget::_ClippingRectangles.top()};
		
		RenderContext->SetClipping(std::get<0>(TopClippingRectangle), std::get<1>(TopClippingRectangle), std::get<2>(TopClippingRectangle), std::get<3>(TopClippingRectangle));
	}
}

void UI::Widget::_PushClippingRectangle(Graphics::RenderContext * RenderContext, float Left, float Top, float Bottom, float Right)
{
	if(_ClippingRectangles.empty() == false)
	{
		auto & TopClippingRectangle{UI::Widget::_ClippingRectangles.top()};
	
		Left = std::max(std::get<0>(TopClippingRectangle), Left);
		Top = std::max(std::get<1>(TopClippingRectangle), Top);
		Bottom = std::min(std::get<2>(TopClippingRectangle), Bottom);
		Right = std::min(std::get<3>(TopClippingRectangle), Right);
		RenderContext->UnsetClipping();
	}
	_ClippingRectangles.push(std::make_tuple(Left, Top, Bottom, Right));
	RenderContext->SetClipping(Left, Top, Bottom, Right);
}
