/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>
#include <vector>

#include <graphics/gl.h>
#include <graphics/render_context.h>

#include "key_event.h"
#include "mouse_button_event.h"
#include "mouse_move_event.h"
#include "sub_widget_event.h"
#include "user_interface.h"
#include "widget.h"

namespace UI
{
    namespace
    {
        struct EventPropagationPathItem
        {
            EventPropagationPathItem(UI::Widget * Widget, UI::Event::Phase Phase) :
                m_Phase{Phase},
                m_Widget{Widget}
            {
                assert(m_Widget != nullptr);
                m_WidgetDestroyingConnection = m_Widget->ConnectDestroyingCallback(std::bind(&UI::EventPropagationPathItem::m_OnWidgetDestroying, this, std::placeholders::_1));
            }
            
            EventPropagationPathItem(UI::EventPropagationPathItem const & EventPropagationPathItem) = delete;
            
            ~EventPropagationPathItem()
            {
                if(m_Widget != nullptr)
                {
                    assert(m_WidgetDestroyingConnection.IsValid() == true);
                    m_WidgetDestroyingConnection.Disconnect();
                }
                assert(m_WidgetDestroyingConnection.IsValid() == false);
            }
            
            UI::Event::Phase m_Phase;
            UI::Widget * m_Widget;
            
        private:
            auto m_OnWidgetDestroying(UI::Event & DestroyingEvent) -> void
            {
                if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
                {
                    assert(m_Widget != nullptr);
                    m_Widget = nullptr;
                    assert(m_WidgetDestroyingConnection.IsValid() == true);
                    m_WidgetDestroyingConnection.Disconnect();
                    assert(m_WidgetDestroyingConnection.IsValid() == false);
                }
            }
            
            Signals::Connection m_WidgetDestroyingConnection;
        };

        struct KeyEventPropagationPathItem : UI::EventPropagationPathItem
        {
            using UI::EventPropagationPathItem::EventPropagationPathItem;
        };

        struct MousePositionEventPropagationPathItem : UI::EventPropagationPathItem
        {
            MousePositionEventPropagationPathItem(UI::Widget * Widget, UI::Event::Phase Phase, Vector2f Position) :
                UI::EventPropagationPathItem{Widget, Phase},
                m_Position{Position}
            {
            }
            
            Vector2f m_Position;
        };
        
        auto ExtendPropagationPathAroundTarget(std::list<UI::EventPropagationPathItem> & PropagationPath) -> void
        {
            assert(PropagationPath.size() == 1);
            
            auto PathWidget = PropagationPath.front().m_Widget;
            
            assert(PathWidget != nullptr);
            while(PathWidget != nullptr)
            {
                PropagationPath.emplace_front(PathWidget, UI::Event::Phase::Capturing);
                PropagationPath.emplace_back(PathWidget, UI::Event::Phase::Bubbling);
                PathWidget = PathWidget->GetSupWidget();
            }
        }
    }
}

UI::UserInterface::UserInterface(void) :
    _CaptureWidget(nullptr),
    _HoverWidget(nullptr)
{
}

UI::UserInterface::~UserInterface(void)
{
    if(_CaptureWidget != nullptr)
    {
        _CaptureWidget = nullptr;
        assert(m_CaptureWidgetDestroyingCallbackConnection.IsValid() == true);
        m_CaptureWidgetDestroyingCallbackConnection.Disconnect();
        assert(m_CaptureWidgetDestroyingCallbackConnection.IsValid() == false);
    }
    m_RootWidget->Destroy();
    m_RootWidget = nullptr;
}

