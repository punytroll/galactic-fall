/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__BUTTON_H
#define GALACTIC_FALL__SOURCE__UI__BUTTON_H

#include <signals/signal.h>

#include "widget.h"

namespace UI
{
    class MouseButtonEvent;
    
    class Button : public UI::Widget
    {
    public:
        Button();
        auto ConnectClickedCallback(std::function<auto () -> void> ClickedHandler) -> Signals::Connection;
        
    private:
        auto m_OnMouseButton(UI::MouseButtonEvent & MouseBUttonEvent) -> void;
        auto m_OnMouseEnter() -> void;
        auto m_OnMouseLeave() -> void;
        
        Signals::Signal<auto () -> void> m_ClickedSignal;
    };
}

#endif
