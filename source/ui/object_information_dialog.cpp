/**
 * galactic-fall
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <expressions/operators.h>

#include <graphics/color_rgbo.h>

#include <signals/signal.h>

#include "../globals.h"
#include "../message.h"
#include "../object.h"
#include "../object_aspect_accessory.h"
#include "../object_aspect_messages.h"
#include "../object_aspect_name.h"
#include "../object_aspect_object_container.h"
#include "../object_aspect_outfitting.h"
#include "../object_aspect_physical.h"
#include "../object_aspect_position.h"
#include "../object_aspect_visualization.h"
#include "../slot.h"
#include "../slot_class.h"
#include "key_event.h"
#include "label.h"
#include "mouse_button_event.h"
#include "object_information_dialog.h"
#include "scroll_bar.h"
#include "scroll_box.h"
#include "text_button.h"
#include "user_interface.h"

using namespace Expressions::Operators;

namespace UI
{
    class ObjectInformationDialog::ViewedObject
    {
    public:
        ViewedObject(Object & Object) :
            m_Object{std::addressof(Object)}
        {
            m_ObjectDestroyingConnection = Object.ConnectDestroyingCallback(std::bind(&UI::ObjectInformationDialog::ViewedObject::m_OnObjectDestroying, this));
        }
        
        ViewedObject(UI::ObjectInformationDialog::ViewedObject const & Other) = delete;
        ViewedObject(UI::ObjectInformationDialog::ViewedObject && Other) = default;
        
        auto m_OnObjectDestroying() -> void
        {
            assert(m_Object != nullptr);
            m_Object = nullptr;
            assert(m_ObjectDestroyingConnection.IsConnected() == true);
            m_ObjectDestroyingConnection.Disconnect();
            assert(m_ObjectDestroyingConnection.IsConnected() == false);
            if(m_ObjectDestroyingCallback != nullptr)
            {
                m_ObjectDestroyingCallback();
            }
        }
        
        Object * m_Object;
        std::function<auto () -> void> m_ObjectDestroyingCallback;
        Signals::ScopedConnection m_ObjectDestroyingConnection;
        std::string m_Title;
    };
    
    namespace
    {
        class ObjectPropertyWidget : public UI::Widget
        {
        public:
            ObjectPropertyWidget(float Indentation, Object & Object) :
                m_Object{std::addressof(Object)}
            {
                assert(m_Object != nullptr);
                m_ObjectDestroyingConnection = m_Object->ConnectDestroyingCallback(std::bind(&UI::ObjectPropertyWidget::m_OnObjectDestroying, this));
                ConnectDestroyingCallback(std::bind(&UI::ObjectPropertyWidget::m_OnDestroying, this, std::placeholders::_1));
                // create components
                m_Button = new UI::TextButton{};
                // initialize components
                m_Button->SetLeft(constant(Indentation));
                m_Button->SetTop(5.0_c);
                m_Button->SetWidth(width(this) - constant(Indentation));
                m_Button->SetHeight(height(this) - 2.0_c * 5.0_c);
                m_Button->SetText(m_Object->GetTypeIdentifier() + " / " + m_Object->GetSubTypeIdentifier());
                m_Button->ConnectClickedCallback(std::bind(&Signals::Signal<auto () -> void>::Emit, std::addressof(m_ObjectButtonClickedSignal)));
                // add components
                AddSubWidget(m_Button);
            }
            
            auto ConnectObjectButtonClickedCallback(std::function<void()> ClickedHandler) -> Signals::Connection
            {
                return m_ObjectButtonClickedSignal.Connect(ClickedHandler);
            }
            
        private:
            auto m_OnDestroying(UI::Event & DestroyingEvent) -> void
            {
                if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
                {
                    if(m_Object != nullptr)
                    {
                        m_Object = nullptr;
                        assert(m_ObjectDestroyingConnection.IsValid() == true);
                        m_ObjectDestroyingConnection.Disconnect();
                    }
                    assert(m_ObjectDestroyingConnection.IsValid() == false);
                }
            }
            
            void m_OnObjectDestroying(void)
            {
                assert(m_Object != nullptr);
                m_Object = nullptr;
                assert(m_ObjectDestroyingConnection.IsValid() == true);
                m_ObjectDestroyingConnection.Disconnect();
                assert(m_ObjectDestroyingConnection.IsValid() == false);
                m_Button->SetText("<destroyed>");
                m_Button->SetEnabled(false);
            }
            
            UI::TextButton * m_Button;
            Signals::Signal<auto () -> void> m_ObjectButtonClickedSignal;
            Object * m_Object;
            Signals::Connection m_ObjectDestroyingConnection;
        };
        
        class TextPropertyWidget : public UI::Widget
        {
        public:
            TextPropertyWidget(float Indentation, std::string const & PropertyName)
            {
                m_NameLabel = new UI::Label{};
                m_NameLabel->SetLeft(constant(Indentation));
                m_NameLabel->SetTop(0.0_c);
                m_NameLabel->SetWidth(constant(6.0f * (PropertyName.length() + 1)));
                m_NameLabel->SetHeight(height(this));
                m_NameLabel->SetText(PropertyName + ":");
                m_NameLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
                m_ValueLabel = new UI::Label{};
                /// @todo why not right()
                m_ValueLabel->SetLeft(constant(m_NameLabel->GetRight()));
                m_ValueLabel->SetTop(0.0_c);
                m_ValueLabel->SetWidth(width(this) - right(m_NameLabel));
                m_ValueLabel->SetHeight(height(this));
                m_ValueLabel->SetHorizontalAlignment(UI::Label::HorizontalAlignment::Right);
                m_ValueLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
                AddSubWidget(m_NameLabel);
                AddSubWidget(m_ValueLabel);
            }
            
            auto GetValueLabel() -> UI::Label &
            {
                assert(m_ValueLabel != nullptr);
                
                return *m_ValueLabel;
            }
            
        private:
            UI::Label * m_NameLabel = nullptr;
            UI::Label * m_ValueLabel = nullptr;
        };
        
        auto AddTextPropertyWidget(UI::ScrollBox & ScrollBox, UI::Widget * PreviousWidget, float Indentation, std::string const & PropertyName) -> UI::TextPropertyWidget *
        {
            auto TextPropertyWidget = new UI::TextPropertyWidget{Indentation, PropertyName};
            
            TextPropertyWidget->SetLeft(10.0_c);
            if(PreviousWidget != nullptr)
            {
                TextPropertyWidget->SetTop(bottom(PreviousWidget));
            }
            else
            {
                TextPropertyWidget->SetTop(0.0_c);
            }
            TextPropertyWidget->SetWidth(width(ScrollBox.GetContent()) - 2.0_c * 10.0_c);
            TextPropertyWidget->SetHeight(20.0_c);
            ScrollBox.GetContent()->AddSubWidget(TextPropertyWidget);
            
            return TextPropertyWidget;
        }
        
        auto ToString(std::integral auto Value) -> std::string
        {
            return std::to_string(Value);
        }
        
        auto ToString(float Value) -> std::string
        {
            return std::to_string(Value);
        }
        
        auto ToString(Vector3f const & Value) -> std::string
        {
            return "x=" + std::to_string(Value[0]) + "; y=" + std::to_string(Value[1]) + "; z=" + std::to_string(Value[2]);
        }
        
        auto ToString(Quaternion const & Value) -> std::string
        {
            return "w=" + std::to_string(Value[0]) + "; x=" + std::to_string(Value[1]) + "; y=" + std::to_string(Value[2]) + "; z=" + std::to_string(Value[3]);
        }
    }
}

UI::ObjectInformationDialog::ObjectInformationDialog(Object & Object)
{
    ConnectDestroyingCallback(std::bind(&UI::ObjectInformationDialog::m_OnDestroying, this, std::placeholders::_1));
    ConnectKeyCallback(std::bind(&UI::ObjectInformationDialog::m_OnKey, this, std::placeholders::_1));
    
    // create components
    auto BackButton = new UI::TextButton{};
    auto CloseButton = new UI::TextButton{};
    auto RefreshButton = new UI::TextButton{};
    
    m_PropertiesScrollBox = new UI::ScrollBox{};
    // initialize components
    BackButton->SetLeft(10.0_c);
    BackButton->SetTop(height(this) - 10.0_c - height(BackButton));
    BackButton->SetWidth(100.0_c);
    BackButton->SetHeight(20.0_c);
    BackButton->SetText("Back");
    BackButton->ConnectClickedCallback(std::bind(&UI::ObjectInformationDialog::m_OnBackButtonClicked, this));
    BackButton->ConnectUpdatingCallback(std::bind(&UI::ObjectInformationDialog::m_OnBackButtonUpdating, this, BackButton, std::placeholders::_1, std::placeholders::_2));
    CloseButton->SetLeft(width(this) - 10.0_c - width(CloseButton));
    CloseButton->SetTop(height(this) - 10.0_c - height(CloseButton));
    CloseButton->SetWidth(100.0_c);
    CloseButton->SetHeight(20.0_c);
    CloseButton->SetText("Close");
    CloseButton->ConnectClickedCallback(std::bind(&UI::ObjectInformationDialog::m_OnCloseButtonClicked, this));
    m_PropertiesScrollBox->SetLeft(10.0_c);
    m_PropertiesScrollBox->SetTop(40.0_c);
    m_PropertiesScrollBox->SetWidth(width(this) - 2.0_c * left(m_PropertiesScrollBox));
    m_PropertiesScrollBox->SetHeight(top(CloseButton) - 10.0_c - top(m_PropertiesScrollBox));
    m_PropertiesScrollBox->GetContent()->SetWidth(width(m_PropertiesScrollBox->GetView()));
    m_PropertiesScrollBox->SetHorizontalScrollBarVisible(false);
    RefreshButton->SetLeft(left(CloseButton) - 10.0_c - width(RefreshButton));
    RefreshButton->SetTop(height(this) - 10.0_c - height(RefreshButton));
    RefreshButton->SetWidth(100.0_c);
    RefreshButton->SetHeight(20.0_c);
    RefreshButton->SetText("Refresh");
    RefreshButton->ConnectClickedCallback(std::bind(&UI::ObjectInformationDialog::m_OnRefreshButtonClicked, this));
    RefreshButton->ConnectUpdatingCallback(std::bind(&UI::ObjectInformationDialog::m_OnRefreshButtonUpdating, this, RefreshButton, std::placeholders::_1, std::placeholders::_2));
    // add components
    AddSubWidget(BackButton);
    AddSubWidget(CloseButton);
    AddSubWidget(m_PropertiesScrollBox);
    AddSubWidget(RefreshButton);
    m_ViewObject(Object);
}

auto UI::ObjectInformationDialog::m_AddObjectProperty(UI::Widget * PreviousWidget, float Indentation, Object * Object) -> UI::Widget *
{
    assert(Object != nullptr);
    
    auto ObjectPropertyWidget = new UI::ObjectPropertyWidget{Indentation, *Object};
    
    ObjectPropertyWidget->SetLeft(10.0_c);
    if(PreviousWidget != nullptr)
    {
        ObjectPropertyWidget->SetTop(bottom(PreviousWidget));
    }
    else
    {
        ObjectPropertyWidget->SetTop(0.0_c);
    }
    ObjectPropertyWidget->SetWidth(width(m_PropertiesScrollBox->GetContent()) - 2.0_c * 10.0_c);
    ObjectPropertyWidget->SetHeight(30.0_c);
    ObjectPropertyWidget->ConnectObjectButtonClickedCallback(std::bind(&UI::ObjectInformationDialog::m_OnObjectButtonClicked, this, Object));
    m_PropertiesScrollBox->GetContent()->AddSubWidget(ObjectPropertyWidget);
    
    return ObjectPropertyWidget;
}

auto UI::ObjectInformationDialog::m_AddSeparator(UI::Widget * PreviousWidget, float Indentation, std::string const & Separator) -> UI::Widget *
{
    // create components
    auto SeparatorDisplay = new UI::Widget{};
    auto SeparatorLabel = new UI::Label{};
    
    // initialize components
    SeparatorDisplay->SetLeft(10.0_c);
    if(PreviousWidget != nullptr)
    {
        SeparatorDisplay->SetTop(bottom(PreviousWidget));
    }
    else
    {
        SeparatorDisplay->SetTop(0.0_c);
    }
    SeparatorDisplay->SetWidth(width(m_PropertiesScrollBox->GetContent()) - 2.0_c * 10.0_c);
    SeparatorDisplay->SetHeight(20.0_c);
    SeparatorLabel->SetLeft(constant(Indentation));
    SeparatorLabel->SetTop(0.0_c);
    SeparatorLabel->SetWidth(width(SeparatorDisplay));
    SeparatorLabel->SetHeight(height(SeparatorDisplay));
    SeparatorLabel->SetText(Separator);
    SeparatorLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
    SeparatorLabel->SetTextColor(Graphics::ColorRGBO(0.5f, 0.8f, 1.0f, 1.0f));
    // add components
    m_PropertiesScrollBox->GetContent()->AddSubWidget(SeparatorDisplay);
    SeparatorDisplay->AddSubWidget(SeparatorLabel);
    
    return SeparatorDisplay;
}

auto UI::ObjectInformationDialog::m_AddString(UI::Widget * PreviousWidget, float Indentation, std::string const & String) -> UI::Widget *
{
    // create components
    auto StringDisplay = new UI::Widget{};
    auto StringLabel = new UI::Label{};
    
    // initialize components
    StringDisplay->SetLeft(10.0_c);
    if(PreviousWidget != nullptr)
    {
        StringDisplay->SetTop(bottom(PreviousWidget));
    }
    else
    {
        StringDisplay->SetTop(0.0_c);
    }
    StringDisplay->SetWidth(width(m_PropertiesScrollBox->GetContent()) - 2.0_c * 10.0_c);
    StringDisplay->SetHeight(20.0_c);
    StringLabel->SetLeft(constant(Indentation));
    StringLabel->SetTop(0.0_c);
    StringLabel->SetWidth(width(StringDisplay));
    StringLabel->SetHeight(height(StringDisplay));
    StringLabel->SetText(String);
    StringLabel->SetVerticalAlignment(UI::Label::VerticalAlignment::Center);
    // add components
    m_PropertiesScrollBox->GetContent()->AddSubWidget(StringDisplay);
    StringDisplay->AddSubWidget(StringLabel);
    
    return StringDisplay;
}

auto UI::ObjectInformationDialog::m_AddDynamicStringProperty(UI::Widget * PreviousWidget, float Indentation, std::string const & PropertyName, std::function<auto () -> std::string> PropertyValueGetter) -> UI::Widget *
{
    auto TextPropertyWidget = UI::AddTextPropertyWidget(*m_PropertiesScrollBox, PreviousWidget, Indentation, PropertyName);
    
    TextPropertyWidget->SetBackgroundColor(Graphics::ColorRGBO{0.0f, 0.3f, 0.0f, 0.3f});
    TextPropertyWidget->GetValueLabel().ConnectUpdatingCallback([&PropertyValueLabel = TextPropertyWidget->GetValueLabel(), PropertyValueGetter = std::move(PropertyValueGetter)]([[maybe_unused]] float RealTimeSeconds, [[maybe_unused]] float GameTimeSeconds)
                                                                {
                                                                    PropertyValueLabel.SetText(PropertyValueGetter());
                                                                });
    
    return TextPropertyWidget;
}

auto UI::ObjectInformationDialog::m_AddStaticStringProperty(UI::Widget * PreviousWidget, float Indentation, std::string const & PropertyName, std::string const & PropertyValue) -> UI::Widget *
{
    auto TextPropertyWidget = UI::AddTextPropertyWidget(*m_PropertiesScrollBox, PreviousWidget, Indentation, PropertyName);
    
    TextPropertyWidget->GetValueLabel().SetText(PropertyValue);
    
    return TextPropertyWidget;
}

auto UI::ObjectInformationDialog::m_NavigateBackwards() -> void
{
    assert(m_ViewedObjectStack.empty() == false);
    m_ViewedObjectStack.pop_back();
    if(m_ViewedObjectStack.empty() == false)
    {
        if(auto & TopViewedObject = m_ViewedObjectStack.back(); TopViewedObject.m_Object != nullptr)
        {
            TopViewedObject.m_ObjectDestroyingCallback = std::bind(&UI::ObjectInformationDialog::m_OnCurrentObjectDestroying, this);
        }
    }
    m_Refresh();
}

auto UI::ObjectInformationDialog::m_OnBackButtonClicked() -> void
{
    m_NavigateBackwards();
}

auto UI::ObjectInformationDialog::m_OnBackButtonUpdating(UI::TextButton * BackButton, [[maybe_unused]] float RealTimeSeconds, [[maybe_unused]] float GameTimeSeconds) -> void
{
    assert(BackButton != nullptr);
    BackButton->SetEnabled(m_ViewedObjectStack.size() > 1);
}

auto UI::ObjectInformationDialog::m_OnCloseButtonClicked() -> void
{
    Destroy();
}

auto UI::ObjectInformationDialog::m_OnCurrentObjectDestroying() -> void
{
    m_Refresh();
}

auto UI::ObjectInformationDialog::m_OnDestroying([[maybe_unused]] UI::Event & DestroyingEvent) -> void
{
    if(DestroyingEvent.GetPhase() == UI::Event::Phase::Target)
    {
        m_ViewedObjectStack.clear();
    }
}

auto UI::ObjectInformationDialog::m_OnKey(UI::KeyEvent & KeyEvent) -> void
{
    if((KeyEvent.GetPhase() == UI::Event::Phase::Bubbling) && (KeyEvent.GetKeyCode() == 9 /* ESCAPE */) && (KeyEvent.IsDown() == true))
    {
        Destroy();
        KeyEvent.StopPropagation();
    }
    if((KeyEvent.GetPhase() == UI::Event::Phase::Bubbling) && (KeyEvent.GetKeyCode() == 22 /* BACKSPACE */) && (KeyEvent.IsDown() == true))
    {
        if(m_ViewedObjectStack.size() > 1)
        {
            m_NavigateBackwards();
        }
    }
}