void UI::UserInterface::Draw(Graphics::RenderContext * RenderContext) const
{
    GLEnable(GL_CLIP_PLANE0);
    GLEnable(GL_CLIP_PLANE1);
    GLEnable(GL_CLIP_PLANE2);
    GLEnable(GL_CLIP_PLANE3);
    if((m_RootWidget != nullptr) && (m_RootWidget->IsVisible() == true))
    {
        UI::Widget::_PushClippingRectangle(RenderContext, m_RootWidget->GetLeft(), m_RootWidget->GetTop(), m_RootWidget->GetTop() + m_RootWidget->GetHeight(), m_RootWidget->GetLeft() + m_RootWidget->GetWidth());
        m_RootWidget->Draw(RenderContext);
        UI::Widget::_PopClippingRectangle(RenderContext);
    }
    GLDisable(GL_CLIP_PLANE0);
    GLDisable(GL_CLIP_PLANE1);
    GLDisable(GL_CLIP_PLANE2);
    GLDisable(GL_CLIP_PLANE3);
}

void UI::UserInterface::SetCaptureWidget(UI::Widget * Widget)
{
    assert(Widget != nullptr);
    assert(_CaptureWidget == nullptr);
    _CaptureWidget = Widget;
    m_CaptureWidgetDestroyingCallbackConnection = _CaptureWidget->ConnectDestroyingCallback(std::bind(&UI::UserInterface::_OnCaptureWidgetDestroying, this, std::placeholders::_1));
}

auto UI::UserInterface::SetRootWidget(UI::Widget * RootWidget) -> void
{
    assert(RootWidget != nullptr);
    assert(m_RootWidget == nullptr);
    m_RootWidget = RootWidget;
    m_RootWidget->ConnectDestroyingCallback(std::bind(&UI::UserInterface::_OnRootWidgetDestroying, this, std::placeholders::_1));
}

void UI::UserInterface::ReleaseCaptureWidget(void)
{
    if(_CaptureWidget != nullptr)
    {
        _CaptureWidget = nullptr;
        assert(m_CaptureWidgetDestroyingCallbackConnection.IsValid() == true);
        m_CaptureWidgetDestroyingCallbackConnection.Disconnect();
        assert(m_CaptureWidgetDestroyingCallbackConnection.IsValid() == false);
    }
}

UI::Widget * UI::UserInterface::GetWidget(const std::string & Path)
{
    if(Path[0] != '/')
    {
        return nullptr;
    }
    
    std::string::size_type Position(1);
    auto * Root(m_RootWidget);
    
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
    
    auto PropagationPath = std::list<UI::EventPropagationPathItem>{};
    
    PropagationPath.emplace_back(DestroyingEvent.GetTarget(), UI::Event::Phase::Target);
    UI::ExtendPropagationPathAroundTarget(PropagationPath);
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            DestroyingEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            DestroyingEvent.SetPhase(PropagationPathItem.m_Phase);
            DestroyingEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_DestroyingSignal.GetCallbacks())
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
    }
}

void UI::UserInterface::DispatchHeightChangedEvent(UI::Event & HeightChangedEvent)
{
    assert(HeightChangedEvent.GetTarget() != nullptr);
    
    auto PropagationPath = std::list<UI::EventPropagationPathItem>{};
    
    PropagationPath.emplace_back(HeightChangedEvent.GetTarget(), UI::Event::Phase::Target);
    UI::ExtendPropagationPathAroundTarget(PropagationPath);
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            HeightChangedEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            HeightChangedEvent.SetPhase(PropagationPathItem.m_Phase);
            HeightChangedEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_HeightChangedSignal.GetCallbacks())
            {
                Callback(HeightChangedEvent);
                if(HeightChangedEvent.GetStopCallbacks() == true)
                {
                    break;
                }
            }
            if(HeightChangedEvent.GetStopPropagation() == true)
            {
                break;
            }
        }
    }
}

