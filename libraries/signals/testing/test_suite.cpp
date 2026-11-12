/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#include <cassert>
#include <iostream>

#include <signals/signal.h>

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
            auto Register(Signals::Signal<auto () -> void> & Signal) -> void
            {
                m_Connection = Signal.Connect(std::bind(&SelfDisconnecting::m_Notify, this));
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
        auto Signal = Signals::Signal<auto () -> void>{};
        
        assert(Signal.GetCallbacks().empty() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        
        Signal.Emit();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        
        Signal.Connect(Signals::Testing::EmptyFunction);
        Signal.Emit();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection = Signal.Connect(Signals::Testing::EmptyFunction);
        
        assert(Connection.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection = Signals::Connection{};
        
        Connection = Signal.Connect(Signals::Testing::EmptyFunction);
        assert(Connection.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection = Signal.Connect(Signals::Testing::EmptyFunction);
        
        Signal.Disconnect(Connection);
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection = Signals::Connection{};
        
        Connection = Signal.Connect(Signals::Testing::EmptyFunction);
        Signal.Disconnect(Connection);
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection = Signal.Connect(Signals::Testing::EmptyFunction);
        
        Connection.Disconnect();
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection = Signals::Connection{};
        
        Connection = Signal.Connect(Signals::Testing::EmptyFunction);
        Connection.Disconnect();
        assert(Connection.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        assert(Connection2.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        Signal.Emit();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        Signal.Disconnect(Connection1);
        assert(Connection1.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        Signal.Disconnect(Connection2);
        assert(Connection2.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        Signal.Disconnect(Connection1);
        assert(Connection2.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        
        Signal.Disconnect(Connection2);
        assert(Connection1.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        auto Connection3 = Signals::Connection{};
        
        Connection3 = Connection2;
        Signal.Disconnect(Connection2);
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == false);
        assert(Connection3.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Connection1;
        auto Connection3 = Signals::Connection{};
        
        Connection3 = Connection2;
        Signal.Disconnect(Connection3);
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == false);
        assert(Connection3.IsValid() == false);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(Signals::Testing::EmptyFunction);
        auto Connection2 = Signals::Connection{std::move(Connection1)};
        
        assert(Connection1.IsValid() == false);
        assert(Connection2.IsValid() == true);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(Signals::Testing::EmptyFunction);
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
        
        auto Signal = Signals::Signal<auto () -> void>{};
        
        Signal.Connect(std::bind(Signals::Testing::IncrementingFunction, 8));
        Signal.Emit();
        assert(Signals::Testing::g_Global == 40);
        Signal.Emit();
        assert(Signals::Testing::g_Global == 48);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Signals::Testing::g_Global = 32;
        
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection = Signal.Connect(std::bind(Signals::Testing::IncrementingFunction, 8));
        
        Signal.Emit();
        assert(Signals::Testing::g_Global == 40);
        Signal.Disconnect(Connection);
        Signal.Emit();
        assert(Signals::Testing::g_Global == 40);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Signals::Testing::g_Global = 32;
        
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection = Signal.Connect(std::bind(Signals::Testing::IncrementingFunction, 8));
        
        Signal.Emit();
        assert(Signals::Testing::g_Global == 40);
        Connection.Disconnect();
        Signal.Emit();
        assert(Signals::Testing::g_Global == 40);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection = Signals::Connection{};
        
        Connection = Signal.Connect(std::bind(Signals::Testing::EmptyFunction));
        Signal.Disconnect(Connection);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Signals::Testing::g_Global = 64;
        
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection = Signal.Connect(std::bind(Signals::Testing::IncrementingFunction, 78));
        
        Signal.Disconnect(Connection);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        Signals::Testing::g_Global = 64;
        
        auto Signal = Signals::Signal<auto () -> void>{};
        auto Connection1 = Signal.Connect(std::bind(Signals::Testing::IncrementingFunction, 8));
        auto Connection2 = Signal.Connect(std::bind(Signals::Testing::IncrementingFunction, 12));
        auto Connection3 = Signal.Connect(std::bind(Signals::Testing::IncrementingFunction, 7));
        
        Signal.Emit();
        assert(Signals::Testing::g_Global == 91);
        Signal.Disconnect(Connection3);
        Signal.Emit();
        assert(Signals::Testing::g_Global == 111);
        Signal.Disconnect(Connection1);
        Signal.Emit();
        assert(Signals::Testing::g_Global == 123);
        Signal.Disconnect(Connection2);
        Signal.Emit();
        assert(Signals::Testing::g_Global == 123);
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Signal = Signals::Signal<auto () -> void>{};
        auto SelfDisconnecting = Signals::Testing::SelfDisconnecting{};
        
        SelfDisconnecting.Register(Signal);
        Signal.Emit();
        Signal.Emit();
    }
    
    std::cout << "-----------------------------" << std::endl;
    
    {
        auto Connection = Signals::Connection{};
        
        {
            auto Signal = Signals::Signal<auto () -> void>{};
            
            Connection = Signal.Connect(Signals::Testing::EmptyFunction);
        }
        assert(Connection.IsValid() == false);
    }
}
