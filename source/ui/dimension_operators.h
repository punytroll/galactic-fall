/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__DIMENSION_OPERATORS_H
#define GALACTIC_FALL__SOURCE__UI__DIMENSION_OPERATORS_H

#include <expressions/expression.h>

namespace UI
{
    class Widget;
    
    auto bottom(UI::Widget * Widget) -> Expressions::Expression;
    auto height(UI::Widget * Widget) -> Expressions::Expression;
    auto left(UI::Widget * Widget) -> Expressions::Expression;
    auto right(UI::Widget * Widget) -> Expressions::Expression;
    auto top(UI::Widget * Widget) -> Expressions::Expression;
    auto width(UI::Widget * Widget) -> Expressions::Expression;
}

#endif
