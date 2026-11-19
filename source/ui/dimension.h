/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__DIMENSION_H
#define GALACTIC_FALL__SOURCE__UI__DIMENSION_H

#include <expressions/expression.h>
#include <expressions/variable.h>

namespace UI
{
    class Widget;
    
    class Dimension : public Expressions::Variable
    {
    public:
        enum class Type
        {
            Height,
            Left,
            Top,
            Width
        };
        
        Dimension(UI::Widget & Owner, UI::Dimension::Type Type);
        auto operator=(Expressions::Expression && Expression) -> void;
        
    protected:
        auto InvalidateValue() -> void override;
        
    private:
        UI::Widget & m_Owner;
        UI::Dimension::Type m_Type;
    };
}

#endif