void UI::UserInterface::DispatchKeyEvent(UI::KeyEvent & KeyEvent)
{
    assert(KeyEvent.GetTarget() == nullptr);
    assert(KeyEvent.GetKeyCode() < 256);
    // we are only interested in this event, if the state changes
    if(m_PressedKeyCodes[KeyEvent.GetKeyCode()] != KeyEvent.IsDown())
    {
        m_PressedKeyCodes[KeyEvent.GetKeyCode()] = KeyEvent.IsDown();
        
        auto PropagationPath = std::list<UI::KeyEventPropagationPathItem>{};
        auto PathWidget = m_RootWidget;
        
        while(PathWidget != nullptr)
        {
            assert(PathWidget->_Enabled == true);
            PropagationPath.emplace_back(PathWidget, UI::Event::Phase::Capturing);
            PathWidget = PathWidget->m_KeyboardFocusWidget;
        }
        if(PropagationPath.size() > 0)
        {
            PropagationPath.emplace_back(PropagationPath.back().m_Widget, UI::Event::Phase::Target);
            PathWidget = PropagationPath.back().m_Widget;
            while(PathWidget != nullptr)
            {
                assert(PathWidget->_Enabled == true);
                PropagationPath.emplace_back(PathWidget, UI::Event::Phase::Bubbling);
                PathWidget = PathWidget->_SupWidget;
            }
        }
        for(auto const & PropagationPathItem : PropagationPath)
        {
            if(PropagationPathItem.m_Widget != nullptr)
            {
                KeyEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
                KeyEvent.SetPhase(PropagationPathItem.m_Phase);
                KeyEvent.ResumeCallbacks();
                for(auto & Callback : PropagationPathItem.m_Widget->m_KeySignal.GetCallbacks())
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
        }
    }
}

void UI::UserInterface::DispatchLeftChangedEvent(UI::Event & LeftChangedEvent)
{
    assert(LeftChangedEvent.GetTarget() != nullptr);
    
    auto PropagationPath = std::list<UI::EventPropagationPathItem>{};
    
    PropagationPath.emplace_back(LeftChangedEvent.GetTarget(), UI::Event::Phase::Target);
    UI::ExtendPropagationPathAroundTarget(PropagationPath);
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            LeftChangedEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            LeftChangedEvent.SetPhase(PropagationPathItem.m_Phase);
            LeftChangedEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_LeftChangedSignal.GetCallbacks())
            {
                Callback(LeftChangedEvent);
                if(LeftChangedEvent.GetStopCallbacks() == true)
                {
                    break;
                }
            }
            if(LeftChangedEvent.GetStopPropagation() == true)
            {
                break;
            }
        }
    }
}

bool IsInside(const Vector2f & Position, float WidgetLeft, float WidgetTop, float WidgetWidth, float WidgetHeight)
{
    return (Position[0] >= WidgetLeft) && (Position[0] < WidgetLeft + WidgetWidth) && (Position[1] >= WidgetTop) && (Position[1] < WidgetTop + WidgetHeight);
}

bool IsInside(const Vector2f & Position, float WidgetWidth, float WidgetHeight)
{
    return (Position[0] >= 0.0f) && (Position[0] < WidgetWidth) && (Position[1] >= 0.0f) && (Position[1] < WidgetHeight);
}

