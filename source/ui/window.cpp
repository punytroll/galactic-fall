/**
 * galactic-fall
 * Copyright (C) 2006-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <expressions/operators.h>

#include <graphics/color_rgbo.h>

#include "../globals.h"
#include "border.h"
#include "label.h"
#include "mouse_button_event.h"
#include "mouse_move_event.h"
#include "user_interface.h"
#include "window.h"

using namespace Expressions::Operators;

UI::Window::Window()
{
    SetBackgroundColor(Graphics::ColorRGBO(0.2f, 0.2f, 0.2f, 1.0f));
    
    // create components
    auto Border = new UI::Border{};
    
    m_ResizeDragBox = new Widget{};
    m_TitleLabel = new UI::Label{};
    // initialize components
    Border->SetLeft(0.0_c);
    Border->SetTop(0.0_c);
    Border->SetWidth(width(this));
    Border->SetHeight(height(this));
    Border->SetLineWidth(1.0f);
    Border->SetColor(Graphics::ColorRGBO(0.4f, 0.4f, 0.4f, 1.0f));
    m_TitleLabel->SetLeft(10.0_c);
    m_TitleLabel->SetTop(10.0_c);
    m_TitleLabel->SetWidth(width(this) - 2.0_c * 10.0_c);
    m_TitleLabel->SetHeight(20.0_c);
    m_TitleLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Center);
    m_TitleLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
    m_TitleLabel->SetBackgroundColor(Graphics::ColorRGBO(0.23f, 0.35f, 0.55f, 1.0f));
    m_TitleLabel->ConnectMouseButtonCallback(std::bind(&UI::Window::m_OnTitleLabelMouseButton, this, std::placeholders::_1));
    m_TitleLabel->ConnectMouseMoveCallback(std::bind(&UI::Window::m_OnTitleLabelMouseMove, this, std::placeholders::_1));
    m_TitleLabel->ConnectUpdatingCallback(std::bind(&UI::Window::m_OnTitleLabelUpdating, this, std::placeholders::_1, std::placeholders::_2));
    m_ResizeDragBox->SetLeft(width(this) - 9.0_c);
    m_ResizeDragBox->SetTop(height(this) - 9.0_c);
    m_ResizeDragBox->SetWidth(7.0_c);
    m_ResizeDragBox->SetHeight(7.0_c);
    m_ResizeDragBox->SetBackgroundColor(Graphics::ColorRGBO(0.23f, 0.35f, 0.55f, 1.0f));
    m_ResizeDragBox->ConnectMouseButtonCallback(std::bind(&UI::Window::m_OnResizeDragBoxMouseButton, this, std::placeholders::_1));
    m_ResizeDragBox->ConnectMouseMoveCallback(std::bind(&UI::Window::m_OnResizeDragBoxMouseMove, this, std::placeholders::_1));
    // add components
    AddSubWidget(Border);
    AddSubWidget(m_ResizeDragBox);
    AddSubWidget(m_TitleLabel);
}

auto UI::Window::SetTitle(std::string_view Title) -> void
{
    m_TitleLabel->SetText(std::string{Title});
}

auto UI::Window::HideResizeDragBox() -> void
{
    m_ResizeDragBox->SetVisible(false);
}

auto UI::Window::ShowResizeDragBox() -> void
{
    m_ResizeDragBox->SetVisible(true);
}

auto UI::Window::m_OnTitleLabelMouseButton(UI::MouseButtonEvent & MouseButtonEvent) -> void
{
    if((MouseButtonEvent.GetPhase() == UI::Event::Phase::Bubbling) && (MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left))
    {
        if(MouseButtonEvent.IsDown() == true)
        {
            m_GrabPosition = MouseButtonEvent.GetPosition();
            g_UserInterface->SetCaptureWidget(m_TitleLabel);
        }
        else
        {
            g_UserInterface->ReleaseCaptureWidget();
        }
    }
}

auto UI::Window::m_OnResizeDragBoxMouseButton(UI::MouseButtonEvent & MouseButtonEvent) -> void
{
    if((MouseButtonEvent.GetPhase() == UI::Event::Phase::Bubbling) && (MouseButtonEvent.GetMouseButton() == UI::MouseButtonEvent::MouseButton::Left))
    {
        if(MouseButtonEvent.IsDown() == true)
        {
            m_GrabPosition = MouseButtonEvent.GetPosition();
            g_UserInterface->SetCaptureWidget(m_ResizeDragBox);
        }
        else
        {
            g_UserInterface->ReleaseCaptureWidget();
        }
    }
}

auto UI::Window::m_OnTitleLabelMouseMove(UI::MouseMoveEvent & MouseMoveEvent) -> void
{
    if((MouseMoveEvent.GetPhase() == UI::Event::Phase::Target) && (g_UserInterface->GetCaptureWidget() == m_TitleLabel))
    {
        SetLeft(constant(GetLeft() + MouseMoveEvent.GetPosition()[0] - m_GrabPosition[0]));
        SetTop(constant(GetTop() + MouseMoveEvent.GetPosition()[1] - m_GrabPosition[1]));
    }
}

auto UI::Window::m_OnResizeDragBoxMouseMove(UI::MouseMoveEvent & MouseMoveEvent) -> void
{
    if((MouseMoveEvent.GetPhase() == UI::Event::Phase::Target) && (g_UserInterface->GetCaptureWidget() == m_ResizeDragBox))
    {
        SetWidth(constant(GetWidth() + MouseMoveEvent.GetPosition()[0] - m_GrabPosition[0]));
        SetHeight(constant(GetHeight() + MouseMoveEvent.GetPosition()[1] - m_GrabPosition[1]));
    }
}

auto UI::Window::m_OnTitleLabelUpdating([[maybe_unused]] float RealTimeSeconds, [[maybe_unused]] float GameTimeSeconds) -> void
{
    auto IsGlobalKeyFocus = true;
    auto CurrentWidget = static_cast<Widget *>(this);
    
    while(CurrentWidget != nullptr)
    {
        if((CurrentWidget->GetSupWidget() != nullptr) && (CurrentWidget != CurrentWidget->GetSupWidget()->GetKeyFocusWidget()))
        {
            IsGlobalKeyFocus = false;
            
            break;
        }
        else
        {
            CurrentWidget = CurrentWidget->GetSupWidget();
        }
    }
    assert(m_TitleLabel != nullptr);
    if(IsGlobalKeyFocus == true)
    {
        m_TitleLabel->SetBackgroundColor(Graphics::ColorRGBO(0.23f, 0.35f, 0.55f, 1.0f));
    }
    else
    {
        m_TitleLabel->SetBackgroundColor(Graphics::ColorRGBO(0.30f, 0.30f, 0.30, 1.0f));
    }
}
