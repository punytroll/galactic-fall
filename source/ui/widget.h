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

#include <signals/event.h>

#include "dimension.h"
#include "dimension_operators.h"

namespace Graphics
{
    class ColorRGBO;
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
        void SetBackgroundColor(const Graphics::ColorRGBO & BackgroundColor);
        void SetDisabledBackgroundColor(const Graphics::ColorRGBO & DisabledBackgroundColor);
        void UnsetBackgroundColor(void);
        void UnsetDisabledBackgroundColor(void);
        void AddSubWidget(Widget * SubWidget);
        void RemoveSubWidget(Widget * SubWidget);
        void RaiseSubWidget(Widget * SubWidget);
        void LowerSubWidget(Widget * SubWidget);
        void Destroy(void);
        void GrabKeyboardFocus(void);
        void SetName(const std::string & Name);
        // getters
        const Graphics::ColorRGBO * GetBackgroundColor(void) const;
        float GetBottom(void);
        bool GetEnabled(void) const;
        float GetHeight(void);
        Widget * GetHoveredWidget(void);
        Vector2f GetGlobalPosition(void);
        auto GetKeyboardFocusWidget() const -> UI::Widget const *;
        float GetLeft(void);
        const std::string & GetName(void) const;
        std::string GetPath(void) const;
        float GetRight(void);
        Widget * GetRootWidget(void);
        Widget * GetSupWidget(void);
        Widget * GetSubWidget(const std::string & Name);
        const std::list< Widget * > & GetSubWidgets(void) const;
        float GetTop(void);
        float GetWidth(void);
        bool IsEnabled(void) const;
        bool IsHovered(void) const;
        bool IsVisible(void) const;
        // setters
        void SetEnabled(bool Enabled);
        void SetHeight(Expressions::Expression && Height);
        void SetLeft(Expressions::Expression && Left);
        void SetTop(Expressions::Expression && Top);
        void SetVisible(bool Visible);
        void SetWidth(Expressions::Expression && Width);
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
        Graphics::ColorRGBO * _BackgroundColor;
        Graphics::ColorRGBO * _DisabledBackgroundColor;
        bool _Enabled;
        UI::Dimension _Height;
        Widget * _HoverWidget;
        UI::Widget * m_KeyboardFocusWidget = nullptr;
        UI::Dimension _Left;
        std::string _Name;
        std::list< Widget * > _SubWidgets;
        Widget * _SupWidget;
        UI::Dimension _Top;
        bool _Visible;
        UI::Dimension _Width;
        // events
        Signals::Event<void, UI::Event &> m_DestroyingEvent;
        Signals::Event<void, UI::Event &> m_HeightChangedEvent;
        Signals::Event<void, UI::KeyEvent &> m_KeyEvent;
        Signals::Event<void, UI::Event &> m_LeftChangedEvent;
        Signals::Event<void, UI::MouseButtonEvent &> m_MouseButtonEvent;
        Signals::Event<void, UI::Event &> m_MouseEnterEvent;
        Signals::Event<void, UI::Event &> m_MouseLeaveEvent;
        Signals::Event<void, UI::MouseMoveEvent &> m_MouseMoveEvent;
        Signals::Event<void, UI::SubWidgetEvent &> m_SubWidgetAddedEvent;
        Signals::Event<void, UI::SubWidgetEvent &> m_SubWidgetRemovedEvent;
        Signals::Event<void, UI::Event &> m_TopChangedEvent;
        Signals::Event<void, float, float> m_UpdatingEvent;
        Signals::Event<void, UI::Event &> m_WidthChangedEvent;
        // static manager properties
        static std::list< Widget * > _DestroyedWidgets;
        static std::stack< std::tuple< float, float, float, float > > _ClippingRectangles;
    };
    
    inline float Widget::GetBottom(void)
    {
        return _Top.GetValue() + _Height.GetValue();
    }
    
    inline bool Widget::GetEnabled(void) const
    {
        return _Enabled;
    }
    
    inline float Widget::GetHeight(void)
    {
        return _Height.GetValue();
    }
    
    inline Widget * Widget::GetHoveredWidget(void)
    {
        return _HoverWidget;
    }
    
    inline auto Widget::GetKeyboardFocusWidget() const -> UI::Widget const *
    {
        return m_KeyboardFocusWidget;
    }
    
    inline float Widget::GetLeft(void)
    {
        return _Left.GetValue();
    }

    inline const std::string & Widget::GetName(void) const
    {
        return _Name;
    }

    inline std::list< Widget * > & Widget::GetDestroyedWidgets(void)
    {
        return _DestroyedWidgets;
    }
    
    inline float Widget::GetRight(void)
    {
        return _Left.GetValue() + _Width.GetValue();
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
    
    inline float Widget::GetTop(void)
    {
        return _Top.GetValue();
    }
    
    inline float Widget::GetWidth(void)
    {
        return _Width.GetValue();
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
