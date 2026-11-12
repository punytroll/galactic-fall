/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__SIGNALS__INCLUDE__SIGNALS__EVENT_H
#define GALACTIC_FALL__LIBRARIES__SIGNALS__INCLUDE__SIGNALS__EVENT_H

#include <cassert>
#include <functional>
#include <list>

#include <signals/connection.h>

namespace Signals
{
    class EventBase
    {
    public:
        virtual ~EventBase() = default;
        virtual auto Disconnect(Signals::Connection & Connection) -> void = 0;
    };
    
    template<typename>
    inline constexpr auto UnsupportedEventSignature = false;
    
    template<typename Signature>
    class Event
    {
        static_assert(UnsupportedEventSignature<Signature>, "Signals::Event requires a function signature.");
    };
    
    template<typename ReturnType, typename ... ArgumentTypes>
    class Event<auto (ArgumentTypes ...) -> ReturnType> : public EventBase
    {
    private:
        class Core : public Connection::Core
        {
        public:
            typename std::function<auto (ArgumentTypes ...) -> ReturnType> m_Callback;
            typename std::list<Event::Core *>::iterator m_Iterator;
        };
        
    public:
        Event() = default;
        Event(Signals::Event<auto (ArgumentTypes ...) -> ReturnType> const & Other) = delete;
        Event(Signals::Event<auto (ArgumentTypes ...) -> ReturnType> && Other) = delete;
        auto operator=(Signals::Event<auto (ArgumentTypes ...) -> ReturnType> const & Other) -> Signals::Event<auto (ArgumentTypes ...) -> ReturnType> = delete;
        auto operator=(Signals::Event<auto (ArgumentTypes ...) -> ReturnType> && Other) -> Signals::Event<auto (ArgumentTypes ...) -> ReturnType> = delete;
        
        ~Event() override
        {
            while(m_Cores.empty() == false)
            {
                auto Core = m_Cores.back();
                
                m_Cores.pop_back();
                assert(Core->m_Event == this);
                Core->m_Event = nullptr;
                Core->m_IsValid = false;
                Core->m_References -= 1;
                if(Core->m_References == 0)
                {
                    delete Core;
                }
            }
        }
        
        auto Connect(std::function<auto (ArgumentTypes ...) -> ReturnType> Callback) -> Signals::Connection
        {
            auto Core = new Event::Core{};
            
            Core->m_Callback = Callback;
            Core->m_Event = this;
            Core->m_IsValid = true;
            Core->m_References += 1;
            m_Cores.push_back(Core);
            Core->m_Iterator = --m_Cores.end();
            
            return Connection(Core);
        }
        
        auto Disconnect(Signals::Connection & Connection) -> void override
        {
            auto Core = static_cast<Event::Core *>(Connection.GetCore());
            
            assert(Core->m_References > 0);
            Core->m_References -= 1;
            assert(Core->m_References > 0);
            m_Cores.erase(Core->m_Iterator);
            Connection.m_Invalidate();
        }
        
        auto GetCallbacks() const -> std::list<std::function<auto (ArgumentTypes ...) -> ReturnType>>
        {
            auto Result = std::list<std::function<auto (ArgumentTypes ...) -> ReturnType>>{};
            
            for(auto Core : m_Cores)
            {
                Result.push_back(Core->m_Callback);
            }
            
            return Result;
        }
        
        auto operator()(ArgumentTypes ... Arguments) -> void
        {
            auto CopyCores = std::list<Event::Core *>{m_Cores};
            
            for(auto Core : CopyCores)
            {
                Core->m_Callback(Arguments ...);
            }
        }
        
    private:
        std::list<Event::Core *> m_Cores;
    };
}

#endif
