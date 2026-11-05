/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__WINDOW_H
#define GALACTIC_FALL__SOURCE__UI__WINDOW_H

#include "widget.h"

namespace UI
{
    class Border;
    class Label;
    
    class Window : public UI::Widget
    {
    public:
        Window();
        auto SetTitle(std::string_view Title) -> void;
        auto HideResizeDragBox() -> void;
        auto ShowResizeDragBox() -> void;
        
    private:
        auto m_OnMouseButton(UI::MouseButtonEvent & MouseButtonEvent) -> void;
        auto m_OnTitleLabelMouseButton(UI::MouseButtonEvent & MouseButtonEvent) -> void;
        auto m_OnTitleLabelMouseMove(UI::MouseMoveEvent & MouseMoveEvent) -> void;
        auto m_OnResizeDragBoxMouseButton(UI::MouseButtonEvent & MouseButtonEvent) -> void;
        auto m_OnResizeDragBoxMouseMove(UI::MouseMoveEvent & MouseMoveEvent) -> void;
        auto m_OnTitleLabelUpdating(float RealTimeSeconds, float GameTimeSeconds) -> void;
        
        Vector2f m_GrabPosition;
        UI::Widget * m_ResizeDragBox = nullptr;
        UI::Label * m_TitleLabel = nullptr;
    };
}

#endif
