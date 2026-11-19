/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include <expressions/operators.h>

#include "dimension_operators.h"
#include "widget.h"

using namespace Expressions::Operators;

auto UI::bottom(UI::Widget * Widget) -> Expressions::Expression
{
    assert(Widget != nullptr);
    
    return Widget->m_Top + Widget->m_Height;
}

auto UI::height(UI::Widget * Widget) -> Expressions::Expression
{
    assert(Widget != nullptr);
    
    return Expressions::Expression{Widget->m_Height};
}

auto UI::left(UI::Widget * Widget) -> Expressions::Expression
{
    assert(Widget != nullptr);
    
    return Expressions::Expression{Widget->m_Left};
}

auto UI::right(UI::Widget * Widget) -> Expressions::Expression
{
    assert(Widget != nullptr);
    
    return Widget->m_Left + Widget->m_Width;
}

auto UI::top(UI::Widget * Widget) -> Expressions::Expression
{
    assert(Widget != nullptr);
    
    return Expressions::Expression{Widget->m_Top};
}

auto UI::width(UI::Widget * Widget) -> Expressions::Expression
{
    assert(Widget != nullptr);
    
    return Expressions::Expression{Widget->m_Width};
}
