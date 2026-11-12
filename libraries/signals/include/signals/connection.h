/**
 * galactic-fall
 * Copyright (C) 2014-2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__SIGNALS__INCLUDE__SIGNALS__CONNECTION_H
#define GALACTIC_FALL__LIBRARIES__SIGNALS__INCLUDE__SIGNALS__CONNECTION_H

namespace Signals
{
    class SignalBase;
    
    class Connection
    {
    public:
        template<typename Signature>
        friend class Signal;
        
        class Core
        {
        public:
            virtual ~Core();
            
            SignalBase * m_Signal = nullptr;
            bool m_IsValid = false;
            int m_References = 0;
        };
        
        Connection() = default;
        Connection(Signals::Connection::Core * Core);
        Connection(Signals::Connection const & Other);
        Connection(Signals::Connection && Other);
        auto operator=(Signals::Connection const & Other) -> Signals::Connection &;
        auto operator=(Signals::Connection && Other) -> Signals::Connection &;
        ~Connection();
        auto Clear() -> void;
        auto Disconnect() -> void;
        auto IsValid() -> bool;
        auto GetCore() -> Signals::Connection::Core *;
        
    private:
        auto m_Invalidate() -> void;
        
        Core * m_Core = nullptr;
    };
    
    inline auto Connection::IsValid() -> bool
    {
        return (m_Core != nullptr) && (m_Core->m_IsValid == true);
    }
    
    inline auto Connection::GetCore() -> Signals::Connection::Core *
    {
        return m_Core;
    }
}

#endif
