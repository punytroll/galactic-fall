/**
 * galactic-fall
 * Copyright (C) 2007-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <expressions/operators.h>

#include <graphics/color_rgbo.h>

#include "../file_handling.h"
#include "../globals.h"
#include "../real_time.h"
#include "key_event.h"
#include "label.h"
#include "list_box.h"
#include "list_box_text_item.h"
#include "mouse_button_event.h"
#include "save_game_dialog.h"
#include "text_button.h"

using namespace Expressions::Operators;

UI::SaveGameDialog::SaveGameDialog()
{
    SetTitle("Save Game");
    ConnectKeyCallback(std::bind(&UI::SaveGameDialog::m_OnKey, this, std::placeholders::_1));
    
    // create components
    auto CancelButton = new UI::TextButton{};
    auto OKButton = new UI::TextButton{};
    
    m_FileListBox = new UI::ListBox{};
    m_FileNameLabel = new UI::Label{};
    m_MessageLabel = new UI::Label{};
    // initialize components
    CancelButton->SetLeft(left(OKButton) - 10.0_c - width(CancelButton));
    CancelButton->SetTop(height(this) - 10.0_c - height(CancelButton));
    CancelButton->SetWidth(100.0_c);
    CancelButton->SetHeight(20.0_c);
    CancelButton->SetText("Cancel");
    CancelButton->ConnectClickedCallback(std::bind(&UI::SaveGameDialog::_Close, this, UI::Dialog::ClosingReason::CANCEL_BUTTON));
    m_FileListBox->SetLeft(10.0_c);
    m_FileListBox->SetTop(110.0_c);
    m_FileListBox->SetWidth(width(this) - 2.0_c * left(m_FileListBox));
    m_FileListBox->SetHeight(top(OKButton) - 10.0_c - top(m_FileListBox));
    m_FileListBox->SetHorizontalScrollBarVisible(false);
    m_FileListBox->ConnectSelectedItemChangedCallback(std::bind(&UI::SaveGameDialog::m_OnFileListBoxSelectedItemChanged, this));
    m_FileNameLabel->SetLeft(10.0_c);
    m_FileNameLabel->SetTop(bottom(m_MessageLabel) + 10.0_c);
    m_FileNameLabel->SetWidth(width(this) - 2.0_c * left(m_FileNameLabel));
    m_FileNameLabel->SetHeight(20.0_c);
    m_FileNameLabel->SetTextColor(Graphics::ColorRGBO(1.0f, 1.0f, 0.5f, 1.0f));
    m_FileNameLabel->SetBackgroundColor(Graphics::ColorRGBO(0.1f, 0.1f, 0.1f, 1.0f));
    m_FileNameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
    m_FileNameLabel->ConnectKeyCallback(std::bind(&UI::SaveGameDialog::m_OnFileNameLabelKey, this, std::placeholders::_1));
    m_FileNameLabel->GrabKeyboardFocus();
    m_MessageLabel->SetLeft(10.0_c);
    m_MessageLabel->SetTop(40.0_c);
    m_MessageLabel->SetWidth(width(this) - 2.0_c * left(m_MessageLabel));
    m_MessageLabel->SetHeight(30.0_c);
    m_MessageLabel->SetTextColor(Graphics::ColorRGBO(1.0f, 0.3, 0.3f, 1.0f));
    m_MessageLabel->SetWrap(true);
    m_MessageLabel->SetWordWrap(true);
    m_MessageLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
    OKButton->SetLeft(width(this) - 10.0_c - width(OKButton));
    OKButton->SetTop(height(this) - 10.0_c - height(OKButton));
    OKButton->SetWidth(100.0_c);
    OKButton->SetHeight(20.0_c);
    OKButton->SetText("OK");
    OKButton->ConnectClickedCallback(std::bind(&UI::SaveGameDialog::_Close, this, UI::Dialog::ClosingReason::OK_BUTTON));
    // add components
    AddSubWidget(CancelButton);
    AddSubWidget(m_FileListBox);
    AddSubWidget(m_FileNameLabel);
    AddSubWidget(m_MessageLabel);
    AddSubWidget(OKButton);
}

auto UI::SaveGameDialog::GetFilePath() -> std::string
{
    if(m_FileNameLabel->GetText() == "")
    {
        return "";
    }
    else
    {
        return m_DirectoryPath + '/' + m_FileNameLabel->GetText() + ".xml";
    }
}

auto UI::SaveGameDialog::SetDirectoryPath(std::string const & DirectoryPath) -> void
{
    m_DirectoryPath = DirectoryPath;
    while(m_FileListBox->GetContent()->GetSubWidgets().empty() == false)
    {
        m_FileListBox->GetContent()->GetSubWidgets().front()->Destroy();
    }
    if(IsExistingDirectory(m_DirectoryPath) == false)
    {
        ShowErrorMessage("Is not an existing directory: \"" + m_DirectoryPath + "\".");
    }
    for(auto DirectoryEntry : GetDirectoryEntries(m_DirectoryPath))
    {
        auto ListBoxTextItem = new UI::ListBoxTextItem{};
        
        ListBoxTextItem->SetText(DirectoryEntry.substr(0, DirectoryEntry.rfind(".xml")));
        m_FileListBox->GetContent()->AddSubWidget(ListBoxTextItem);
    }
}

auto UI::SaveGameDialog::ShowErrorMessage(std::string const & ErrorMessage) -> void
{
    if(m_MessageTimeoutNotification.IsValid() == true)
    {
        m_MessageTimeoutNotification.Dismiss();
    }
    m_MessageLabel->SetVisible(true);
    m_MessageLabel->SetText(ErrorMessage);
    m_MessageTimeoutNotification = g_RealTimeTimeoutNotifications->Add(RealTime::Get() + 2.0f, std::bind(&UI::Label::SetVisible, m_MessageLabel, false));
}

auto UI::SaveGameDialog::m_OnFileNameLabelKey(UI::KeyEvent & KeyEvent) -> void
{
    if(KeyEvent.GetPhase() == UI::Event::Phase::Target)
    {
        if((KeyEvent.GetKeyCode() == 22 /* BACKSPACE */) && (KeyEvent.IsDown() == true))
        {
            if(m_FileNameLabel->GetText().length() > 0)
            {
                m_FileNameLabel->SetText(m_FileNameLabel->GetText().substr(0, m_FileNameLabel->GetText().length() - 1));
                m_OnFileNameLabelTextChanged();
            }
        }
        else if((KeyEvent.GetString().empty() == false) && (KeyEvent.IsDown() == true))
        {
            m_FileNameLabel->SetText(m_FileNameLabel->GetText() + KeyEvent.GetString());
            m_OnFileNameLabelTextChanged();
        }
        KeyEvent.StopPropagation();
    }
}

