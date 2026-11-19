/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__WIDGET_H
#define GALACTIC_FALL__SOURCE__UI__WIDGET_H

#include <iostream>
#include <list>
#include <stack>
#include <string>

#include <algebra/vector2f.h>

#include <graphics/color_rgbo.h>

#include <signals/signal.h>

#include "dimension.h"
#include "dimension_operators.h"

namespace Graphics
{
    class RenderContext;
}

namespace UI
{
    class Event;
    class KeyEvent;
    class MouseButtonEvent;
    class MouseMoveEvent;
    class SubWidgetEvent;
    class UserInterface;
    
    class Widget
    {
    public:
        friend class UI::UserInterface;
        friend Expressions::Expression UI::bottom(UI::Widget * Widget);
        friend Expressions::Expression UI::height(UI::Widget * Widget);
        friend Expressions::Expression UI::left(UI::Widget * Widget);
        friend Expressions::Expression UI::right(UI::Widget * Widget);
        friend Expressions::Expression UI::top(UI::Widget * Widget);
        friend Expressions::Expression UI::width(UI::Widget * Widget);
        
        Widget(void);
        virtual ~Widget(void);
        virtual void Draw(Graphics::RenderContext * RenderContext);
        auto SetBackgroundColor(std::optional<Graphics::ColorRGBO> const & BackgroundColor) -> void;
        auto SetDisabledBackgroundColor(std::optional<Graphics::ColorRGBO> const & DisabledBackgroundColor) -> void;
        void AddSubWidget(Widget * SubWidget);
        void RemoveSubWidget(Widget * SubWidget);
        void RaiseSubWidget(Widget * SubWidget);
        void LowerSubWidget(Widget * SubWidget);
        void Destroy(void);
        void GrabKeyboardFocus(void);
        void SetName(const std::string & Name);
        // getters
        auto GetBottom() -> float;
        bool GetEnabled(void) const;
        auto GetHeight() -> float;
        Widget * GetHoveredWidget(void);
        Vector2f GetGlobalPosition(void);
        auto GetKeyboardFocusWidget() const -> UI::Widget const *;
        auto GetLeft() -> float;
        const std::string & GetName(void) const;
        std::string GetPath(void) const;
        auto GetRight() -> float;
        Widget * GetRootWidget(void);
        Widget * GetSupWidget(void);
        Widget * GetSubWidget(const std::string & Name);
        const std::list< Widget * > & GetSubWidgets(void) const;
        auto GetTop() -> float;
        auto GetWidth() -> float;
        bool IsEnabled(void) const;
        bool IsHovered(void) const;
        bool IsVisible(void) const;
        // setters
        void SetEnabled(bool Enabled);
        auto SetHeight(Expressions::Expression && Height) -> void;
        auto SetLeft(Expressions::Expression && Left) -> void;
        auto SetTop(Expressions::Expression && Top) -> void;
        void SetVisible(bool Visible);
        auto SetWidth(Expressions::Expression && Width) -> void;
        // connect events
        auto ConnectDestroyingCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection;
        auto ConnectHeightChangedCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection;
        auto ConnectKeyCallback(std::function<void(UI::KeyEvent &)> Callback) -> Signals::Connection;
        auto ConnectLeftChangedCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection;
        auto ConnectMouseButtonCallback(std::function<void(UI::MouseButtonEvent &)> Callback) -> Signals::Connection;
        auto ConnectMouseEnterCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection;
        auto ConnectMouseLeaveCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection;
        auto ConnectMouseMoveCallback(std::function<void(UI::MouseMoveEvent &)> Callback) -> Signals::Connection;
        auto ConnectSubWidgetAddedCallback(std::function<void(UI::SubWidgetEvent &)> Callback) -> Signals::Connection;
        auto ConnectSubWidgetRemovedCallback(std::function<void(UI::SubWidgetEvent &)> Callback) -> Signals::Connection;
        auto ConnectTopChangedCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection;
        auto ConnectUpdatingCallback(std::function<void(float, float)> Callback) -> Signals::Connection;
        auto ConnectWidthChangedCallback(std::function<void(UI::Event &)> Callback) -> Signals::Connection;
        // static manager functions
        static std::list< Widget * > & GetDestroyedWidgets(void);
    protected:
        static void _PopClippingRectangle(Graphics::RenderContext * RenderContext);
        static void _PushClippingRectangle(Graphics::RenderContext * RenderContext, float Left, float Top, float Bottom, float Right);
    private:
        void _UnsetHoverWidget(void);
        void _SetHoverWidget(UI::Widget * HoverWidget);
        std::optional<Graphics::ColorRGBO> m_BackgroundColor;
        std::optional<Graphics::ColorRGBO> m_DisabledBackgroundColor;
        bool _Enabled;
        UI::Dimension m_Height;
        Widget * _HoverWidget;
        UI::Widget * m_KeyboardFocusWidget = nullptr;
        UI::Dimension m_Left;
        std::string _Name;
        std::list< Widget * > _SubWidgets;
        Widget * _SupWidget;
        UI::Dimension m_Top;
        bool _Visible;
        UI::Dimension m_Width;
        // events
        Signals::Signal<auto (UI::Event &) -> void> m_DestroyingSignal;
        Signals::Signal<auto (UI::Event &) -> void> m_HeightChangedSignal;
        Signals::Signal<auto (UI::KeyEvent &) -> void> m_KeySignal;
        Signals::Signal<auto (UI::Event &) -> void> m_LeftChangedSignal;
        Signals::Signal<auto (UI::MouseButtonEvent &) -> void> m_MouseButtonSignal;
        Signals::Signal<auto (UI::Event &) -> void> m_MouseEnterSignal;
        Signals::Signal<auto (UI::Event &) -> void> m_MouseLeaveSignal;
        Signals::Signal<auto (UI::MouseMoveEvent &) -> void> m_MouseMoveSignal;
        Signals::Signal<auto (UI::SubWidgetEvent &) -> void> m_SubWidgetAddedSignal;
        Signals::Signal<auto (UI::SubWidgetEvent &) -> void> m_SubWidgetRemovedSignal;
        Signals::Signal<auto (UI::Event &) -> void> m_TopChangedSignal;
        Signals::Signal<auto (float, float) -> void> m_UpdatingSignal;
        Signals::Signal<auto (UI::Event &) -> void> m_WidthChangedSignal;
        // static manager properties
        static std::list< Widget * > _DestroyedWidgets;
        static std::stack< std::tuple< float, float, float, float > > _ClippingRectangles;
    };
    