auto UI::ObjectInformationDialog::m_OnObjectButtonClicked(Object * Object) -> void
{
    assert(Object != nullptr);
    if((g_UserInterface->IsKeyCodePressed(50) == true /* LEFT SHIFT */) || (g_UserInterface->IsKeyCodePressed(62) == true /* RIGHT SHIFT */))
    {
        auto ObjectInformationDialog = new UI::ObjectInformationDialog{*Object};
        
        ObjectInformationDialog->SetName("object_information(" + Object->GetTypeIdentifier() + " / " + Object->GetSubTypeIdentifier() + ")");
        ObjectInformationDialog->SetLeft(100.0_c);
        ObjectInformationDialog->SetTop(400.0_c);
        ObjectInformationDialog->SetWidth(500.0_c);
        ObjectInformationDialog->SetHeight(300.0_c);
        GetRootWidget()->AddSubWidget(ObjectInformationDialog);
        ObjectInformationDialog->GrabKeyboardFocus();
    }
    else
    {
        m_ViewObject(*Object);
    }
}

auto UI::ObjectInformationDialog::m_OnRefreshButtonClicked() -> void
{
    m_Refresh();
}

auto UI::ObjectInformationDialog::m_OnRefreshButtonUpdating(UI::TextButton * RefreshButton, [[maybe_unused]] float RealTimeSeconds, [[maybe_unused]] float GameTimeSeconds) -> void
{
    assert(RefreshButton != nullptr);
    assert(m_ViewedObjectStack.empty() == false);
    RefreshButton->SetEnabled(m_ViewedObjectStack.back().m_Object != nullptr);
}