void UI::UserInterface::DispatchMouseButtonEvent(UI::MouseButtonEvent & MouseButtonEvent)
{
    assert(MouseButtonEvent.GetTarget() == nullptr);
    
    auto PropagationPath = std::list<UI::MousePositionEventPropagationPathItem>{};
    
    if(_CaptureWidget == nullptr)
    {
        if((m_RootWidget != nullptr) && (m_RootWidget->IsVisible() == true) && (m_RootWidget->IsEnabled() == true))
        {
            auto Position = MouseButtonEvent.GetPosition() - Vector2f{m_RootWidget->GetLeft(), m_RootWidget->GetTop()};
            
            if(IsInside(Position, m_RootWidget->GetWidth(), m_RootWidget->GetHeight()) == true)
            {
                auto PathWidget = m_RootWidget;
                
                while(PathWidget != nullptr)
                {
                    PropagationPath.emplace_back(PathWidget, UI::Event::Phase::Capturing, Position);
                    
                    auto NextWidget = static_cast<UI::Widget *>(nullptr);
                    
                    for(auto SubWidget : PathWidget->_SubWidgets)
                    {
                        if((SubWidget->IsVisible() == true) && (SubWidget->IsEnabled() == true) && (IsInside(Position, SubWidget->GetLeft(), SubWidget->GetTop(), SubWidget->GetWidth(), SubWidget->GetHeight()) == true))
                        {
                            Position -= Vector2f{SubWidget->GetLeft(), SubWidget->GetTop()};
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
        auto GlobalPosition = Vector2f{_CaptureWidget->GetGlobalPosition()};
        auto PathWidget = _CaptureWidget;
        
        while(PathWidget != nullptr)
        {
            PropagationPath.emplace_front(PathWidget, UI::Event::Phase::Capturing, MouseButtonEvent.GetPosition() - GlobalPosition);
            GlobalPosition[0] -= PathWidget->GetLeft();
            GlobalPosition[1] -= PathWidget->GetTop();
            PathWidget = PathWidget->_SupWidget;
        }
    }
    if(PropagationPath.size() > 0)
    {
        PropagationPath.emplace_back(PropagationPath.back().m_Widget, UI::Event::Phase::Target, PropagationPath.back().m_Position);
        for(auto Iterator = std::next(PropagationPath.rbegin()); Iterator != PropagationPath.rend(); ++Iterator)
        {
            PropagationPath.emplace_back(Iterator->m_Widget, UI::Event::Phase::Bubbling, Iterator->m_Position);
        }
    }
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            MouseButtonEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            MouseButtonEvent.SetPhase(PropagationPathItem.m_Phase);
            MouseButtonEvent.SetPosition(PropagationPathItem.m_Position);
            MouseButtonEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_MouseButtonSignal.GetCallbacks())
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
    }
}

void UI::UserInterface::DispatchMouseEnterEvent(UI::Event & MouseEnterEvent)
{
    assert(MouseEnterEvent.GetTarget() != nullptr);
    
    auto PropagationPath = std::list<UI::EventPropagationPathItem>{};
    
    PropagationPath.emplace_back(MouseEnterEvent.GetTarget(), UI::Event::Phase::Target);
    UI::ExtendPropagationPathAroundTarget(PropagationPath);
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            MouseEnterEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            MouseEnterEvent.SetPhase(PropagationPathItem.m_Phase);
            MouseEnterEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_MouseEnterSignal.GetCallbacks())
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
    }
}

void UI::UserInterface::DispatchMouseLeaveEvent(UI::Event & MouseLeaveEvent)
{
    assert(MouseLeaveEvent.GetTarget() != nullptr);
    
    auto PropagationPath = std::list<UI::EventPropagationPathItem>{};
    
    PropagationPath.emplace_back(MouseLeaveEvent.GetTarget(), UI::Event::Phase::Target);
    UI::ExtendPropagationPathAroundTarget(PropagationPath);
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            MouseLeaveEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            MouseLeaveEvent.SetPhase(PropagationPathItem.m_Phase);
            MouseLeaveEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_MouseLeaveSignal.GetCallbacks())
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
    }
}

