/**
 * galactic-fall
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include "object.h"
#include "object_aspect_object_container.h"

ObjectAspectObjectContainer::ObjectAspectObjectContainer(Object & Object) :
    m_Object{Object}
{
}

ObjectAspectObjectContainer::~ObjectAspectObjectContainer()
{
    assert(m_Content.empty() == true);
}

auto ObjectAspectObjectContainer::AddContent(Object * Content) -> void
{
    assert(Content != nullptr);
    assert(Content->GetContainer() == nullptr);
    
    auto const & [Iterator, Success] = m_Content.insert(Content);
    
    assert(Success == true);
    Content->SetContainer(std::addressof(m_Object));
    m_ContentAddedSignal.Emit(Content);
}

auto ObjectAspectObjectContainer::ConnectContentAddedCallback(std::function<auto (Object *) -> void> Callback) -> Signals::Connection
{
    return m_ContentAddedSignal.Connect(Callback);
}

auto ObjectAspectObjectContainer::ConnectContentRemovedCallback(std::function<auto (Object *) -> void> Callback) -> Signals::Connection
{
    return m_ContentRemovedSignal.Connect(Callback);
}

auto ObjectAspectObjectContainer::GetAmount(std::string const & TypeIdentifier, std::string const & SubTypeIdentifier) const -> std::uint32_t
{
    auto Amount = std::uint32_t{0};
    
    for(auto Content : m_Content)
    {
        if((Content->GetTypeIdentifier() == TypeIdentifier) && (Content->GetSubTypeIdentifier() == SubTypeIdentifier))
        {
            Amount += 1;
        }
    }
    
    return Amount;
}

auto ObjectAspectObjectContainer::RemoveContent(Object * Content) -> void
{
    assert(Content != nullptr);
    assert(Content->GetContainer() == std::addressof(m_Object));
    
    auto ContentIterator = m_Content.find(Content);
        
    assert(ContentIterator != m_Content.end());
    Content->SetContainer(nullptr);
    m_Content.erase(ContentIterator);
    m_ContentRemovedSignal.Emit(Content);
}

auto ObjectAspectObjectContainer::Destroy() -> void
{
    // now delete and remove all content objects
    while(m_Content.empty() == false)
    {
        // save the pointer to Content because Destroy() will remove it from m_Content
        auto Content = *(m_Content.begin());
        
        Content->Destroy();
        delete Content;
    }
}