auto UI::ObjectInformationDialog::m_Refresh() -> void
{
    m_PropertiesScrollBox->Clear();
    m_PropertiesScrollBox->GetContent()->SetTop(0.0_c);
    
    auto PreviousWidget = static_cast<UI::Widget *>(nullptr);
    
    assert(m_ViewedObjectStack.empty() == false);
    
    auto & TopViewedObject = m_ViewedObjectStack.back();
    
    SetTitle(TopViewedObject.m_Title);
    // fill the properties view
    if(TopViewedObject.m_Object != nullptr)
    {
        auto & Object = *(TopViewedObject.m_Object);
        
        PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 0.0f, "Type", Object.GetTypeIdentifier());
        PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 0.0f, "Sub-Type", Object.GetSubTypeIdentifier());
        PreviousWidget = m_AddSeparator(PreviousWidget, 0.0f, "Container");
        if(Object.GetContainer() != nullptr)
        {
            PreviousWidget = m_AddObjectProperty(PreviousWidget, 20.0f, Object.GetContainer());
        }
        else
        {
            PreviousWidget = m_AddString(PreviousWidget, 20.0f, "No container.");
        }
        if(Object.GetAspectName() != nullptr)
        {
            PreviousWidget = m_AddSeparator(PreviousWidget, 0.0f, "Name");
            PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 20.0f, "Name", Object.GetAspectName()->GetName());
        }
        if(Object.GetAspectObjectContainer() != nullptr)
        {
            PreviousWidget = m_AddSeparator(PreviousWidget, 0.0f, "Object Container");
            for(auto Content : Object.GetAspectObjectContainer()->GetContent())
            {
                PreviousWidget = m_AddObjectProperty(PreviousWidget, 20.0f, Content);
            }
        }
        if(Object.GetAspectPosition() != nullptr)
        {
            PreviousWidget = m_AddSeparator(PreviousWidget, 0.0f, "Position");
            PreviousWidget = m_AddDynamicStringProperty(PreviousWidget, 20.0f, "Position", [&Object]()
                                                                                           {
                                                                                               return UI::ToString(Object.GetAspectPosition()->GetPosition());
                                                                                           });
            PreviousWidget = m_AddDynamicStringProperty(PreviousWidget, 20.0f, "Orientation", [&Object]()
                                                                                              {
                                                                                                  return UI::ToString(Object.GetAspectPosition()->GetOrientation());
                                                                                              });
        }
        if(Object.GetAspectPhysical() != nullptr)
        {
            PreviousWidget = m_AddSeparator(PreviousWidget, 0.0f, "Physical");
            PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 20.0f, "Radial Size", UI::ToString(Object.GetAspectPhysical()->GetRadialSize()));
            PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 20.0f, "Space Requirement", UI::ToString(Object.GetAspectPhysical()->GetSpaceRequirement()));
        }
        if(Object.GetAspectAccessory() != nullptr)
        {
            PreviousWidget = m_AddSeparator(PreviousWidget, 0.0f, "Accessory");
            PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 20.0f, "Slot Class Identifier", Object.GetAspectAccessory()->GetSlotClassIdentifier());
            if(Object.GetAspectAccessory()->GetSlot() != nullptr)
            {
                PreviousWidget = m_AddString(PreviousWidget, 20.0f, "Mounted to:");
                PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 40.0f, "Slot Identifier", Object.GetAspectAccessory()->GetSlot()->GetIdentifier());
                PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 40.0f, "Slot Name", Object.GetAspectAccessory()->GetSlot()->GetName());
                PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 40.0f, "Slot Class Identifier", Object.GetAspectAccessory()->GetSlot()->GetSlotClass()->GetIdentifier());
            }
            else
            {
                PreviousWidget = m_AddString(PreviousWidget, 20.0f, "Not mounted.");
            }
        }
        if(Object.GetAspectMessages() != nullptr)
        {
            PreviousWidget = m_AddSeparator(PreviousWidget, 0.0f, "Messages");
            PreviousWidget = m_AddDynamicStringProperty(PreviousWidget, 20.0f, "Number of messages", [&Object]()
                                                                                                     {
                                                                                                         return UI::ToString(Object.GetAspectMessages()->GetMessages().size());
                                                                                                     });
            for(auto Message : Object.GetAspectMessages()->GetMessages())
            {
                PreviousWidget = m_AddString(PreviousWidget, 40.0f, Message->GetTypeIdentifier());
            }
        }
        if(Object.GetAspectOutfitting() != nullptr)
        {
            PreviousWidget = m_AddSeparator(PreviousWidget, 0.0f, "Outfitting");
            PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 20.0f, "Number of slots", UI::ToString(Object.GetAspectOutfitting()->GetSlots().size()));
            for(auto const & [SlotIdentifier, Slot] : Object.GetAspectOutfitting()->GetSlots())
            {
                PreviousWidget = m_AddStaticStringProperty(PreviousWidget, 40.0f, Slot->GetName(), Slot->GetSlotClass()->GetIdentifier());
            }
        }
        if(Object.GetAspectUpdate() != nullptr)
        {
            PreviousWidget = m_AddSeparator(PreviousWidget, 0.0f, "Update");
        }
        if(Object.GetAspectVisualization() != nullptr)
        {
            PreviousWidget = m_AddSeparator(PreviousWidget, 0.0f, "Visualization");
            PreviousWidget = m_AddDynamicStringProperty(PreviousWidget, 20.0f, "Number of visualizations", [&Object]()
                                                                                                           {
                                                                                                               return UI::ToString(Object.GetAspectVisualization()->GetVisualizations().size());
                                                                                                           });
        }
    }
    else
    {
        PreviousWidget = m_AddString(nullptr, 0.0f, "No object has been selected, or the object has been destroyed.");
    }
    m_PropertiesScrollBox->GetContent()->SetHeight(bottom(PreviousWidget));
}

auto UI::ObjectInformationDialog::m_PushObject(Object & Object) -> void
{
    if(m_ViewedObjectStack.empty() == false)
    {
        auto & TopViewedObject = m_ViewedObjectStack.back();
        
        if(TopViewedObject.m_Object != nullptr)
        {
            assert(TopViewedObject.m_ObjectDestroyingCallback != nullptr);
            TopViewedObject.m_ObjectDestroyingCallback = nullptr;
        }
        else
        {
            assert(TopViewedObject.m_ObjectDestroyingCallback == nullptr);
        }
    }
    
    auto & NewViewedObject = m_ViewedObjectStack.emplace_back(Object);
    
    NewViewedObject.m_ObjectDestroyingCallback = std::bind(&UI::ObjectInformationDialog::m_OnCurrentObjectDestroying, this);
    if(Object.GetAspectName() != nullptr)
    {
        NewViewedObject.m_Title = "Object Information: " + Object.GetAspectName()->GetName();
    }
    else
    {
        NewViewedObject.m_Title = "Object Information";
    }
}

auto UI::ObjectInformationDialog::m_ViewObject(Object & Object) -> void
{
    m_PushObject(Object);
    m_Refresh();
}
