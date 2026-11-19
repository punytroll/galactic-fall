/**
 * galactic-fall
 * Copyright (C) 2008-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__SOURCE__OBJECT_ASPECT_OBJECT_CONTAINER_H
#define GALACTIC_FALL__SOURCE__OBJECT_ASPECT_OBJECT_CONTAINER_H

#include <cstdint>
#include <functional>
#include <set>
#include <string>

#include <signals/signal.h>

class Object;

class ObjectAspectObjectContainer
{
public:
    ObjectAspectObjectContainer(Object & Object);
    ~ObjectAspectObjectContainer();
    auto AddContent(Object * Content) -> void;
    auto ConnectContentAddedCallback(std::function<auto (Object *) -> void> Callback) -> Signals::Connection;
    auto ConnectContentRemovedCallback(std::function<auto (Object *) -> void> Callback) -> Signals::Connection;
    auto Destroy() -> void;
    auto GetContent() const -> std::set<Object *> const &;
    auto GetAmount(std::string const & TypeIdentifier, std::string const & ClassIdentifier) const -> std::uint32_t;
    auto RemoveContent(Object * Content) -> void;
    
private:
    std::set<Object *> m_Content;
    Object & m_Object;
    Signals::Signal<auto (Object *) -> void> m_ContentAddedSignal;
    Signals::Signal<auto (Object *) -> void> m_ContentRemovedSignal;
};

inline auto ObjectAspectObjectContainer::GetContent() const -> std::set<Object *> const &
{
    return m_Content;
}

#endif
