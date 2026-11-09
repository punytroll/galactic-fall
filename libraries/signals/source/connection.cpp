/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>

#include <signals/connection.h>
#include <signals/event.h>

Connection::Core::Core(void) :
    _Event(nullptr),
    _IsValid(false),
    _References(0)
{
}

Connection::Core::~Core(void)
{
    assert(_IsValid == false);
    assert(_References == 0);
}

Connection::Connection(void) :
    _Core(nullptr)
{
}

Connection::Connection(Core * Core) :
    _Core(Core)
{
    assert(_Core != nullptr);
    _Core->_IsValid = true;
    _Core->_References += 1;
}

Connection::Connection(const Connection & Connection) :
    _Core(Connection._Core)
{
    if(_Core != nullptr)
    {
        assert(_Core->_References > 0);
        _Core->_References += 1;
    }
}

Connection & Connection::operator=(const Connection & Other)
{
    if(this != &Other)
    {
        if(_Core != nullptr)
        {
            assert(_Core->_References > 0);
            _Core->_References -= 1;
            if(_Core->_References == 0)
            {
                _Core->_IsValid = false;
                delete _Core;
            }
            _Core = nullptr;
        }
        if(Other._Core != nullptr)
        {
            assert(Other._Core->_References > 0);
            _Core = Other._Core;
            _Core->_References += 1;
        }
    }
    
    return *this;
}

Connection::~Connection(void)
{
    if(_Core != nullptr)
    {
        assert(_Core->_References > 0);
        _Core->_References -= 1;
        if(_Core->_References == 0)
        {
            _Core->_IsValid = false;
            delete _Core;
        }
        _Core = nullptr;
    }
}

void Connection::Clear(void)
{
    if(_Core != nullptr)
    {
        assert(_Core->_References > 0);
        _Core->_References -= 1;
        if(_Core->_References == 0)
        {
            _Core->_IsValid = false;
            delete _Core;
        }
        _Core = nullptr;
    }
}

void Connection::Disconnect(void)
{
    assert(_Core != nullptr);
    assert(_Core->_Event != nullptr);
    assert(_Core->_IsValid == true);
    _Core->_Event->Disconnect(*this);
}

void Connection::_Invalidate(void)
{
    if(_Core != nullptr)
    {
        assert(_Core->_References > 0);
        _Core->_Event = nullptr;
        _Core->_IsValid = false;
        _Core->_References -= 1;
        if(_Core->_References == 0)
        {
            delete _Core;
        }
        _Core = nullptr;
    }
}