void UI::UserInterface::DispatchMouseMoveEvent(UI::MouseMoveEvent & MouseMoveEvent)
{
    assert(MouseMoveEvent.GetTarget() == nullptr);
    
    auto PropagationPath = std::list<UI::MousePositionEventPropagationPathItem>{};
    auto InsertIterator = PropagationPath.end();
    
    if(_CaptureWidget == nullptr)
    {
        if((m_RootWidget != nullptr) && (m_RootWidget->IsEnabled() == true) && (m_RootWidget->IsVisible() == true))
        {
            auto Position = MouseMoveEvent.GetPosition();
            
            assert((_HoverWidget == nullptr) || (_HoverWidget == m_RootWidget));
            if(IsInside(Position, m_RootWidget->GetLeft(), m_RootWidget->GetTop(), m_RootWidget->GetWidth(), m_RootWidget->GetHeight()) == true)
            {
                if(_HoverWidget == nullptr)
                {
                    _HoverWidget = m_RootWidget;
                    
                    auto MouseEnterEvent = UI::Event{};
                    
                    MouseEnterEvent.SetTarget(m_RootWidget);
                    DispatchMouseEnterEvent(MouseEnterEvent);
                }
                
                auto CurrentWidget = m_RootWidget;
                
                while(CurrentWidget != nullptr)
                {
                    auto NextWidget = static_cast<UI::Widget *>(nullptr);
                    
                    Position[0] -= CurrentWidget->GetLeft();
                    Position[1] -= CurrentWidget->GetTop();
                    // insert bubbling phase path item
                    InsertIterator = PropagationPath.emplace(InsertIterator, CurrentWidget, UI::Event::Phase::Bubbling, Position);
                    // calculate next widget
                    for(auto SubWidget : CurrentWidget->_SubWidgets)
                    {
                        assert(SubWidget != nullptr);
                        if((SubWidget->IsEnabled() == true) && (SubWidget->IsVisible() == true) && (IsInside(Position, SubWidget->GetLeft(), SubWidget->GetTop(), SubWidget->GetWidth(), SubWidget->GetHeight()) == true))
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
                        InsertIterator = PropagationPath.emplace(InsertIterator, CurrentWidget, UI::Event::Phase::Target, Position);
                    }
                    InsertIterator = PropagationPath.emplace(InsertIterator, CurrentWidget, UI::Event::Phase::Capturing, Position);
                    CurrentWidget = NextWidget;
                }
            }
            else if(_HoverWidget == m_RootWidget)
            {
                m_RootWidget->_UnsetHoverWidget();
                _HoverWidget = nullptr;
            }
        }
    }
    else
    {
        if((_CaptureWidget->IsEnabled() == true) && (_CaptureWidget->IsVisible() == true))
        {
            auto Position = MouseMoveEvent.GetPosition() - _CaptureWidget->GetGlobalPosition();
            auto CurrentWidget = _CaptureWidget;
            
            while(CurrentWidget != nullptr)
            {
                if(CurrentWidget == _CaptureWidget)
                {
                    PropagationPath.emplace_back(CurrentWidget, UI::Event::Phase::Target, Position);
                }
                if(CurrentWidget->_SupWidget != nullptr)
                {
                    if((IsInside(Position, CurrentWidget->GetWidth(), CurrentWidget->GetHeight()) == true) && (CurrentWidget->_SupWidget->_HoverWidget != CurrentWidget))
                    {
                        CurrentWidget->_SupWidget->_SetHoverWidget(CurrentWidget);
                    }
                    if((IsInside(Position, CurrentWidget->GetWidth(), CurrentWidget->GetHeight()) == false) && (CurrentWidget->_SupWidget->_HoverWidget == CurrentWidget))
                    {
                        CurrentWidget->_SupWidget->_UnsetHoverWidget();
                    }
                }
                // insert capturing phase item
                PropagationPath.emplace_front(CurrentWidget, UI::Event::Phase::Capturing, Position);
                // insert bubbling phase item
                PropagationPath.emplace_back(CurrentWidget, UI::Event::Phase::Bubbling, Position);
                Position[0] += CurrentWidget->GetLeft();
                Position[1] += CurrentWidget->GetTop();
                CurrentWidget = CurrentWidget->_SupWidget;
            }
        }
    }
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            MouseMoveEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            MouseMoveEvent.SetPhase(PropagationPathItem.m_Phase);
            MouseMoveEvent.SetPosition(PropagationPathItem.m_Position);
            MouseMoveEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_MouseMoveSignal.GetCallbacks())
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
    }
}

void UI::UserInterface::DispatchSubWidgetAddedEvent(UI::SubWidgetEvent & SubWidgetAddedEvent)
{
    assert(SubWidgetAddedEvent.GetTarget() != nullptr);
    
    auto PropagationPath = std::list<UI::EventPropagationPathItem>{};
    
    PropagationPath.emplace_back(SubWidgetAddedEvent.GetTarget(), UI::Event::Phase::Target);
    UI::ExtendPropagationPathAroundTarget(PropagationPath);
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            SubWidgetAddedEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            SubWidgetAddedEvent.SetPhase(PropagationPathItem.m_Phase);
            SubWidgetAddedEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_SubWidgetAddedSignal.GetCallbacks())
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
    }
}

