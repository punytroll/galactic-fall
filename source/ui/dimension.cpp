/**
 * galactic-fall
 * Copyright (C) 2018-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>
#include <iostream>

#include "../globals.h"
#include "dimension.h"
#include "event.h"
#include "user_interface.h"

UI::Dimension::Dimension(UI::Widget & Owner, UI::Dimension::Type Type) :
    m_Owner{Owner},
    m_Type{Type}
{
}

auto UI::Dimension::operator=(Expressions::Expression && Expression) -> void
{
    SetExpression(std::move(Expression));
}

auto UI::Dimension::InvalidateValue() -> void
{
    switch(m_Type)
    {
    case UI::Dimension::Type::Height:
        {
            UI::Event HeightChangedEvent;
            
            HeightChangedEvent.SetTarget(std::addressof(m_Owner));
            g_UserInterface->DispatchHeightChangedEvent(HeightChangedEvent);
            
            break;
        }
    case UI::Dimension::Type::Left:
        {
            UI::Event LeftChangedEvent;
            
            LeftChangedEvent.SetTarget(std::addressof(m_Owner));
            g_UserInterface->DispatchLeftChangedEvent(LeftChangedEvent);
            
            break;
        }
    case UI::Dimension::Type::Top:
        {
            UI::Event TopChangedEvent;
            
            TopChangedEvent.SetTarget(std::addressof(m_Owner));
            g_UserInterface->DispatchTopChangedEvent(TopChangedEvent);
            
            break;
        }
    case UI::Dimension::Type::Width:
        {
            UI::Event WidthChangedEvent;
            
            WidthChangedEvent.SetTarget(std::addressof(m_Owner));
            g_UserInterface->DispatchWidthChangedEvent(WidthChangedEvent);
            
            break;
        }
    }
    Expressions::Variable::InvalidateValue();
}
