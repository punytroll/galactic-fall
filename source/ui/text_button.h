/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__TEXT_BUTTON_H
#define GALACTIC_FALL__SOURCE__UI__TEXT_BUTTON_H

#include "button.h"

namespace UI
{
    class Label;
    
    class TextButton : public UI::Button
    {
    public:
        TextButton();
        auto SetText(std::string const & Text) -> void;
        
    private:
        UI::Label * m_Label = nullptr;
    };
}

#endif
