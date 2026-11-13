/**
 * galactic-fall
 * Copyright (C) 2026  Hagen Möbius
 * SPDX-License-Identifier: MIT
**/

#ifndef GALACTIC_FALL__LIBRARIES__SIGNALS__INCLUDE__SIGNALS__SCOPED_CONNECTION_H
#define GALACTIC_FALL__LIBRARIES__SIGNALS__INCLUDE__SIGNALS__SCOPED_CONNECTION_H

#include <signals/connection.h>

namespace Signals
{
    class ScopedConnection
    {
    public:
        template<typename Signature>
        friend class Signal;
        
        ScopedConnection() = default;
        ScopedConnection(Signals::ScopedConnection const & Other) = delete;
        ScopedConnection(Signals::ScopedConnection && Other);
        ScopedConnection(Signals::Connection && Connection);
        ~ScopedConnection();
        auto operator=(Signals::ScopedConnection const & Other) -> Signals::ScopedConnection & = delete;
        auto operator=(Signals::ScopedConnection && Other) -> Signals::ScopedConnection &;
        auto operator=(Signals::Connection && Connection) -> Signals::ScopedConnection &;
        
        /**
         * @brief Disconnects the slot referred to by this scoped connection.
         * @note It is a necessary precondition, that the connection refers to an actual connected slot in a signal, i.e. @a IsValid() returns true.
         **/
        auto Disconnect() -> void;
        
        /**
         * @brief Returns whether this connection refers to an actual connected slot in a signal.
         **/
        auto IsConnected() -> bool;
        
    private:
        /**
         * @brief Releases the core and possibly deletes it.
         * @note The core must exist and its reference counter must be positive.
         **/
        auto m_ReleaseCore() -> void;
        
        Signals::Connection::Core * m_Core = nullptr;
    };
}

#endif
