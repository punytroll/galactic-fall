#include <cassert>
#include <memory>
#include <utility>

#include <signals/signal.h>
#include <signals/scoped_connection.h>

Signals::ScopedConnection::ScopedConnection(Signals::ScopedConnection && Other) :
    m_Core{std::exchange(Other.m_Core, nullptr)}
{
}

Signals::ScopedConnection::ScopedConnection(Signals::Connection && Connection) :
    m_Core{std::exchange(Connection.m_Core, nullptr)}
{
}

Signals::ScopedConnection::~ScopedConnection()
{
    if(IsConnected() == true)
    {
        Disconnect();
    }
    if(m_Core != nullptr)
    {
        m_ReleaseCore();
    }
}

auto Signals::ScopedConnection::operator=(Signals::ScopedConnection && Other) -> Signals::ScopedConnection &
{
    if(this != std::addressof(Other))
    {
        if(IsConnected() == true)
        {
            Disconnect();
        }
        if(m_Core != nullptr)
        {
            m_ReleaseCore();
        }
        assert(m_Core == nullptr);
        m_Core = std::exchange(Other.m_Core, nullptr);
    }
    
    return *this;
}

auto Signals::ScopedConnection::operator=(Signals::Connection && Connection) -> Signals::ScopedConnection &
{
    if(IsConnected() == true)
    {
        Disconnect();
    }
    if(m_Core != nullptr)
    {
        m_ReleaseCore();
    }
    assert(m_Core == nullptr);
    m_Core = std::exchange(Connection.m_Core, nullptr);
    
    return *this;
}

auto Signals::ScopedConnection::Disconnect() -> void
{
    assert(m_Core != nullptr);
    assert(m_Core->m_Signal != nullptr);
    m_Core->m_Signal->Disconnect(*this);
}

auto Signals::ScopedConnection::IsConnected() -> bool
{
    return (m_Core != nullptr) && (m_Core->m_Signal != nullptr);
}

auto Signals::ScopedConnection::m_ReleaseCore() -> void
{
    assert(m_Core != nullptr);
    assert(m_Core->m_References > 0);
    m_Core->m_References -= 1;
    if(m_Core->m_References == 0)
    {
        delete m_Core;
    }
    m_Core = nullptr;
}
