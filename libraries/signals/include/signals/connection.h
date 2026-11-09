/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__SIGNALS__INCLUDE__SIGNALS__CONNECTION_H
#define GALACTIC_FALL__LIBRARIES__SIGNALS__INCLUDE__SIGNALS__CONNECTION_H

class EventBase;

class Connection
{
public:
    template < typename ReturnType, typename ... Types >
    friend class Event;
    
    class Core
    {
    public:
        Core(void);
        virtual ~Core(void);
        EventBase * _Event;
        bool _IsValid;
        int _References;
    };
    
    Connection(void);
    Connection(Core * Core);
    Connection(const Connection & Connection);
    Connection & operator=(const Connection & Other);
    ~Connection(void);
    void Clear(void);
    void Disconnect(void);
    bool IsValid(void);
    Core * GetCore(void);
private:
    void _Invalidate(void);
    Core * _Core;
};

inline bool Connection::IsValid(void)
{
    return (_Core != nullptr) && (_Core->_IsValid == true);
}

inline Connection::Core * Connection::GetCore(void)
{
    return _Core;
}

#endif
