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
            int m_References = 0;
        };
        
        Connection() = default;
        Connection(Signals::Connection::Core * Core);
        Connection(Signals::Connection const & Other);
        Connection(Signals::Connection && Other);
        ~Connection();
        auto operator=(Signals::Connection const & Other) -> Signals::Connection &;
        auto operator=(Signals::Connection && Other) -> Signals::Connection &;
        
        /**
         * @brief Disconnects the slot referred to by this connection.
         * @note It is a necessary precondition, that the connection refers to an actual connected slot in a signal, i.e. @a IsValid() returns true.
         **/
        auto Disconnect() -> void;
        
        /**
         * @brief Returns whether this connection refers to an actual connected slot in a signal.
         **/
        auto IsValid() -> bool;
        
    private:
        /**
         * @brief Releases the core and possibly deletes it.
         * @note The core must exist and its reference counter must be positive.
         **/
        auto m_ReleaseCore() -> void;
        
        Core * m_Core = nullptr;
    };
    
    inline auto Connection::IsValid() -> bool
    {
        return (m_Core != nullptr) && (m_Core->m_Signal != nullptr);
    }
}

#endif