auto UI::SaveGameDialog::m_OnFileNameLabelTextChanged() -> void
{
    for(auto ListBoxItem : m_FileListBox->GetContent()->GetSubWidgets())
    {
        auto ListBoxTextItem = dynamic_cast<UI::ListBoxTextItem *>(ListBoxItem);
        
        if(ListBoxTextItem != nullptr)
        {
            if(ListBoxTextItem->GetText() == m_FileNameLabel->GetText())
            {
                m_FileListBox->SetSelectedItem(ListBoxTextItem);
                
                break;
            }
        }
    }
}

auto UI::SaveGameDialog::m_OnFileListBoxSelectedItemChanged() -> void
{
    auto SelectedFileListBoxItem = dynamic_cast<UI::ListBoxTextItem *>(m_FileListBox->GetSelectedItem());
    
    if(SelectedFileListBoxItem != nullptr)
    {
        m_FileNameLabel->SetText(SelectedFileListBoxItem->GetText());
    }
    else
    {
        m_FileNameLabel->SetText("");
    }
}

auto UI::SaveGameDialog::m_OnKey(UI::KeyEvent & KeyEvent) -> void
{
    if((KeyEvent.GetKeyCode() == 9 /* ESCAPE */) && (KeyEvent.IsDown() == true))
    {
        _Close(UI::Dialog::ClosingReason::ESCAPE_KEY);
    }
    else if((KeyEvent.GetKeyCode() == 36 /* RETURN */) && (KeyEvent.IsDown() == true))
    {
        _Close(UI::Dialog::ClosingReason::RETURN_KEY);
    }
}
