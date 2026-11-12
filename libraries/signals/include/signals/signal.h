/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__SIGNALS__INCLUDE__SIGNALS__SIGNAL_H
#define GALACTIC_FALL__LIBRARIES__SIGNALS__INCLUDE__SIGNALS__SIGNAL_H

#include <cassert>
#include <functional>
#include <list>

#include <signals/connection.h>

namespace Signals
{
    class SignalBase
    {
    public:
        virtual ~SignalBase() = default;
        virtual auto Disconnect(Signals::Connection & Connection) -> void = 0;
    };
    
    template<typename>
    inline constexpr auto UnsupportedSignalSignature = false;
    
    template<typename Signature>
    class Signal
    {
        static_assert(UnsupportedSignalSignature<Signature>, "Signals::Signal requires a function signature tempalte argument.");
    };
    
    template<typename ReturnType, typename ... ArgumentTypes>
    class Signal<auto (ArgumentTypes ...) -> ReturnType> : public SignalBase
    {
    private:
        class Core : public Connection::Core
        {
        public:
            typename std::function<auto (ArgumentTypes ...) -> ReturnType> m_Callback;
            typename std::list<Signal::Core *>::iterator m_Iterator;
        };
        
    public:
        Signal() = default;
        Signal(Signals::Signal<auto (ArgumentTypes ...) -> ReturnType> const & Other) = delete;
        Signal(Signals::Signal<auto (ArgumentTypes ...) -> ReturnType> && Other) = delete;
        auto operator=(Signals::Signal<auto (ArgumentTypes ...) -> ReturnType> const & Other) -> Signals::Signal<auto (ArgumentTypes ...) -> ReturnType> = delete;
        auto operator=(Signals::Signal<auto (ArgumentTypes ...) -> ReturnType> && Other) -> Signals::Signal<auto (ArgumentTypes ...) -> ReturnType> = delete;
        
        ~Signal() override
        {
            while(m_Cores.empty() == false)
            {
                auto Core = m_Cores.back();
                
                m_Cores.pop_back();
                assert(Core->m_Signal == this);
                Core->m_Signal = nullptr;
                Core->m_References -= 1;
                if(Core->m_References == 0)
                {
                    delete Core;
                }
            }
        }
        
        auto Connect(std::function<auto (ArgumentTypes ...) -> ReturnType> Callback) -> Signals::Connection
        {
            auto Core = new Signal::Core{};
            
            Core->m_Callback = Callback;
            Core->m_Signal = this;
            Core->m_References += 1;
            m_Cores.push_back(Core);
            Core->m_Iterator = --m_Cores.end();
            
            return Signals::Connection{Core};
        }
        
        auto Disconnect(Signals::Connection & Connection) -> void override
        {
            auto Core = static_cast<Signal::Core *>(Connection.m_Core);
            
            Core->m_Signal = nullptr;
            assert(Core->m_References > 0);
            Core->m_References -= 1;
            // because at least the Connection holds an additional reference
            assert(Core->m_References > 0);
            m_Cores.erase(Core->m_Iterator);
            Connection.m_ReleaseCore();
        }
        
        auto Emit(ArgumentTypes ... Arguments) -> void
        {
            auto CopyCores = m_Cores;
            
            for(auto Core : CopyCores)
            {
                Core->m_Callback(Arguments ...);
            }
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
        
    private:
        std::list<Signal::Core *> m_Cores;
    };
}

#endif
