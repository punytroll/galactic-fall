/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>
#include <iostream>

#include <events/event.h>

auto g_Global = 0;

auto EmptyFunction() -> void
{
}

auto IncrementingFunction(int Value) -> void
{
    g_Global += Value;
}

class SelfDisconnecting
{
public:
    auto Register(Event<void> & Event) -> void
    {
        m_Connection = Event.Connect(std::bind(&SelfDisconnecting::m_Notify, this));
    }
private:
    auto m_Notify() -> void
    {
        m_Connection.Disconnect();
    }
    
    Connection m_Connection;
};

auto main([[maybe_unused]] int argc, [[maybe_unused]] char ** argv) -> int
{
    std::cout << "-----------------------------" << std::endl;
    
    {
        Connection Connection;
        
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Connection Connection;
        
        Connection.Clear();
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        
        assert(Event.GetCallbacks().empty() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        
        Event();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        
        Event.Connect(EmptyFunction);
        Event();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection(Event.Connect(EmptyFunction));
        
        assert(Connection.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        Connection Connection;
        
        Connection = Event.Connect(EmptyFunction);
        assert(Connection.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection(Event.Connect(EmptyFunction));
        
        Event.Disconnect(Connection);
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        Connection Connection;
        
        Connection = Event.Connect(EmptyFunction);
        Event.Disconnect(Connection);
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection(Event.Connect(EmptyFunction));
        
        Connection.Disconnect();
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        Connection Connection;
        
        Connection = Event.Connect(EmptyFunction);
        Connection.Disconnect();
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection1(Event.Connect(EmptyFunction));
        auto Connection2(Connection1);
        
        assert(Connection2.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection1(Event.Connect(EmptyFunction));
        auto Connection2(Connection1);
        
        Event();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection1(Event.Connect(EmptyFunction));
        auto Connection2(Connection1);
        
        Event.Disconnect(Connection1);
        assert(Connection1.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection1(Event.Connect(EmptyFunction));
        auto Connection2(Connection1);
        
        Event.Disconnect(Connection2);
        assert(Connection2.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection1(Event.Connect(EmptyFunction));
        auto Connection2(Connection1);
        
        Event.Disconnect(Connection1);
        assert(Connection2.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection1(Event.Connect(EmptyFunction));
        auto Connection2(Connection1);
        
        Event.Disconnect(Connection2);
        assert(Connection1.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection1(Event.Connect(EmptyFunction));
        auto Connection2(Connection1);
        Connection Connection3;
        
        Connection3 = Connection2;
        Event.Disconnect(Connection2);
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == false);
        assert(Connection3.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        auto Connection1(Event.Connect(EmptyFunction));
        auto Connection2(Connection1);
        Connection Connection3;
        
        Connection3 = Connection2;
        Event.Disconnect(Connection3);
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == false);
        assert(Connection3.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        g_Global = 32;
        
        Event< void > Event;
        
        Event.Connect(std::bind(IncrementingFunction, 8));
        Event();
        assert(g_Global == 40);
        Event();
        assert(g_Global == 48);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        g_Global = 32;
        
        Event< void > Event;
        auto Connection(Event.Connect(std::bind(IncrementingFunction, 8)));
        
        Event();
        assert(g_Global == 40);
        Event.Disconnect(Connection);
        Event();
        assert(g_Global == 40);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        g_Global = 32;
        
        Event< void > Event;
        auto Connection(Event.Connect(std::bind(IncrementingFunction, 8)));
        
        Event();
        assert(g_Global == 40);
        Connection.Disconnect();
        Event();
        assert(g_Global == 40);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        Connection Connection;
        
        Connection = Event.Connect(std::bind(EmptyFunction));
        Event.Disconnect(Connection);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        g_Global = 64;
        
        Event< void > Event;
        Connection Connection(Event.Connect(std::bind(IncrementingFunction, 78)));
        
        Event.Disconnect(Connection);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        g_Global = 64;
        
        Event< void > Event;
        Connection Connection1(Event.Connect(std::bind(IncrementingFunction, 8)));
        Connection Connection2(Event.Connect(std::bind(IncrementingFunction, 12)));
        Connection Connection3(Event.Connect(std::bind(IncrementingFunction, 7)));
        
        Event();
        assert(g_Global == 91);
        Event.Disconnect(Connection3);
        Event();
        assert(g_Global == 111);
        Event.Disconnect(Connection1);
        Event();
        assert(g_Global == 123);
        Event.Disconnect(Connection2);
        Event();
        assert(g_Global == 123);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Event< void > Event;
        SelfDisconnecting SelfDisconnecting;
        
        SelfDisconnecting.Register(Event);
        Event();
        Event();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Connection Connection;
        
        {
            Event< void > Event;
            
            Connection = Event.Connect(EmptyFunction);
        }
        assert(Connection.IsValid() == false);
    }
}
