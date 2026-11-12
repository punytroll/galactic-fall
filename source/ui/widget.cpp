/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>
#include <algorithm>

#include <graphics/color_rgbo.h>
#include <graphics/drawing.h>
#include <graphics/render_context.h>

#include "../globals.h"
#include "../math.h"
#include "event.h"
#include "sub_widget_event.h"
#include "user_interface.h"
#include "widget.h"
#include "window.h"

using namespace Expressions::Operators;

std::list< UI::Widget * > UI::Widget::_DestroyedWidgets;
std::stack< std::tuple< float, float, float, float > > UI::Widget::_ClippingRectangles;

UI::Widget::Widget(void) :
    _BackgroundColor(nullptr),
    _DisabledBackgroundColor(nullptr),
    _Enabled(true),
    _Height(this, UI::Dimension::Type::Height),
    _HoverWidget(nullptr),
    _Left(this, UI::Dimension::Type::Left),
    _SupWidget(nullptr),
    _Top(this, UI::Dimension::Type::Top),
    _Visible(true),
    _Width(this, UI::Dimension::Type::Width)
{
    _Height = 0.0_c;
    _Left = 0.0_c;
    _Top = 0.0_c;
    _Width = 0.0_c;
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
        Graphics::Drawing::DrawBoxFromPositionAndSize(RenderContext, GlobalPosition, Vector2f(_Width.GetValue(), _Height.GetValue()));
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
                _PushClippingRectangle(RenderContext, GlobalPosition[0] + SubWidget->_Left.GetValue(), GlobalPosition[1] + SubWidget->_Top.GetValue(), GlobalPosition[1] + SubWidget->_Top.GetValue() + SubWidget->_Height.GetValue(), GlobalPosition[0] + SubWidget->_Left.GetValue() + SubWidget->_Width.GetValue());
                SubWidget->Draw(RenderContext);
                _PopClippingRectangle(RenderContext);
            }
        }
    }
}

Vector2f UI::Widget::GetGlobalPosition(void)
{
    auto * CurrentWidget(this);
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

void UI::Widget::SetHeight(Expressions::Expression && Height)
{
    _Height = std::move(Height);
}

void UI::Widget::SetLeft(Expressions::Expression && Left)
{
    _Left = std::move(Left);
}

void UI::Widget::SetTop(Expressions::Expression && Top)
{
    _Top = std::move(Top);
}

void UI::Widget::SetWidth(Expressions::Expression && Width)
{
    _Width = std::move(Width);
}

void UI::Widget::GrabKeyboardFocus(void)
{
    if(_SupWidget != nullptr)
    {
        _SupWidget->m_KeyboardFocusWidget = this;
        _SupWidget->GrabKeyboardFocus();
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
    if(SubWidget == m_KeyboardFocusWidget)
    {
        m_KeyboardFocusWidget = nullptr;
        // if a window was removed and lost focus, we focus the next-top-most window
        if(dynamic_cast<UI::Window *>(SubWidget) != nullptr)
        {
            for(auto IterateSubWidget : _SubWidgets | std::views::reverse)
            {
                if(dynamic_cast<UI::Window *>(IterateSubWidget) != nullptr)
                {
                    IterateSubWidget->GrabKeyboardFocus();
                }
            }
        }
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
    // now reset our dimensions, so all dependencies are broken up
    _Height.Reset();
    _Left.Reset();
    _Top.Reset();
    _Width.Reset();
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

auto UI::Widget::ConnectDestroyingCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection
{
    return m_DestroyingSignal.Connect(Callback);
}

auto UI::Widget::ConnectHeightChangedCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection
{
    return m_HeightChangedSignal.Connect(Callback);
}

auto UI::Widget::ConnectKeyCallback(std::function<void(UI::KeyEvent &)> Callback) -> Signals::Connection
{
    return m_KeySignal.Connect(Callback);
}

auto UI::Widget::ConnectLeftChangedCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection
{
    return m_LeftChangedSignal.Connect(Callback);
}

auto UI::Widget::ConnectMouseButtonCallback(std::function<void(UI::MouseButtonEvent &)> Callback) -> Signals::Connection
{
    return m_MouseButtonSignal.Connect(Callback);
}

auto UI::Widget::ConnectMouseEnterCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection
{
    return m_MouseEnterSignal.Connect(Callback);
}

auto UI::Widget::ConnectMouseLeaveCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection
{
    return m_MouseLeaveSignal.Connect(Callback);
}

auto UI::Widget::ConnectMouseMoveCallback(std::function<void(UI::MouseMoveEvent &)> Callback) -> Signals::Connection
{
    return m_MouseMoveSignal.Connect(Callback);
}

auto UI::Widget::ConnectSubWidgetAddedCallback(std::function<void(UI::SubWidgetEvent &)> Callback) -> Signals::Connection
{
    return m_SubWidgetAddedSignal.Connect(Callback);
}

auto UI::Widget::ConnectSubWidgetRemovedCallback(std::function<void(UI::SubWidgetEvent &)> Callback) -> Signals::Connection
{
    return m_SubWidgetRemovedSignal.Connect(Callback);
}

auto UI::Widget::ConnectTopChangedCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection
{
    return m_TopChangedSignal.Connect(Callback);
}

auto UI::Widget::ConnectWidthChangedCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection
{
    return m_WidthChangedSignal.Connect(Callback);
}

auto UI::Widget::ConnectUpdatingCallback(std::function<void(float, float)> Callback) -> Signals::Connection
{
    return m_UpdatingSignal.Connect(Callback);
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