    inline auto Widget::GetBottom() -> float
    {
        return m_Top.GetValue() + m_Height.GetValue();
    }
    
    inline bool Widget::GetEnabled(void) const
    {
        return _Enabled;
    }
    
    inline auto Widget::GetHeight() -> float
    {
        return m_Height.GetValue();
    }
    
    inline Widget * Widget::GetHoveredWidget(void)
    {
        return _HoverWidget;
    }
    
    inline auto Widget::GetKeyboardFocusWidget() const -> UI::Widget const *
    {
        return m_KeyboardFocusWidget;
    }
    
    inline auto Widget::GetLeft() -> float
    {
        return m_Left.GetValue();
    }

    inline const std::string & Widget::GetName(void) const
    {
        return _Name;
    }

    inline std::list< Widget * > & Widget::GetDestroyedWidgets(void)
    {
        return _DestroyedWidgets;
    }
    
    inline auto Widget::GetRight() -> float
    {
        return m_Left.GetValue() + m_Width.GetValue();
    }

    inline Widget * Widget::GetRootWidget(void)
    {
        auto SupWidget(this);
        
        while(SupWidget->_SupWidget != nullptr)
        {
            SupWidget = SupWidget->_SupWidget;
        }
        
        return SupWidget;
    }

    inline Widget * Widget::GetSupWidget(void)
    {
        return _SupWidget;
    }

    inline Widget * Widget::GetSubWidget(const std::string & Name)
    {
        for(auto Widget : _SubWidgets)
        {
            if(Widget->GetName() == Name)
            {
                return Widget;
            }
        }
        
        return nullptr;
    }

    inline const std::list< Widget * > & Widget::GetSubWidgets(void) const
    {
        return _SubWidgets;
    }
    
    inline auto Widget::GetTop() -> float
    {
        return m_Top.GetValue();
    }
    
    inline auto Widget::GetWidth() -> float
    {
        return m_Width.GetValue();
    }
    
    inline bool Widget::IsEnabled(void) const
    {
        return _Enabled;
    }
    
    inline bool Widget::IsHovered(void) const
    {
        if(_SupWidget != nullptr)
        {
            return this == _SupWidget->_HoverWidget;
        }
        else
        {
            return false;
        }
    }

    inline bool Widget::IsVisible(void) const
    {
        return _Visible;
    }

    inline void Widget::SetEnabled(bool Enabled)
    {
        _Enabled = Enabled;
    }
    
    inline void Widget::SetName(const std::string & Name)
    {
        _Name = Name;
    }
    
    inline void Widget::SetVisible(bool Visible)
    {
        _Visible = Visible;
    }
}

#endif