void UI::UserInterface::DispatchSubWidgetRemovedEvent(UI::SubWidgetEvent & SubWidgetRemovedEvent)
{
    assert(SubWidgetRemovedEvent.GetTarget() != nullptr);
    
    auto PropagationPath = std::list<UI::EventPropagationPathItem>{};
    
    PropagationPath.emplace_back(SubWidgetRemovedEvent.GetTarget(), UI::Event::Phase::Target);
    UI::ExtendPropagationPathAroundTarget(PropagationPath);
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            SubWidgetRemovedEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            SubWidgetRemovedEvent.SetPhase(PropagationPathItem.m_Phase);
            SubWidgetRemovedEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_SubWidgetRemovedSignal.GetCallbacks())
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
    }
}

void UI::UserInterface::DispatchTopChangedEvent(UI::Event & TopChangedEvent)
{
    assert(TopChangedEvent.GetTarget() != nullptr);
    
    auto PropagationPath = std::list<UI::EventPropagationPathItem>{};
    
    PropagationPath.emplace_back(TopChangedEvent.GetTarget(), UI::Event::Phase::Target);
    UI::ExtendPropagationPathAroundTarget(PropagationPath);
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            TopChangedEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            TopChangedEvent.SetPhase(PropagationPathItem.m_Phase);
            TopChangedEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_TopChangedSignal.GetCallbacks())
            {
                Callback(TopChangedEvent);
                if(TopChangedEvent.GetStopCallbacks() == true)
                {
                    break;
                }
            }
            if(TopChangedEvent.GetStopPropagation() == true)
            {
                break;
            }
        }
    }
}

void UI::UserInterface::DispatchWidthChangedEvent(UI::Event & WidthChangedEvent)
{
    assert(WidthChangedEvent.GetTarget() != nullptr);
    
    auto PropagationPath = std::list<UI::EventPropagationPathItem>{};
    
    PropagationPath.emplace_back(WidthChangedEvent.GetTarget(), UI::Event::Phase::Target);
    UI::ExtendPropagationPathAroundTarget(PropagationPath);
    for(auto const & PropagationPathItem : PropagationPath)
    {
        if(PropagationPathItem.m_Widget != nullptr)
        {
            WidthChangedEvent.SetCurrentTarget(PropagationPathItem.m_Widget);
            WidthChangedEvent.SetPhase(PropagationPathItem.m_Phase);
            WidthChangedEvent.ResumeCallbacks();
            for(auto & Callback : PropagationPathItem.m_Widget->m_WidthChangedSignal.GetCallbacks())
            {
                Callback(WidthChangedEvent);
                if(WidthChangedEvent.GetStopCallbacks() == true)
                {
                    break;
                }
            }
            if(WidthChangedEvent.GetStopPropagation() == true)
            {
                break;
            }
        }
    }
}

auto UI::UserInterface::IsKeyCodePressed(unsigned int const KeyCode) const -> bool
{
    assert(KeyCode < 256);
    
    return m_PressedKeyCodes[KeyCode];
}

void UI::UserInterface::Update(float RealTimeSeconds, float GameTimeSeconds)
{
    assert(m_RootWidget != nullptr);
    
    std::stack< std::pair< UI::Widget *, bool > > ToDo;
    
    ToDo.push(std::make_pair(m_RootWidget, false));
    while(ToDo.size() > 0)
    {
        std::pair< UI::Widget *, bool > & Widget(ToDo.top());
        
        if((Widget.first->_SubWidgets.size() == 0) || (Widget.second == true))
        {
            Widget.first->m_UpdatingSignal.Emit(RealTimeSeconds, GameTimeSeconds);
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
        m_CaptureWidgetDestroyingCallbackConnection.Disconnect();
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
        assert(m_RootWidget != nullptr);
        m_RootWidget = nullptr;
    }
}
