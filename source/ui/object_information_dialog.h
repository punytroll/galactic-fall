/**
 * galactic-fall
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__OBJECT_INFORMATION_DIALOG_H
#define GALACTIC_FALL__SOURCE__UI__OBJECT_INFORMATION_DIALOG_H

#include "window.h"

class Object;

namespace UI
{
    class ScrollBox;
    class TextButton;
    
    class ObjectInformationDialog : public UI::Window
    {
    public:
        class ViewedObject;
        
        explicit ObjectInformationDialog(Object & Object);
        
    private:
        auto m_AddObjectProperty(UI::Widget * PreviousWidget, float Indentation, Object * Object) -> UI::Widget *;
        auto m_AddSeparator(UI::Widget * PreviousWidget, float Indentation, std::string const & Separator) -> UI::Widget *;
        auto m_AddString(UI::Widget * PreviousWidget, float Indentation, std::string const & String) -> UI::Widget *;
        auto m_AddDynamicStringProperty(UI::Widget * PreviousWidget, float Indentation, std::string const & PropertyName, std::function<auto () -> std::string> PropertyValueGetter) -> UI::Widget *;
        auto m_AddStaticStringProperty(UI::Widget * PreviousWidget, float Indentation, std::string const & PropertyName, std::string const & PropertyValue) -> UI::Widget *;
        auto m_NavigateBackwards() -> void;
        auto m_OnBackButtonClicked() -> void;
        auto m_OnBackButtonUpdating(UI::TextButton * BackButton, float RealTimeSeconds, float GameTimeSeconds) -> void;
        auto m_OnCloseButtonClicked() -> void;
        auto m_OnCurrentObjectDestroying() -> void;
        auto m_OnDestroying(UI::Event & DestroyingEvent) -> void;
        auto m_OnKey(UI::KeyEvent & KeyEvent) -> void;
        auto m_OnObjectButtonClicked(Object * Object) -> void;
        auto m_OnRefreshButtonClicked() -> void;
        auto m_OnRefreshButtonUpdating(UI::TextButton * RefreshButton, float RealTimeSeconds, float GameTimeSeconds) -> void;
        auto m_PushObject(Object & Object) -> void;
        auto m_Refresh() -> void;
        auto m_ViewObject(Object & Object) -> void;
        
        std::vector<UI::ObjectInformationDialog::ViewedObject> m_ViewedObjectStack;
        UI::ScrollBox * m_PropertiesScrollBox = nullptr;
    };
}

#endif
