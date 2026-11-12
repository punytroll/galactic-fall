/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include <signals/connection.h>
#include <signals/signal.h>

Signals::Connection::Core::~Core()
{
    assert(m_References == 0);
}

Signals::Connection::Connection(Signals::Connection::Core * Core) :
    m_Core{Core}
{
    assert(m_Core != nullptr);
    m_Core->m_IsValid = true;
    m_Core->m_References += 1;
}

Signals::Connection::Connection(Signals::Connection const & Other) :
    m_Core{Other.m_Core}
{
    if(m_Core != nullptr)
    {
        assert(m_Core->m_References > 0);
        m_Core->m_References += 1;
    }
}

Signals::Connection::Connection(Signals::Connection && Other) :
    m_Core{Other.m_Core}
{
    if(m_Core != nullptr)
    {
        assert(m_Core->m_References > 0);
        Other.m_Core = nullptr;
    }
}

auto Signals::Connection::operator=(Signals::Connection const & Other) -> Signals::Connection &
{
    if(this != std::addressof(Other))
    {
        if(m_Core != nullptr)
        {
            assert(m_Core->m_References > 0);
            m_Core->m_References -= 1;
            if(m_Core->m_References == 0)
            {
                delete m_Core;
            }
            m_Core = nullptr;
        }
        if(Other.m_Core != nullptr)
        {
            assert(Other.m_Core->m_References > 0);
            m_Core = Other.m_Core;
            m_Core->m_References += 1;
        }
    }
    
    return *this;
}

auto Signals::Connection::operator=(Signals::Connection && Other) -> Signals::Connection &
{
    if(this != std::addressof(Other))
    {
        if(m_Core != nullptr)
        {
            assert(m_Core->m_References > 0);
            m_Core->m_References -= 1;
            if(m_Core->m_References == 0)
            {
                delete m_Core;
            }
            m_Core = nullptr;
        }
        if(Other.m_Core != nullptr)
        {
            assert(Other.m_Core->m_References > 0);
            m_Core = Other.m_Core;
            Other.m_Core = nullptr;
        }
    }
    
    return *this;
}

Signals::Connection::~Connection()
{
    Clear();
}

auto Signals::Connection::Clear() -> void
{
    if(m_Core != nullptr)
    {
        assert(m_Core->m_References > 0);
        m_Core->m_References -= 1;
        if(m_Core->m_References == 0)
        {
            delete m_Core;
        }
        m_Core = nullptr;
    }
}

auto Signals::Connection::Disconnect() -> void
{
    assert(m_Core != nullptr);
    assert(m_Core->m_Signal != nullptr);
    assert(m_Core->m_IsValid == true);
    m_Core->m_Signal->Disconnect(*this);
}

auto Signals::Connection::m_Invalidate() -> void
{
    if(m_Core != nullptr)
    {
        assert(m_Core->m_References > 0);
        m_Core->m_Signal = nullptr;
        m_Core->m_IsValid = false;
        m_Core->m_References -= 1;
        if(m_Core->m_References == 0)
        {
            delete m_Core;
        }
        m_Core = nullptr;
    }
}
