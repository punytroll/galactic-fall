/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>
#include <iostream>

#include <signals/event.h>

namespace Signals::Testing
{
    namespace
    {
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
            auto Register(Signals::Event<auto () -> void> & Event) -> void
            {
                m_Connection = Event.Connect(std::bind(&SelfDisconnecting::m_Notify, this));
            }
        private:
            auto m_Notify() -> void
            {
                m_Connection.Disconnect();
            }
            
            Signals::Connection m_Connection;
        };
    }
}

auto main([[maybe_unused]] int argc, [[maybe_unused]] char ** argv) -> int
{
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Connection = Signals::Connection{};
        
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Connection = Signals::Connection{};
        
        Connection.Clear();
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        
        assert(Event.GetCallbacks().empty() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        
        Event();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        
        Event.Connect(Signals::Testing::EmptyFunction);
        Event();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection = Event.Connect(Signals::Testing::EmptyFunction);
        
        assert(Connection.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection = Signals::Connection{};
        
        Connection = Event.Connect(Signals::Testing::EmptyFunction);
        assert(Connection.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection = Event.Connect(Signals::Testing::EmptyFunction);
        
        Event.Disconnect(Connection);
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection = Signals::Connection{};
        
        Connection = Event.Connect(Signals::Testing::EmptyFunction);
        Event.Disconnect(Connection);
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection = Event.Connect(Signals::Testing::EmptyFunction);
        
        Connection.Disconnect();
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection = Signals::Connection{};
        
        Connection = Event.Connect(Signals::Testing::EmptyFunction);
        Connection.Disconnect();
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        assert(Connection2.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        Event();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        Event.Disconnect(Connection1);
        assert(Connection1.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        Event.Disconnect(Connection2);
        assert(Connection2.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        Event.Disconnect(Connection1);
        assert(Connection2.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        Event.Disconnect(Connection2);
        assert(Connection1.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        auto Connection3 = Signals::Connection{};
        
        Connection3 = Connection2;
        Event.Disconnect(Connection2);
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == false);
        assert(Connection3.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        auto Connection3 = Signals::Connection{};
        
        Connection3 = Connection2;
        Event.Disconnect(Connection3);
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == false);
        assert(Connection3.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Signals::Connection{std::move(Connection1)};
        
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Signals::Connection{};
        
        Connection2 = std::move(Connection1);
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Connection1 = Signals::Connection{};
        auto Connection2 = Signals::Connection{};
        
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == false);
        Connection2 = std::move(Connection1);
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Connection1 = Signals::Connection{};
        
        assert(Connection1.IsValid() == false);
        
        auto Connection2 = std::move(Connection1);
        
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Signals::Testing::g_Global = 32;
        
        auto Event = Signals::Event<auto () -> void>{};
        
        Event.Connect(std::bind(Signals::Testing::IncrementingFunction, 8));
        Event();
        assert(Signals::Testing::g_Global == 40);
        Event();
        assert(Signals::Testing::g_Global == 48);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Signals::Testing::g_Global = 32;
        
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection = Event.Connect(std::bind(Signals::Testing::IncrementingFunction, 8));
        
        Event();
        assert(Signals::Testing::g_Global == 40);
        Event.Disconnect(Connection);
        Event();
        assert(Signals::Testing::g_Global == 40);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Signals::Testing::g_Global = 32;
        
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection = Event.Connect(std::bind(Signals::Testing::IncrementingFunction, 8));
        
        Event();
        assert(Signals::Testing::g_Global == 40);
        Connection.Disconnect();
        Event();
        assert(Signals::Testing::g_Global == 40);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection = Signals::Connection{};
        
        Connection = Event.Connect(std::bind(Signals::Testing::EmptyFunction));
        Event.Disconnect(Connection);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Signals::Testing::g_Global = 64;
        
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection = Event.Connect(std::bind(Signals::Testing::IncrementingFunction, 78));
        
        Event.Disconnect(Connection);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Signals::Testing::g_Global = 64;
        
        auto Event = Signals::Event<auto () -> void>{};
        auto Connection1 = Event.Connect(std::bind(Signals::Testing::IncrementingFunction, 8));
        auto Connection2 = Event.Connect(std::bind(Signals::Testing::IncrementingFunction, 12));
        auto Connection3 = Event.Connect(std::bind(Signals::Testing::IncrementingFunction, 7));
        
        Event();
        assert(Signals::Testing::g_Global == 91);
        Event.Disconnect(Connection3);
        Event();
        assert(Signals::Testing::g_Global == 111);
        Event.Disconnect(Connection1);
        Event();
        assert(Signals::Testing::g_Global == 123);
        Event.Disconnect(Connection2);
        Event();
        assert(Signals::Testing::g_Global == 123);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Event = Signals::Event<auto () -> void>{};
        auto SelfDisconnecting = Signals::Testing::SelfDisconnecting{};
        
        SelfDisconnecting.Register(Event);
        Event();
        Event();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Connection = Signals::Connection{};
        
        {
            auto Event = Signals::Event<auto () -> void>{};
            
            Connection = Event.Connect(Signals::Testing::EmptyFunction);
        }
        assert(Connection.IsValid() == false);
    }
}
