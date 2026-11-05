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
        // constructor
        Window(void);
        // setters
        void SetTitle(const std::string & Title);
        // modifiers
        void HideResizeDragBox(void);
        void ShowResizeDragBox(void);
    private:
        // callbacks
        void _OnTitleLabelMouseButton(UI::MouseButtonEvent & MouseButtonEvent);
        void _OnTitleLabelMouseMove(UI::MouseMoveEvent & MouseMoveEvent);
        void _OnResizeDragBoxMouseButton(UI::MouseButtonEvent & MouseButtonEvent);
        void _OnResizeDragBoxMouseMove(UI::MouseMoveEvent & MouseMoveEvent);
        // member variables
        Vector2f _GrabPosition;
        UI::Widget * _ResizeDragBox;
        UI::Label * _TitleLabel;
    };
}

#endif
