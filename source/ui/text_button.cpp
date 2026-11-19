/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include <expressions/operators.h>

#include "label.h"
#include "text_button.h"

using namespace Expressions::Operators;

UI::TextButton::TextButton()
{
    m_Label = new UI::Label{};
    m_Label->SetLeft(0.0_c);
    m_Label->SetTop(0.0_c);
    m_Label->SetWidth(width(this));
    m_Label->SetHeight(height(this));
    m_Label->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
    m_Label->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
    AddSubWidget(m_Label);
}

auto UI::TextButton::SetText(std::string const & Text) -> void
{
    assert(m_Label != nullptr);
    m_Label->SetText(Text);
}
