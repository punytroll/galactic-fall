/**
 * galactic-fall
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__UI__OBJECT_INFORMATION_DIALOG_H
#define GALACTIC_FALL__SOURCE__UI__OBJECT_INFORMATION_DIALOG_H

#include "window.h"

class Object;
class Quaternion;
class Vector3f;

namespace UI
{
    class ScrollBox;
    
    class ObjectInformationDialog : public UI::Window
    {
    public:
        explicit ObjectInformationDialog(Object & Object);
        
    private:
        auto m_AddObjectProperty(UI::Widget * PreviousWidget, float Indentation, Object * Object) -> UI::Widget *;
        auto m_AddSeparator(UI::Widget * PreviousWidget, float Indentation, std::string const & Separator) -> UI::Widget *;
        auto m_AddString(UI::Widget * PreviousWidget, float Indentation, std::string const & String) -> UI::Widget *;
        auto m_AddStringProperty(UI::Widget * PreviousWidget, float Indentation, std::string const & PropertyName, std::string const & PropertyValue) -> UI::Widget *;
        auto m_GetOrientationString(Quaternion const & Orientation) -> std::string;
        auto m_GetPositionString(Vector3f const & Position) -> std::string;
        auto m_OnCloseClicked() -> void;
        auto m_OnDestroying(UI::Event & DestroyingEvent) -> void;
        auto m_OnKey(UI::KeyEvent & KeyEvent) -> void;
        auto m_OnObjectButtonClicked(Object * Object) -> void;
        auto m_OnObjectDestroying() -> void;
        auto m_OnRefreshClicked() -> void;
        auto m_Refresh() -> void;
        auto m_SetObject(Object * Object) -> void;
        auto m_ViewObject(Object * Object) -> void;
        
        Object * m_Object = nullptr;
        Signals::Connection m_ObjectDestroyingConnection;
        UI::ScrollBox * m_PropertiesScrollBox = nullptr;
    };
}

#endif
