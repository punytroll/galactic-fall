/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <graphics/color_rgbo.h>

#include "../globals.h"
#include "button.h"
#include "mouse_button_event.h"
#include "user_interface.h"

UI::Button::Button()
{
    SetBackgroundColor(Graphics::ColorRGBO{0.3f, 0.3f, 0.3f, 1.0f});
    SetDisabledBackgroundColor(Graphics::ColorRGBO{0.23f, 0.23f, 0.23f, 1.0f});
    ConnectMouseButtonCallback(std::bind(&UI::Button::m_OnMouseButton, this, std::placeholders::_1));
    ConnectMouseEnterCallback(std::bind(&UI::Button::m_OnMouseEnter, this));
    ConnectMouseLeaveCallback(std::bind(&UI::Button::m_OnMouseLeave, this));
}

auto UI::Button::ConnectClickedCallback(std::function<auto () -> void> ClickedHandler) -> Signals::Connection
{
    return m_ClickedSignal.Connect(ClickedHandler);
}

auto UI::Button::m_OnMouseButton(UI::MouseButtonEvent & MouseButtonEvent) -> void
{
    if((MouseButtonEvent.GetPhase() == UI::MouseButtonEvent::Phase::Capturing) && (MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left))
    {
        if(MouseButtonEvent.IsDown() == true)
        {
            g_UserInterface->SetCaptureWidget(this);
        }
        else if((MouseButtonEvent.IsUp() == true) && (g_UserInterface->GetCaptureWidget() == this))
        {
            g_UserInterface->ReleaseCaptureWidget();
            if((MouseButtonEvent.GetPosition()[0] >= 0.0f) && (MouseButtonEvent.GetPosition()[0] <= GetWidth()) && (MouseButtonEvent.GetPosition()[1] >= 0.0f) && (MouseButtonEvent.GetPosition()[1] <= GetHeight()))
            {
                m_ClickedSignal.Emit();
            }
        }
    }
}

auto UI::Button::m_OnMouseEnter() -> void
{
    SetBackgroundColor(Graphics::ColorRGBO{0.4f, 0.4f, 0.4f, 1.0f});
}

auto UI::Button::m_OnMouseLeave() -> void
{
    SetBackgroundColor(Graphics::ColorRGBO{0.3f, 0.3f, 0.3f, 1.0f});
}
