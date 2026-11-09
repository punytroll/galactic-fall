/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__USER_INTERFACE_H
#define GALACTIC_FALL__SOURCE__UI__USER_INTERFACE_H

#include <bitset>
#include <string>

#include <signals/connection.h>

class KeyEventInformation;

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
    class Widget;
    
    class UserInterface
    {
    public:
        UserInterface(void);
        virtual ~UserInterface(void);
        void Draw(Graphics::RenderContext * RenderContext) const;
        void DispatchDestroyingEvent(UI::Event & DestroyingEvent);
        void DispatchKeyEvent(UI::KeyEvent & KeyEvent);
        void DispatchHeightChangedEvent(UI::Event & HeightChangedEvent);
        void DispatchLeftChangedEvent(UI::Event & LeftChangedEvent);
        void DispatchMouseButtonEvent(UI::MouseButtonEvent & MouseButtonEvent);
        void DispatchMouseEnterEvent(UI::Event & MouseEnterEvent);
        void DispatchMouseLeaveEvent(UI::Event & MouseLeaveEvent);
        void DispatchMouseMoveEvent(UI::MouseMoveEvent & MouseMoveEvent);
        void DispatchSubWidgetAddedEvent(UI::SubWidgetEvent & SubWidgetAddedEvent);
        void DispatchSubWidgetRemovedEvent(UI::SubWidgetEvent & SubWidgetRemovedEvent);
        void DispatchTopChangedEvent(UI::Event & TopChangedEvent);
        void DispatchWidthChangedEvent(UI::Event & WidthChangedEvent);
        auto IsKeyCodePressed(unsigned int const KeyCode) const -> bool;
        void Update(float RealTimeSeconds, float GameTimeSeconds);
        // setters
        void SetCaptureWidget(UI::Widget * Widget);
        auto SetRootWidget(UI::Widget * RootWidget) -> void;
        void ReleaseCaptureWidget(void);
        // getters
        UI::Widget * GetWidget(const std::string & Path);
        auto GetRootWidget() -> UI::Widget *;
        UI::Widget * GetCaptureWidget(void);
    private:
        // callbacks
        void _OnCaptureWidgetDestroying(UI::Event & DestroyingEvent);
        void _OnHoverWidgetDestroying(UI::Event & DestroyingEvent);
        void _OnRootWidgetDestroying(UI::Event & DestroyingEvent);
        // member variables
        UI::Widget * _CaptureWidget;
        Connection _CaptureWidgetDestroyingCallbackConnection;
        UI::Widget * _HoverWidget;
        std::bitset<256> m_PressedKeyCodes;
        UI::Widget * m_RootWidget = nullptr;
    };

    inline UI::Widget * UserInterface::GetCaptureWidget(void)
    {
        return _CaptureWidget;
    }

    inline auto UserInterface::GetRootWidget() -> UI::Widget *
    {
        return m_RootWidget;
    }
}

#endif
